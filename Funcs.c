//
// Created by Mattan on 19/12/2015.
//

#include "API.h"

void printError() {
    printf("Unexpected error occoured!\n");
}

int floatIsInt(float a) {
    return (a == ceil(a) || a == floor(a));
}

int compare (const void* a, const void* b) {
    float delta = *(float*)a - *(float*)b;
    if (delta > 0) { return 1; }
    else if (delta == 0) { return 0; }
    else { return -1; }
}

float generalBinaryCalc(float a, float b, Op op, bool* validResult) {
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
            if (a <= b && floatIsInt(a) && floatIsInt(b)) {
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