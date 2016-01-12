//
// Created by Mattan on 19/12/2015.
//

#include "API.h"

void printError() {
    printf("Unexpected error occoured!\n");
}

bool floatIsInt(double a) {
    return (a == ceil(a) || a == floor(a)) ? true : false;
}

int compare (const void* a, const void* b) {
    float delta = *(float*)a - *(float*)b;
    if (delta > 0) { return 1; }
    else if (delta == 0) { return 0; }
    else { return -1; }
}

double generalBinaryCalc(double a, double b, Op op, bool* validResult) {
    switch (op) {
        case ADD:
            return a + b;
        case SUB:
            return a - b;
        case MUL:
            return a * b;
        case DIV: {
            if (b != 0) {
                return a / b;
            }
            else {
                *validResult = false;
                break;
            }
        }
        case DOL: {
            if (a <= b && double(a) && double(b)) {
                // by series sum formula
                return ((b - a + 1) / 2) * (a + b);
            }
            else {
                *validResult = false;
                break;
            }
        }
        case MIN:
            return (a < b ? a : b);
        case MAX:
            return (a < b ? b : a);
        case MED:; // should never reach here, not binary op, handled in calcTree()
        case AVG:; // should never reach here, not binary op, handled in calcTree()
        case INVALID:; // should only reach here for unrecognized variable
        default: *validResult = false;
    }
    return NaN;
}

char opEnumToChar(Op op):{
    switch (op) {
        case ADD: return '+';
        case SUB: return '-';
        case MUL: return '*';
        case DIV: return '/';
        case DOL: return '$';
        default:;
    }
    return NULL;
}

int atoiForSubstring(const char string[], size_t start, size_t end) {
    const size_t len = end - start + 1;
    char* subString = (char*)malloc((len + 1)*sizeof(char));
    if (subString == NULL) {
        printError();
        exit(1);
    }
    strncpy(subString, string + start, len);
    subString[len] = '\0';
    int result = atoi(subString);
    free(subString);
    return result;
}

void addParenthesisBinaryOpAndExpressionToOriginalExpression(char* src, Op op, char* exp) {
    char buffer[MAX_LINE_LEN];
    buffer[0] = '(';
    buffer[1] = '\0';
    strcat(exp, buffer);
    //buffer[0] = '(';
    //buffer[1] = '\0';
    char opString[2];
    opString[0] = opEnumToChar(op);
    opString[1] = '\0';
    strcat(op, buffer);
    strcar(exp, buffer);
    buffer[strlen(buffer)] = ')';
    buffer[strlen(buffer)] = '\0';
    strcpy(buffer, src);
}