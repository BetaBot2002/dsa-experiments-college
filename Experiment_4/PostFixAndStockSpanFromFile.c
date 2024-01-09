#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<stdbool.h>
#include<math.h>

#define MAX_STACK_SIZE 100
#define WRONG_SYMBOL -987064256

typedef struct Stack{
    int stackData[MAX_STACK_SIZE];
    int top;
} Stack;

Stack* createStack(){
    Stack* newStack=(Stack*)malloc(sizeof(Stack));
    newStack->top=-1;
    return newStack;
}

bool isFull(Stack* stack){
    if(stack->top>=MAX_STACK_SIZE) return true;
    return false;
}

bool isEmpty(Stack* stack){
    if(stack->top==-1) return true;
    return false;
}

void push(Stack* stack,int data){
    if(isFull(stack)){
        printf("STACK OVERFLOW!!\n");
        return;
    }

    stack->stackData[++(stack->top)]=data;
}

int pop(Stack* stack){
    if(isEmpty(stack)){
        printf("STACK UNDERFLOW!!\n");
        return -1;
    }

    int popped=stack->stackData[stack->top--];
    return popped;
}

int peek(Stack* stack){
    if(isEmpty(stack)){
        printf("STACK UNDERFLOW!!\n");
        return -1;
    }

    return stack->stackData[stack->top];
}

bool isDigit(char input){
    return input>='0' && input<='9';
}

int toInteger(char string[]){
    int number=0,count=0;
    while(isDigit(string[count])){
        number=number*10+(string[count]-'0');
        count++;
    }
    return number;
}

int evaluate(int a,int b,char symbol){
    int result=0;
    switch (symbol){
        case '+':
            result=a+b;
            break;
        case '-':
            result=a-b;
            break;
        case '*':
            result=a*b;
            break;
        case '/':
            result=a/b;
            break;
        case '^':
            result=pow(a,b);
            break;
        default:
            printf("WRONG SYMBOL!!! Skipping Symbol: %c\n",symbol);
            return WRONG_SYMBOL;
            break;
    }
    return result;
}

void evaluatePostFix(Stack* stack,char input){
    int a=pop(stack);
    int b=pop(stack);
    int result=evaluate(a,b,input);
    if(result==WRONG_SYMBOL){
        push(stack,b);
        push(stack,a);
    }else{
        push(stack,result);
    }
}

void solveStockSpan(Stack* stack,int prices[],int spans[],int numberOfPrices){
    for(int i=0;i<numberOfPrices;i++) spans[i]=0;
    for(int i=0;i<numberOfPrices;i++){
        while (!isEmpty(stack) && prices[peek(stack)]<=prices[i]){
            pop(stack);
        }
        if(isEmpty(stack)){
            spans[i]=i+1;
            push(stack,i);
        }else{
            spans[i]=i-peek(stack);
            push(stack,i);
        }
    }
}

void printArray(int arr[], int size) {
  for (int i = 0; i < size; printf("%d ", arr[i++]));
  printf("\n");
}

int main(){
    FILE *postFixData=fopen("PostFixData.txt","r");

    if(postFixData==NULL){
        printf("ERROR IN OPENING FILE!!\n");
        return 1;
    }

    char line[100];
    Stack *postFixStack=createStack();

    while(fgets(line, sizeof(line), postFixData) != NULL){
        if(isDigit(*line)){
            int number=toInteger(line);
            push(postFixStack,number);
        }else{
            evaluatePostFix(postFixStack,*line);
        }
    }

    fclose(postFixData);

    int postFixEavluated=peek(postFixStack);
    printf("%d\n",postFixEavluated);

    FILE *stockSpanData=fopen("StockSpanData.txt","r");
    if(stockSpanData==NULL){
        printf("ERROR IN OPENING FILE!!\n");
        return 1;
    }

    Stack* stockSpanStack=createStack();
    int prices[100];
    int numberOfPrices=0;

    while(fgets(line, sizeof(line), stockSpanData) != NULL){
        if(isDigit(*line)){
            int number=toInteger(line);
            prices[numberOfPrices++]=number;
        }
    }
    int *spans=(int*)malloc(numberOfPrices*sizeof(int));
    
    solveStockSpan(stockSpanStack,prices,spans,numberOfPrices);
    printArray(prices,numberOfPrices);
    printArray(spans,numberOfPrices);

    fclose(stockSpanData);

    return 0;
}