//
// Created by Mattan on 19/12/2015.
//

#include "API.h"
#include <assert.h>

#define DELS " \t\r\n="

// convert function operators to function names
// return NULL for regular operators;
const char *opToFunc(Op op) {
    switch (op) {
    case MIN: return "min";
    case MAX: return "max";
    case MED: return "median";
    case AVG: return "average";
    default: return NULL;
    }
}

// regular operator -> type to string
const char *opToStr(Op op) {
    switch(op) {
    case ADD: return "+";
    case SUB: return "-";
    case MUL: return "*";
    case DIV: return "/";
    case DOL: return "$";
    default: return NULL;
    }
}

void printInnerExpression(Node *node, FILE *output) {
    if (node->type == TYPE_NUM) {
        // print as integer since expression tree only has integers:
        // only results and variables can be doubles
        fprintf(output, "%.0f", node->val.num);
    } else if (node->type == TYPE_VAR) {
        fprintf(output, "%s", node->val.var);
    } else {
        printTreeExpression(node, output);
    }    
}

void printTreeExpression(Node *node, FILE *output) {
    const char *funcName;
    fprintf(output, "(");
    switch(node->type) {
    case TYPE_NUM:
        fprintf(output, "%.0f", node->val.num);
        break;
    case TYPE_VAR:
        fprintf(output, "%s", node->val.var);
        break;
    case TYPE_OP:
        funcName = opToFunc(node->val.op);
        if (funcName != NULL) {
            // handle function calls
            fprintf(output, "%s(", funcName);
            for (int i = 0; i < node->numOfSons; i++) {
                if (i > 0) {
                    fprintf(output, ",");
                }
                Node *child = node->children[i];
                printInnerExpression(child, output);
            }
            fprintf(output, ")");
        } else if (node->numOfSons == 1) {
            // unary op
            switch(node->val.op) {
                case ADD:
                    fprintf(output,"+");
                    break;
                case SUB:
                    fprintf(output,"-");
                    break;
                default:
                    // only +/- can be unary!
                    assert(0);
            }
            printInnerExpression(node->children[0], output);
        } else {
            // binary op 
            assert(node->numOfSons == 2);
            printInnerExpression(node->children[0], output);
            const char *opStr = opToStr(node->val.op);
            assert(opStr != NULL);
            fprintf(output, "%s", opStr);
            printInnerExpression(node->children[1], output);
        }
        break;
    case TYPE_EQU:
        assert(node->numOfSons == 1);
        fprintf(output, "%s=", node->val.var);
        printInnerExpression(node->children[0], output);
    }
    fprintf(output, ")");
}

void printHashMsgToOutput(HashResult msg, FILE* output) {
    switch(msg) {
        case SP_HASH_OUT_OF_MEMORY:
            fprintf(output, "Unexpected error occurred!\n");
            break;
        case SP_HASH_NULL_ARGUMENT:
            fprintf(output, "Invalid Result\n");
            break;
        default:
            break;
    }
}


bool argsSanityCheck(int argc, char** argv, char* varFile, char* outFile) {
    if (argc == 1) { return true; } // no args
    else {
        int i;
        if (argc != 3 && argc != 5) {
            printf("Invalid command line arguments, use [-v filename1] [-o filename2])\n");
            return false;
        }
        for (i = 1; i < argc; i += 2) {
            if (!strcmp(argv[i], "-v") && !strcmp(varFile, "")) {
                strcpy(varFile, argv[i + 1]);
            }
            else if (!strcmp(argv[i], "-o") && !strcmp(outFile, "")) {
                strcpy(outFile, argv[i + 1]);
            }
        }
        if (varFile && outFile && !strcmp(varFile, outFile)) {
            printf("Files must be different\n");
            return false;
        }
        return true;
    }
}

bool assignOutput(char* fname, FILE** output) {
    if(!strcmp(fname, "")) { return true; }
    else {
        FILE *new_output = fopen(fname, "w");
        if (new_output == NULL) {
            printf("Output file is read-only or cannot be created\n");
            return false;
        }
        *output = new_output;
    }
    return true;
}


bool insertVarsFromFileToHashTable(char* fname, SPHash hashTable, HashResult* msg) {
    char line[MAX_LINE_LEN];
    char* currentVar;
    char* currentVal;
    FILE* varFile;
    if (!strcmp(fname,"")) { return true; } // no file, no need to insert vars
    if (!(varFile = fopen(fname, "r"))) {
        printf("Variable init file doesn’t exist or is not readable\n");
        return false;
    }
    while (fgets(line, sizeof(line), varFile)) {
        currentVar = strtok(line, DELS);
        currentVal = strtok(NULL, DELS);
        if (!hashInsert(hashTable, currentVar, atof(currentVal), msg)) {
            printError();
            return false;
        }
    }
    fclose(varFile);
    return true;
}


bool argsHandler(int argc, char** argv, SPHash table, HashResult* msg, FILE** output){
    char varFile[MAX_LINE_LEN] = "";
    char outFile[MAX_LINE_LEN] = "";

    if (!argsSanityCheck(argc, argv, varFile, outFile)
        || !assignOutput(outFile, output)
        || !insertVarsFromFileToHashTable(varFile, table, msg)
        || *msg != SP_HASH_OK) {
        return false;
    }
    return true;
}



void printError() {
    printf("Unexpected error occured!\n");
    assert(0);
}

bool doubleIsInt(double a) {
    return a == ceil(a) || a == floor(a);
}

int compare (const void* a, const void* b) {
    double delta = *(double*)a - *(double*)b;
    if (delta > 0) { return 1; }
    else if (delta == 0) { return 0; }
    else { return -1; }
}

double generalBinaryCalc(double a, double b, Op op, bool* validResult) {
    *validResult = true;
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
            if (a <= b && doubleIsInt(a) && doubleIsInt(b)) {
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
        case MED: // should never reach here, not binary op, handled in calcTree()
        case AVG: // should never reach here, not binary op, handled in calcTree()
        case INVALID: // should only reach here for unrecognized variable
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
