//
// Created by Mattan on 19/12/2015.
//

#include "API.h"


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

Node* newIntNode(const int num) {
    Node* result = newNode();
    result->isNum = true;
    result->val.num = num;
    return result;
}

Node* newOpNode(const Op op) {
    Node* result = newNode();
    result->isNum = false;
    result->val.op = op;
    return result;
}

Node* intSubStringToNode(char* line, size_t start, size_t end) {
    Node* result = newIntNode(atoiForSubstring(line, start + 1, end - 1));
    return result;
}

Node* varSubStringToNode(char* line, const size_t start, const size_t end) {
    char var[end - start];
    strncpy(var, line + start + 1, end - start - 1);
    var[end - start - 1] = '\0';
    if (hashIsEmpty() || !hashContains(var)) {
        printError();
        exit(1);
    }
    else {
        char* varValueString = hashGetValue(var);
        int varValueInt = atoi(varValueString);
        free(varValueString);
        Node *result = newIntNode(varValueInt);
        return result;
    }
}

Node* opSubStringToNode(char* line, size_t start, size_t end) {
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
                // this function is safe: if mem alloc for chils fails,
                // it frees the string and the tree from the root and then exit.
                // new nodes can only be created as children of op nodes,
                // so this safety covers all mem alloc issues.
                attachNewChildToParentBySubstring(result, line, subStart, subEnd);
            }
        }
    }
    return result;
}



Node* generalSubStringToNode(char* line, size_t start, size_t end) {
    char c = line[start + 1];
    if (c >= '0' && c <= '9') {
        return intSubStringToNode(line, start, end);
    }
    else if (c >= 'A' && c <= 'z') {
        return varSubStringToNode(line, start, end);
    }
    else {
        return opSubStringToNode(line, start, end);
    }
}

void attachNewChildToParentBySubstring(Node* parent, char* line, size_t start, size_t end) {
    Node* child = generalSubStringToNode(line, start, end);
    parent->children[parent->numOfSons] = child;
    parent->numOfSons++;

}

Node* stringToTree(char* line) {
    char newline[MAX_LINE_LEN];
    size_t i, j;
    size_t original_len = strlen(line) + 1; // length including the '\0'
    for (i = 0, j = 0; i < original_len; i++, j++) {
        if ((original_len - i) >= 3 && strncmp(line + i, "min", 3)) {
            newline[j] = MIN_CHAR;
            i += 2;
        }
        else if ((original_len - i) >= 3 && strncmp(line + i, "max", 3)) {
            newline[j] = MAX_CHAR;
            i += 2;
        }
        else if ((original_len - i) >= 6 && strncmp(line + i, "median", 6)) {
            newline[j] = MED_CHAR;
            i += 5;
        }
        else if ((original_len - i) >= 7 && strncmp(line + i, "average", 7)) {
            newline[j] = AVG_CHAR;
            i += 6;
        }
        else {
            newline[j] = line[i];
        }
    }
    Node* result = generalSubStringToNode(newline, 0, strlen(line) - 1);
    return result;
}

double calcTree(Node* tree, bool* status) {
    if (tree->isNum) {
        return tree->val.num;
    }
    else { // node is op
        double result;
        int i;
        Op op = tree->val.op;
        if (op == SUB && tree->numOfSons == 1) { // unary minus ?
            result = -calcTree(tree->children[0], status);
        }
        else {
            result = calcTree(tree->children[0], status); // assuming operator has at least one operand!
            for (i = 1; i < tree->numOfSons; i++) {
                result = generalBinaryCalc(result, calcTree(tree->children[i], status), op, status);
            }
        }
        return (result != 0 ? result : 0);
        // result may be erroneous. That's why we carry the status flag
        // -0 turns to +0
    }
}

