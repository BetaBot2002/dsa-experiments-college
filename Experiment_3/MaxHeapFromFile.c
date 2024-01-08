#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<stdbool.h>

#define MAX_STUDENT_NUMBER 100
#define MAX_NAME_LENGTH 50

typedef struct Student{
    int rollNumber;
    char name[MAX_NAME_LENGTH];
    float totalMarks;
} Student;

typedef struct MaxHeap{
    Student studentArray[MAX_STUDENT_NUMBER];
    int size;
} MaxHeap;

void printStudent(Student student){
    printf("\t%d\t%s\t%.2f\n",student.rollNumber,student.name,student.totalMarks);
}

void writeStudentInFile(Student student,FILE* file){
    fprintf(file, "%d,%s,%.2f\n", student.rollNumber, student.name, student.totalMarks);
}

Student createStudent(int rollNumber,char* name,float totalMarks){
    Student newStudent;
    newStudent.rollNumber=rollNumber;
    strcpy(newStudent.name,name);
    newStudent.totalMarks=totalMarks;
    return newStudent;
}

MaxHeap* createMaxHeap(){
    MaxHeap* newMaxHeap=(MaxHeap*)malloc(sizeof(MaxHeap));
    newMaxHeap->size=0;
    return newMaxHeap;
}

void swap(Student *a,Student *b){
    Student temp=*a;
    *a=*b;
    *b=temp;
    printf("Swapped %d,%d\n",a->rollNumber,b->rollNumber);
}

void maxHeapify(MaxHeap* heap,int index){
    int max=index;
    int left=2*index+1;
    int right=2*index+2;

    if(left<heap->size && heap->studentArray[max].rollNumber<heap->studentArray[left].rollNumber) {
        printf("In IF 1\n");
        max=left;
    }
    if(right<heap->size && heap->studentArray[max].rollNumber<heap->studentArray[right].rollNumber) max=right;

    if(max!=index){
        swap(&heap->studentArray[max],&heap->studentArray[index]);
        maxHeapify(heap,max);
    }

}

void insert(MaxHeap* heap,Student student){
    if(heap->size==MAX_STUDENT_NUMBER){
        printf("Heap Overflow. Cannot Insert!!\n");
        return;
    }

    heap->studentArray[heap->size++]=student;
    for (int i = heap->size / 2 - 1; i >= 0; --i) {
        maxHeapify(heap, i);
    }
}

void inorderTraversal(MaxHeap* heap,int index,FILE* file){
    if(index>=heap->size) return;
    
    inorderTraversal(heap,2*index+1,file);
    writeStudentInFile(heap->studentArray[index],file);
    inorderTraversal(heap,2*index+2,file);
    
}

void preorderTraversal(MaxHeap* heap,int index,FILE* file){
    if(index>=heap->size) return;
    
    writeStudentInFile(heap->studentArray[index],file);
    preorderTraversal(heap,2*index+1,file);
    preorderTraversal(heap,2*index+2,file);
    
}

void postorderTraversal(MaxHeap* heap,int index,FILE* file){
    if(index>=heap->size) return;
    
    postorderTraversal(heap,2*index+1,file);
    postorderTraversal(heap,2*index+2,file);
    writeStudentInFile(heap->studentArray[index],file);
    
}

void traverseIntoFile(MaxHeap* heap,char* traversalType,char* fileName){
    fclose(fopen(fileName,"w")); //To clear anything in the file before opening it in append mode, so that no recurring data exists 
    FILE *traversal=fopen(fileName,"a");

    if(traversal==NULL){
        printf("ERROR IN OPENING FILE!!\n");
        return;
    }

    if(!strcmp("InOrder",traversalType)){
        inorderTraversal(heap,0,traversal);
    }else if(!strcmp("PreOrder",traversalType)){
        preorderTraversal(heap,0,traversal);
    }else if(!strcmp("PostOrder",traversalType)){
        postorderTraversal(heap,0,traversal);
    }

    fclose(traversal);
}

int main(){
    MaxHeap* heap=createMaxHeap();
    FILE *studentData=fopen("StudentData.txt","r");

    if(studentData==NULL){
        printf("ERROR IN OPENING FILE!!\n");
        return 1;
    }

    int currentRollNumber;
    char currentName[MAX_NAME_LENGTH];
    float currentTotalMarks;

    while(fscanf(studentData,"%d,%[^,],%f",&currentRollNumber,currentName,&currentTotalMarks)!=EOF){
        printf("Inserting Node with roll number %d ...\n", currentRollNumber);
        insert(heap,createStudent(currentRollNumber,currentName,currentTotalMarks));
        printf("Node with roll number %d inserted successfully.\n\n", currentRollNumber);
    }

    fclose(studentData);

    // printf("The In Order Traversal of the Tree is:\n");
    // inorderTraversal(root);

    // printf("The Pre Order Traversal of the Tree is:\n");
    // preorderTraversal(root);
 
    // printf("The Post Order Traversal of the Tree is:\n");
    // postorderTraversal(root);

    traverseIntoFile(heap,"InOrder","InOrder.txt");
    traverseIntoFile(heap,"PreOrder","PreOrder.txt");
    traverseIntoFile(heap,"PostOrder","PostOrder.txt");

    for(int i=0;i<heap->size;i++){
        printStudent(heap->studentArray[i]);
    }

    return 0;
}