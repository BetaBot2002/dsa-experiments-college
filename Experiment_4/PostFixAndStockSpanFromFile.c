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

int evaluate(int a,int b,char symbol,FILE* file){
    int result=0;
    bool isDefaultCase=false;
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
            fprintf(file,"\t> WRONG SYMBOL!!! Skipping Symbol: %c\n",symbol);
            isDefaultCase=true;
            break;
    }
    if(isDefaultCase) return WRONG_SYMBOL;
    else{
        fprintf(file,"\t> Performed %d%c%d=",a,symbol,b);
        fprintf(file,"%d\n",result);
        return result;
    }
}

void evaluatePostFix(Stack* stack,char input,FILE* file){
    int a=pop(stack);
    int b=pop(stack);
    fprintf(file,"\t> Data Popped: %d\n",a);
    fprintf(file,"\t> Data Popped: %d\n",b);

    int result=evaluate(a,b,input,file);
    if(result==WRONG_SYMBOL){
        push(stack,b);
        push(stack,a);
        fprintf(file,"\t> Data Pushed Again: %d\n",b);
        fprintf(file,"\t> Data Pushed Again: %d\n",a);
    }else{
        push(stack,result);
        fprintf(file,"Result Pushed: %d\n",result);
    }
}

void solveStockSpan(Stack* stack,int prices[],int spans[],int numberOfPrices,FILE* file){
    for(int i=0;i<numberOfPrices;i++) spans[i]=0;
    for(int i=0;i<numberOfPrices;i++){
        fprintf(file,"Day %d (Actual Index %d):\n",i+1,i);
        fprintf(file,"\tPrice: %d\n",prices[i]);
        while (!isEmpty(stack) && prices[peek(stack)]<=prices[i]){
            int index=pop(stack);
            fprintf(file,"\tPopped Index: %d\n",index);
        }
        if(isEmpty(stack)){
            spans[i]=i+1;
            fprintf(file,"\tStack is empty, So, Calculation of span of Day %d will be %d+1\n",i+1,i);
            push(stack,i);
        }else{
            spans[i]=i-peek(stack);
            fprintf(file,"\tStack is not empty, So, Calculation of span of Day %d will be %d-%d\n",i+1,i,peek(stack));
            push(stack,i);
        }
        fprintf(file,"\tPushed Index: %d\n",i);
        fprintf(file,"\tSpan of Day %d: %d\n",i+1,spans[i]);
    }
}

void printArrayToFile(int arr[], int size,FILE* file) {
  for (int i = 0; i < size; fprintf(file,"%d ", arr[i++]));
  fprintf(file,"\n");
}

int main(){
    FILE *postFixData=fopen("PostFixData.txt","r");

    if(postFixData==NULL){
        printf("ERROR IN OPENING FILE!!\n");
        return 1;
    }

    char line[100];
    Stack *postFixStack=createStack();

    FILE *postFixOutput=fopen("PostFixOutput.txt","w");

    if(postFixOutput==NULL){
        printf("ERROR IN OPENING FILE!!\n");
        return 1;
    }

    while(fgets(line, sizeof(line), postFixData) != NULL){
        if(isDigit(*line)){
            int number=toInteger(line);
            fprintf(postFixOutput,"Reading Data: %d\n",number);
            push(postFixStack,number);
            fprintf(postFixOutput,"\t> %d Pushed to Stack\n",number);
        }else{
            fprintf(postFixOutput,"Reading Symbol: %c\n",*line);
            evaluatePostFix(postFixStack,*line,postFixOutput);
        }
    }

    fclose(postFixData);

    int postFixEavluated=peek(postFixStack);
    fprintf(postFixOutput,"Result of the Postfix: %d\n",postFixEavluated);
    fclose(postFixOutput);

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
    fclose(stockSpanData);

    FILE *stockSpanOutput=fopen("StockSpanOutput.txt","w");
    if(stockSpanOutput==NULL){
        printf("ERROR IN OPENING FILE!!\n");
        return 1;
    }

    fprintf(stockSpanOutput,"Given Prices: ");
    printArrayToFile(prices,numberOfPrices,stockSpanOutput);
    
    solveStockSpan(stockSpanStack,prices,spans,numberOfPrices,stockSpanOutput);

    fprintf(stockSpanOutput,"Stock Spans: ");
    printArrayToFile(spans,numberOfPrices,stockSpanOutput);
    fprintf(stockSpanOutput,"Breakdown of Spans:\n");
    for(int i=0;i<numberOfPrices;i++){
        fprintf(stockSpanOutput,"Stock Span of Day %d: %d\n",i+1,spans[i]);
    }

    fclose(stockSpanOutput);
    return 0;
}