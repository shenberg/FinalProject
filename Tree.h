//
// Created by Mattan on 19/12/2015.
//

#ifndef SP_EX3_TREE_H
#define SP_EX3_TREE_H

#include "API.h"
#include "SPHash.h"

#define MAX_SONS 10

typedef enum {
    INVALID, EQU, ADD, SUB, MUL, DIV, DOL, MIN, MAX, MED, AVG,
} Op;

typedef enum {
    TYPE_OP, TYPE_NUM, TYPE_VAR, TYPE_EQU,
} Node_Type;

typedef union {
    Op op;
    double num;
    char var[MAX_LINE_LEN];
} Node_Value;

typedef struct Node_t {
    Node_Type type;
    Node_Value val;
    int numOfSons;
    bool valid;
    struct Node_t* children[MAX_SONS];
} Node;


Node* stringToTree(char* line);
double calcTree(Node* tree, SPHash hash, bool* status);
void freeTree(Node* tree);


#endif //SP_EX3_TREE_H
