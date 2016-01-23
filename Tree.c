//
// Created by Mattan on 19/12/2015.
//

#include "API.h"
#include "Tree.h"
#include "SPHash.h"
#include <assert.h>

#define MIN_CHAR '?'
#define MAX_CHAR '!'
#define MED_CHAR '@'
#define AVG_CHAR '%'

void freeTree(Node* tree) {
    if (tree == NULL) {
        return;
    }
    int i;
    for (i = 0; i < tree->numOfSons; i++) {
        freeTree(tree->children[i]);
    }
    free(tree);
    return;
}

Node* newNode() {
    Node* node = (Node*)malloc(sizeof(Node));
    if (node == NULL) {
        printError();
        exit(1);
    }
    node->numOfSons = 0;
    return node;
}

Node* newNumNode(const double num) {
    Node* result = newNode();
    result->type = TYPE_NUM;
    result->val.num = num;
    return result;
}

Node* newOpNode(const Op op) {
    Node* result = newNode();
    result->type = TYPE_OP;
    result->val.op = op;
    return result;
}

Node* newVarNode(const char* var) {
    Node* result = newNode();
    result->type = TYPE_VAR;
    strcpy(result->val.var, var);
    return result;
}

Node* newEquNode(const char* var) {
    Node* result = newVarNode(var);
    result->type = TYPE_EQU;
    return result;
}

//////////////// Tree construction from string ///////////////////////

Node* generalSubStringToNode(char* line, size_t start, size_t end);

Node* intSubStringToNode(char* line, size_t start, size_t end) {
    Node* result = newNumNode(atoiForSubstring(line, start + 1, end - 1));
    return result;
}

Node* varSubStringToNode(char* line, const size_t start, const size_t end) {
    char var[end - start];
    strncpy(var, line + start + 1, end - start - 1);
    var[end - start - 1] = '\0';
    //printf("var name: %s\n", var); //TODO: remove
    return newVarNode(var);
}

void attachNewChildToParentBySubstring(Node* parent, char* line, size_t start, size_t end) {
    Node* child = generalSubStringToNode(line, start, end);
    parent->children[parent->numOfSons] = child;
    parent->numOfSons++;

}


Node* opSubStringToNode(char* line, size_t start, size_t end) {
    //printf("opSubStringToNode(%p,%zu,%zu)\n", line, start, end); //TODO:remove
    Op op;
    switch(line[start + 1]) {
        case '+': op = ADD; break;
        case '-': op = SUB; break;
        case '*': op =  MUL; break;
        case '/': op = DIV; break;
        case '$': op =  DOL; break;
        case MIN_CHAR: op =  MIN; break;
        case MAX_CHAR: op =  MAX; break;
        case MED_CHAR: op =  MED; break;
        case AVG_CHAR: op =  AVG; break;
        default: printError(); exit(1);
    }
    Node* result = newOpNode(op);
    result->numOfSons = 0;
    int openedPars = 0;
    size_t subStart = 0;
    size_t subEnd;
    size_t current;
    for (current = start + 1; current <= end - 1; current++) {
        if (line[current] == '(') {
            if (openedPars == 0) {
                subStart = current;
            }
            openedPars++;
        }
        else if (line[current] == ')') {
            openedPars--;
            if (openedPars == 0) {
                subEnd = current;
                //printf("going to sub-child: %zu-%zu (%zu-%zu)\n", subStart, subEnd, start, end);//TODO:remove
                // this function is safe: if mem alloc for chils fails,
                // it frees the string and the tree from the root and then exit.
                // new nodes can only be created as children of op nodes,
                // so this safety covers all mem alloc issues.
                attachNewChildToParentBySubstring(result, line, subStart, subEnd);
            }
        }
    }
    assert(openedPars == 0);
    return result;
}



