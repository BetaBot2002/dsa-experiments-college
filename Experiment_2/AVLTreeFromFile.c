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

typedef struct AVLNode{
    Student data;
    struct AVLNode* left;
    struct AVLNode* right;
    int height;
} AVLNode;

AVLNode* createNode(Student data){
    AVLNode* newNode=(AVLNode*)malloc(sizeof(AVLNode));
    newNode->data=data;
    newNode->left=NULL;
    newNode->right=NULL;
    newNode->height=1;
    return newNode;
}

void printStudent(Student student){
    printf("\t%d\t%s\t%.2f\n",student.rollNumber,student.name,student.totalMarks);
}

void writeStudentInFile(Student student,FILE* file){
    fprintf(file, "%d,%s,%.2f\n", student.rollNumber, student.name, student.totalMarks);
}

int getHeight(AVLNode* root){
    if(root==NULL) return 0;
    return root->height;
}

int getBalance(AVLNode* root){
    if(root==NULL) return 0;
    return getHeight(root->left)-getHeight(root->right);
}

int maxOf(int a,int b){
    return a>b?a:b;
}

AVLNode* leftRotate(AVLNode* root){
    AVLNode* current=root->right;
    AVLNode* temp=current->left;

    current->left=root;
    root->right=temp;

    root->height=1+maxOf(getHeight(root->left),getHeight(root->right));
    current->height=1+maxOf(getHeight(current->left),getHeight(current->right));

    return current;
}

AVLNode* rightRotate(AVLNode* root){
    AVLNode* current=root->left;
    AVLNode* temp=current->right;

    current->right=root;
    root->left=temp;

    root->height=1+maxOf(getHeight(root->left),getHeight(root->right));
    current->height=1+maxOf(getHeight(current->left),getHeight(current->right));

    return current;
}

AVLNode* insertNode(AVLNode* root,Student data){
    if (root==NULL){ 
        printf("Student Inserted: %d\n",data.rollNumber);
        return createNode(data);
    }
    if(data.rollNumber<root->data.rollNumber) root->left=insertNode(root->left,data);
    else if(data.rollNumber>root->data.rollNumber) root->right=insertNode(root->right,data);

    root->height=1+maxOf(getHeight(root->left),getHeight(root->right));

    int balance=getBalance(root);

    if(balance>1 && data.rollNumber<root->left->data.rollNumber){
        printf("Right Rotation Performed.\n");
        return rightRotate(root);
    }
    if(balance<-1 && data.rollNumber>root->right->data.rollNumber){
        printf("Left Rotation Performed.\n");
        return leftRotate(root);
    }
    if(balance>1 && data.rollNumber>root->left->data.rollNumber){
        printf("Left-Right Rotation Performed.\n");
        root->left=leftRotate(root->left);
        return rightRotate(root);
    }
    if(balance<-1 && data.rollNumber<root->right->data.rollNumber){
        printf("Right-Left Rotation Performed.\n");
        root->right=rightRotate(root->right);
        return leftRotate(root);
    }

    printf("No Rotation Performed. Tree Balanced.\n");
    return root;
    
}


void inorderTraversal(AVLNode* root,FILE* file){
    if(root!=NULL){
        inorderTraversal(root->left,file);
        writeStudentInFile(root->data,file);
        inorderTraversal(root->right,file);
    }
}

void preorderTraversal(AVLNode* root,FILE* file){
    if(root!=NULL){
        writeStudentInFile(root->data,file);
        preorderTraversal(root->left,file);
        preorderTraversal(root->right,file);
    }
}

void postorderTraversal(AVLNode* root,FILE* file){
    if(root!=NULL){
        postorderTraversal(root->left,file);
        postorderTraversal(root->right,file);
        writeStudentInFile(root->data,file);
    }
}

// void inorderTraversal(AVLNode* root){
//     if(root!=NULL){
//         inorderTraversal(root->left);
//         writeStudentInFile(root->data);
//         inorderTraversal(root->right);
//     }
// }

// void preorderTraversal(AVLNode* root){
//     if(root!=NULL){
//         writeStudentInFile(root->data);
//         preorderTraversal(root->left);
//         preorderTraversal(root->right);
//     }
// }

// void postorderTraversal(AVLNode* root){
//     if(root!=NULL){
//         postorderTraversal(root->left);
//         postorderTraversal(root->right);
//         writeStudentInFile(root->data);
//     }
// }

int main(){
    AVLNode* root=NULL;
    FILE *studentData=fopen("StudentData.txt","r");

    if(studentData==NULL){
        printf("ERROR IN OPENING FILE!!\n");
        return 1;
    }

    Student students[MAX_STUDENT_NUMBER];
    int numberOfStudents=0;

    while(fscanf(studentData,"%d,%[^,],%f",&students[numberOfStudents].rollNumber,students[numberOfStudents].name,&students[numberOfStudents].totalMarks)!=EOF){
        root=insertNode(root,students[numberOfStudents]);
        numberOfStudents++;
    }

    fclose(studentData);


    printf("The In Order Traversal of the Tree is:\n");
    
    FILE *traversal=fopen("InOrder.txt","a");

    if(traversal==NULL){
        printf("ERROR IN OPENING FILE!!\n");
        return 1;
    }

    inorderTraversal(root,traversal);

    fclose(studentData);

    

    printf("The Pre Order Traversal of the Tree is:\n");
    
    traversal=fopen("PreOrder.txt","a");

    if(traversal==NULL){
        printf("ERROR IN OPENING FILE!!\n");
        return 1;
    }

    preorderTraversal(root,traversal);

    fclose(traversal);

    

    printf("The Post Order Traversal of the Tree is:\n");
    
    traversal=fopen("PostOrder.txt","a");

    if(traversal==NULL){
        printf("ERROR IN OPENING FILE!!\n");
        return 1;
    }

    postorderTraversal(root,traversal);

    fclose(traversal);

    

    return 0;
}