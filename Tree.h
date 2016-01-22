//
// Created by Mattan on 19/12/2015.
//

#ifndef SP_EX3_TREE_H
#define SP_EX3_TREE_H

#define MAX_SONS 10

typedef enum {
    INVALID, EQU, ADD, SUB, MUL, DIV, DOL, MIN, MAX, MED, AVG,
} Op;

typedef enum {
    OP, NUM, VAR, EQU,
} Node_Type;

typedef union {
    Op op;
    double num;
    char var[MAX_LINE_LEN];
} Node_Value;

typedef struct Node_t {
    //Node_Value val
    Node_Type;
    Node_Value val;
    int numOfSons;
    struct Node_t* children[MAX_SONS];
} Node;


Node* newNode();
Node* newIntNode(const int num);
Node* newOpNode(const Op op);
Node* intSubStringToNode(char* line, size_t start, size_t end);
Node* varSubStringToNode(char* line, const size_t start, const size_t end);
Node* opSubStringToNode(char* line, size_t start, size_t end);
Node* generalSubStringToNode(char* line, size_t start, size_t end);
Node* stringToTree(char* line);
double calcTree(Node* tree, bool* status);
void freeTree(Node* tree);
void attachNewChildToParentBySubstring(Node* parent, char* line, size_t start, size_t end);


#endif //SP_EX3_TREE_H