Node* generalSubStringToNode(char* line, size_t start, size_t end) {
    //printf("%zu-%zu, %d (%c), %s\n", start, end, line[end], line[end], line);//TODO:remove
    // not lisp tree?
    assert (line[0] == '(' && line[end] == ')');

    char c = line[start + 1];
    if (c >= '0' && c <= '9') {
        return intSubStringToNode(line, start, end);
    }
    else if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
        return varSubStringToNode(line, start, end);
    }
    else if (c == '=') {
        // always of the form (=<var><expression>)
        // so line[1] == '=', line[2]=='(', then scan till first ')'
        // to find var name
        size_t endOfVarPar;
        for (endOfVarPar = 2; line[endOfVarPar] != ')'; endOfVarPar++);
        char varName[MAX_LINE_LEN];
        memset(varName, '\0', sizeof(varName));
        strncpy(varName, line + 3, endOfVarPar - 3); // get the var name
        Node* result = newEquNode(varName);
        result->children[0] = generalSubStringToNode(line, endOfVarPar + 1, end-1);
        result->numOfSons = 1;
        return result;
    }
    else {
        return opSubStringToNode(line, start, end);
    }
}


Node* stringToTree(char* line) {
    Node* result = generalSubStringToNode(line, 0, strlen(line) - 1);
    return result;
}

double calcTree(Node* tree, SPHash hash, bool* status) {
    enum HashResult_t error;
    double result = 0;
    double *hashValue;
    int i;
    Op op;
    switch (tree->type) {
    case TYPE_NUM:
        *status = true;
        return tree->val.num;
    case TYPE_VAR:
        //printf("getting hash for var %s\n", t) //TODO:remove
        hashValue = hashGetValue(hash, tree->val.var, &error);
        if (error == SP_HASH_OUT_OF_MEMORY) {
            printError();
            exit(1);
        } else if (hashValue == NULL) {
            // variable doesn't exist
            *status = false;
            return 0;
        }
        result = *hashValue;
        free(hashValue);
        *status = true;
        return result;
    case TYPE_OP:
        op = tree->val.op;
        assert(tree->numOfSons > 0);
        if (tree->numOfSons == 1) { // unary minus/plus
            if (op == SUB) {
                result = -calcTree(tree->children[0], hash, status);
            } else { // add, or single-param min/max/avg/med
                result = calcTree(tree->children[0], hash, status);
            } 
        }
        else if (op == MED) {
            double* doublesForMedian = (double*)malloc(tree->numOfSons * sizeof(double));
            if (doublesForMedian == NULL) {
                printError();
                exit(1);
            }
            for (i = 0; i < tree->numOfSons; i++) {
                doublesForMedian[i] = calcTree(tree->children[i], hash, status);
                if (!*status) {
                    break;
                }
            }
            if (*status) {
                qsort(doublesForMedian, (size_t)(tree->numOfSons), sizeof(double), compare);
                if (tree->numOfSons % 2 == 1) { // odd number of sons, greather than 0
                    result = doublesForMedian[(tree->numOfSons - 1) / 2];
                }
                else { // even number of sons, greater than 0
                    result = (doublesForMedian[(tree->numOfSons / 2)] + doublesForMedian[(tree->numOfSons / 2) - 1]) / 2;
                }
            }
            free(doublesForMedian);
        }
        else if (op == AVG) {
            result = 0;
            *status = true;
            for (i = 0; i < tree->numOfSons && *status; i++) {
                result += calcTree(tree->children[i], hash, status);
            }
            if (*status) {
                result /= tree->numOfSons;
            }
        }
        else {
            result = calcTree(tree->children[0], hash, status); // assuming operator has at least one operand!

            for (i = 1; i < tree->numOfSons && *status; i++) {
                double parameter = calcTree(tree->children[i], hash, status);
                if (!*status) {
                    break;
                }
                result = generalBinaryCalc(result, parameter, op, status);
            }
        }
        return (result != 0 ? result : 0);
        // result may be erroneous. That's why we carry the status flag
        // -0 turns to +0
    case TYPE_EQU:
        assert(tree->numOfSons == 1);
        return calcTree(tree->children[0], hash, status);
    default:
        assert(0);
    }
}

