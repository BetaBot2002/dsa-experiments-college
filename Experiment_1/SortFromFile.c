#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

#define MAX_STUDENT_NUMBER 100
#define MAX_NAME_LENGTH 50

typedef struct Student{
    int rollNumber;
    char name[MAX_NAME_LENGTH];
    float totalMarks;
} Student;

void printStudentArray(Student studentArray[],int numberOfStudents){
    for(int i=0;i<numberOfStudents;i++){
        printf("\t%d\t%s\t%f\n",studentArray[i].rollNumber,studentArray[i].name,studentArray[i].totalMarks);
    }
}

int main(){
    FILE *studentData=fopen("StudentData.txt","r");

    if(studentData==NULL){
        printf("ERROR IN OPENING FILE!!\n");
        return 1;
    }

    Student students[MAX_STUDENT_NUMBER];
    int numberOfStudents=0;

    while(fscanf(studentData,"%d,%[^,],%f",&students[numberOfStudents].rollNumber,students[numberOfStudents].name,&students[numberOfStudents].totalMarks)!=EOF){
        numberOfStudents++;
    }

    fclose(studentData);

    printf("The Student Data:\n");
    printStudentArray(students,numberOfStudents);
    return 0;
}