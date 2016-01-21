//
// Created by Mattan on 19/12/2015.
//

#include "API.h"

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


bool argsHandler(int argc, char** argv, SPHash* table, HashResult* msg, FILE** output){
    char varFile[MAX_LINE_LEN] = "";
    char outFile[MAX_LINE_LEN] = "";

    if (!argsSanityCheck(argc, argv, varFile, outFile)
        || !assignOutput(out_fname, output)
        || !insertVarsFromFileToHashTable(var_fname, table, msg)
        || *msg != SP_HASH_OK) {
        return false;
    }
    return true;
}

bool argsSanityCheck(int argc, char** argv, char* var_file, char* out_file) {
    if (argc == 1) { return true; } // no args
    else {
        int i;
        if (argc != 3 && argc != 5) {
            printf("Invalid command line arguments, use [-v filename1] [-o filename2])\n");
            return false;
        }
        for (i = 1; i < argc; i += 2) {
            if (!strcmp(argv[i], "-v") && !strcmp(var_file, "")) {
                strcpy(var_file, argv[i + 1]);
            }
            else if (!strcmp(argv[i], "-o") && !strcmp(out_file, "")) {
                strcpy(out_file, argv[i + 1]);
            }
        }
        if (var_file && out_file && !strcmp(var_file, out_file)) {
            printf("Files must be different\n");
            return false;
        }
        return true;
    }
}

bool assignOutput(char* fname, FILE** output) {
    if(!strcmp(fname, "")) { return true; }
    else {
        if (!(*output = fopen(fname, "w"))) {
            printf("Output file is read-only or cannot be created\n");
            return false;
        }
    }
    return true;
}


bool insertVarsFromFileToHashTable(char* fname, SPHash* hashTable, HashResult* msg) {
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
        currentVar = strtok(line, DELS2);
        currentVal = strtok(NULL, DELS2);
        if (!hashInsert(hashTable, var, atof(val), msg)) {
            return false;
        }
    }
    fclose(varFile);
    return true;
}


void printError() {
    printf("Unexpected error occoured!\n");
}

bool floatIsInt(double a) {
    return a == ceil(a) || a == floor(a);
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