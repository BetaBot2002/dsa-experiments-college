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

double calculateCpuSpeed(clock_t start,clock_t end){
    return ((double) end-start)/CLOCKS_PER_SEC;
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
    clock_t bubble_sort_start,bubble_sort_end;
    clock_t quick_sort_roll_start,quick_sort_roll_end;
    clock_t quick_sort_name_start,quick_sort_name_end;
    clock_t quick_sort_marks_start,quick_sort_marks_end;

    while(fscanf(studentData,"%d,%[^,],%f",&students[numberOfStudents].rollNumber,students[numberOfStudents].name,&students[numberOfStudents].totalMarks)!=EOF){
        numberOfStudents++;
    }

    fclose(studentData);

    printf("The Student Data:\n");
    printStudentArray(students,numberOfStudents);

    //Bubble Sort By Roll
    copyArray(students,copyOfStudents,numberOfStudents);
    bubble_sort_start=clock();
    bubbleSort(copyOfStudents,numberOfStudents);
    bubble_sort_end=clock();

    printf("The Student Data after sorting [By roll, bubblesort]:\n");
    // printStudentArray(copyOfStudents,numberOfStudents);
    writeIntoFile("BubbleSort.txt",copyOfStudents,numberOfStudents);

    //Quick Sort By Roll
    copyArray(students,copyOfStudents,numberOfStudents);
    quick_sort_roll_start=clock();
    qsort(copyOfStudents,numberOfStudents,sizeof(Student),compareByRollNumber);
    quick_sort_roll_end=clock();

    printf("The Student Data after sorting [By roll, qsort]:\n");
    // printStudentArray(copyOfStudents,numberOfStudents);
    writeIntoFile("QuickSortRoll.txt",copyOfStudents,numberOfStudents);

    //Quick Sort By Marks
    copyArray(students,copyOfStudents,numberOfStudents);
    quick_sort_marks_start=clock();
    qsort(copyOfStudents,numberOfStudents,sizeof(Student),compareByTotalMarks);
    quick_sort_marks_end=clock();

    printf("The Student Data after sorting [By total marks, qsort]:\n");
    // printStudentArray(copyOfStudents,numberOfStudents);
    writeIntoFile("QuickSortMarks.txt",copyOfStudents,numberOfStudents);

    //Quick Sort By Names
    copyArray(students,copyOfStudents,numberOfStudents);
    quick_sort_name_start=clock();
    qsort(copyOfStudents,numberOfStudents,sizeof(Student),compareByName);
    quick_sort_name_end=clock();

    printf("The Student Data after sorting [By name, qsort]:\n");
    // printStudentArray(copyOfStudents,numberOfStudents);
    writeIntoFile("QuickSortNames.txt",copyOfStudents,numberOfStudents);

    //Perfomances
    printf("PERFORMANCES OF THE SORTING METHODS\n");
    printf("Bubble sort: %f s\n", calculateCpuSpeed(bubble_sort_start,bubble_sort_end));
    printf("qsort (by roll no.): %f s\n", calculateCpuSpeed(quick_sort_roll_start,quick_sort_roll_end));
    printf("qsort (by name): %f s\n", calculateCpuSpeed(quick_sort_name_start,quick_sort_name_end));
    printf("qsort (by marks): %f s\n", calculateCpuSpeed(quick_sort_marks_start,quick_sort_marks_end));
    return 0;
}