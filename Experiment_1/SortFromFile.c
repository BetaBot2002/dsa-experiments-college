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

void copyArray(Student main[],Student copy[],int numberOfStudents){
    for(int i=0;i<numberOfStudents;i++){
        copy[i]=main[i];
    }
}

void swap(Student *a,Student *b){
    Student temp=*a;
    *a=*b;
    *b=temp;
}

void bubbleSort(Student studentArray[],int numberOfStudents){
    for(int i=0;i<numberOfStudents-1;i++){
        bool swapped=false;
        for(int j=0;j<numberOfStudents-i-1;j++){
            if(studentArray[j].rollNumber>studentArray[j+1].rollNumber){
                 swap(&studentArray[j],&studentArray[j+1]);
                 swapped=true;
            }
        }
        if(!swapped) break;
    }
}

int compareByRollNumber(const void *a,const void* b){
    return ((Student*)a)->rollNumber-((Student*)b)->rollNumber;
}

int compareByName(const void *a,const void* b){
    return strcmp(((Student*)a)->name,((Student*)b)->name);
}

int compareByTotalMarks(const void *a,const void* b){
    return ((Student*)a)->totalMarks-((Student*)b)->totalMarks;
}

void printStudentArray(Student studentArray[],int numberOfStudents){
    for(int i=0;i<numberOfStudents;i++){
        printf("\t%d\t%s\t%.2f\n",studentArray[i].rollNumber,studentArray[i].name,studentArray[i].totalMarks);
    }
}

void writeIntoFile(char* fileName,Student studentArray[],int numberOfStudents){
    FILE *sortedData=fopen(fileName,"w");

    if(sortedData==NULL){
        printf("ERROR IN OPENING FILE!!\n");
        return;
    }

    for(int i=0;i<numberOfStudents;i++){
        fprintf(sortedData, "%d,%s,%.2f\n", studentArray[i].rollNumber, studentArray[i].name, studentArray[i].totalMarks);
    }

    fclose(sortedData);
}

int main(){
    FILE *studentData=fopen("StudentData.txt","r");

    if(studentData==NULL){
        printf("ERROR IN OPENING FILE!!\n");
        return 1;
    }

    Student students[MAX_STUDENT_NUMBER];
    Student copyOfStudents[MAX_STUDENT_NUMBER];
    int numberOfStudents=0;

    while(fscanf(studentData,"%d,%[^,],%f",&students[numberOfStudents].rollNumber,students[numberOfStudents].name,&students[numberOfStudents].totalMarks)!=EOF){
        numberOfStudents++;
    }

    fclose(studentData);

    printf("The Student Data:\n");
    printStudentArray(students,numberOfStudents);

    copyArray(students,copyOfStudents,numberOfStudents);
    bubbleSort(copyOfStudents,numberOfStudents);

    printf("The Student Data after sorting [By roll, bubblesort]:\n");
    printStudentArray(copyOfStudents,numberOfStudents);
    writeIntoFile("BubbleSort.txt",copyOfStudents,numberOfStudents);

    copyArray(students,copyOfStudents,numberOfStudents);
    qsort(copyOfStudents,numberOfStudents,sizeof(Student),compareByRollNumber);

    printf("The Student Data after sorting [By roll, qsort]:\n");
    printStudentArray(copyOfStudents,numberOfStudents);
    writeIntoFile("QuickSortRoll.txt",copyOfStudents,numberOfStudents);

    copyArray(students,copyOfStudents,numberOfStudents);
    qsort(copyOfStudents,numberOfStudents,sizeof(Student),compareByTotalMarks);

    printf("The Student Data after sorting [By total marks, qsort]:\n");
    printStudentArray(copyOfStudents,numberOfStudents);
    writeIntoFile("QuickSortMarks.txt",copyOfStudents,numberOfStudents);

    copyArray(students,copyOfStudents,numberOfStudents);
    qsort(copyOfStudents,numberOfStudents,sizeof(Student),compareByName);

    printf("The Student Data after sorting [By name, qsort]:\n");
    printStudentArray(copyOfStudents,numberOfStudents);
    writeIntoFile("QuickSortNames.txt",copyOfStudents,numberOfStudents);
    return 0;
}