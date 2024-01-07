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

AVLNode* insertNode(AVLNode* root, Student data) {
    if (root == NULL) {
        printf("\tCreating a new node with roll number %d\n", data.rollNumber);
        return createNode(data);
    }

    printf("\tComparing roll number %d with the current node's roll number %d\n", data.rollNumber, root->data.rollNumber);

    if (data.rollNumber < root->data.rollNumber) {
        printf("\tGoing to the left subtree\n");
        root->left = insertNode(root->left, data);
    } else if (data.rollNumber > root->data.rollNumber) {
        printf("\tGoing to the right subtree\n");
        root->right = insertNode(root->right, data);
    } else {
        printf("\tNode with roll number %d already exists, not inserting duplicate\n", data.rollNumber);
        return root; // Node with the same roll number already exists
    }

    printf("\tUpdating height of the node with roll %d\n",root->data.rollNumber);
    root->height = 1 + maxOf(getHeight(root->left), getHeight(root->right));

    int balance = getBalance(root);

    if (balance > 1 && data.rollNumber < root->left->data.rollNumber) {
        printf("\tPerforming right rotation\n");
        return rightRotate(root);
    }
    if (balance < -1 && data.rollNumber > root->right->data.rollNumber) {
        printf("\tPerforming left rotation\n");
        return leftRotate(root);
    }
    if (balance > 1 && data.rollNumber > root->left->data.rollNumber) {
        printf("\tPerforming left rotation on left child and then right rotation on current node\n");
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }
    if (balance < -1 && data.rollNumber < root->right->data.rollNumber) {
        printf("\tPerforming right rotation on right child and then left rotation on current node\n");
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }
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

void traverseIntoFile(AVLNode* root,char* traversalType,char* fileName){
    fclose(fopen(fileName,"w")); //To clear anything in the file before opening it in append mode, so that no recurring data exists 
    FILE *traversal=fopen(fileName,"a");

    if(traversal==NULL){
        printf("ERROR IN OPENING FILE!!\n");
        return;
    }

    if(!strcmp("InOrder",traversalType)){
        inorderTraversal(root,traversal);
    }else if(!strcmp("PreOrder",traversalType)){
        preorderTraversal(root,traversal);
    }else if(!strcmp("PostOrder",traversalType)){
        postorderTraversal(root,traversal);
    }

    fclose(traversal);
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
        printf("Inserting Node with roll number %d ...\n", students[numberOfStudents].rollNumber);
        root=insertNode(root,students[numberOfStudents]);
        printf("Node with roll number %d inserted successfully.\n\n", students[numberOfStudents].rollNumber);
        numberOfStudents++;
    }

    fclose(studentData);

    // printf("The In Order Traversal of the Tree is:\n");
    // inorderTraversal(root);

    // printf("The Pre Order Traversal of the Tree is:\n");
    // preorderTraversal(root);
 
    // printf("The Post Order Traversal of the Tree is:\n");
    // postorderTraversal(root);

    traverseIntoFile(root,"InOrder","InOrder.txt");
    traverseIntoFile(root,"PreOrder","PreOrder.txt");
    traverseIntoFile(root,"PostOrder","PostOrder.txt");

    return 0;
}