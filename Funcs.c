//
// Created by Mattan on 19/12/2015.
//

#include "API.h"

void printError() {
    printf("Unexpected error occoured!\n");
}

double generalBinaryCalc(double a, double b, Op op, bool* validResult) {
    switch (op) {
        case ADD: return a + b;
        case SUB: return a - b;
        case MUL: return a * b;
        case DIV:{
            if (b != 0) {
                return a / b;
            }
            else {
                *validResult = false;
                break;
            }
        }
        case DOL:{
            if (a <= b) {
                // by series sum formula
                return ((b - a + 1) / 2) * (a + b);
            }
            else {
                *validResult = false;
                break;
            }
        }
        case MIN: return (a < b ? a : b);
        case MAX: return (a < b ? b : a);
        case INVALID:;
    }
    return NaN;
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