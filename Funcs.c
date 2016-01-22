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
    case MIN:
        return "min";
    case MAX:
        return "max";
    case MED:
        return "median";
    case AVG:
        return "average";
    default:
        return NULL;
    }
}

// regular operator -> type to string
const char *opToStr(Op op) {
    switch(op) {
    case ADD:
        return "+";
    case SUB:
        return "-";
    case MUL:
        return "*";
    case DIV:
        return "/";
    case DOL:
        return "$";
    default:
        return NULL;
    }
}
void printInnerExpression(Node *node, FILE *output) {
    if (node->type == TYPE_NUM) {
        fprintf(output, "%.2f", node->val.num);
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
        fprintf(output, "%.2f", node->val.num);
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
/*
int printExpressionToOutput(Node* root, FILE* output){
    char stringToPrint[MAX_LINE_LEN * 2 + 1];
    strcpy(outputString, "");
    printExpressionToOutputRec(root, outputString, valid);
    if (!valid) { return -1; }
    return fprintf(output, "%s\n", outputString);
}

void printExpressionToOutputRec(Node* root, char* output) {
    size_t i;
    char temp[MAX_LINE_LEN + 1];

    // if there was an error, do nothing
    if (!valid || !root)
        return;

    if (root == NULL)
        return;

    // NUMBER / VAR

    if (root->Node_Type == TYPE_NUM) {
        if (sprintf(temp, "(%.2f)", root->val.num) < 0) {
            *valid = false;
            return;
        }
        strcat(output, temp);
        return;

    }

    else if (root->Node_Type == VAR) {
        if (sprintf(temp, "(%s)", root->val.var) < 0) {
            *valid = false;
            return;
        }
        strcat(output, temp);
        return;
    }

    // ASSIGNMENT
    else if (getNodeType(root) == EQUAL) {
        if (sprintf(temp, "(%s=", getNodeValue(getChildAt(root, 0))) < 0) {
            *msg = SP_TREE_ERROR_PRINT_FAILED;
            return;
        }
        strcat(output, temp);
        if ((getNodeType(getChildAt(root, 1)) == NUMBER) || (getNodeType(getChildAt(root, 1)) == VARIABLE))
            strcat(output, getNodeValue(getChildAt(root, 1)));
        else
            printTreeExpressionRec(getChildAt(root, 1), output, msg);
        strcat(output, ")");
        return;
    }

    // FUNCTIONS
    if ((getNodeType(root) == MIN) || (getNodeType(root) == MAX)\
    || (getNodeType(root) == AVERAGE) || (getNodeType(root) == MEDIAN)) {
        if (sprintf(temp, "(%s(", getNodeValue(root)) < 0) {
            *msg = SP_TREE_ERROR_PRINT_FAILED;
            return;
        }
        strcat(output, temp);
        for (i = 0; i < getNodeChildrenSize(root); ++i) {
            if ((getNodeType(getChildAt(root,i)) == NUMBER) || (getNodeType(getChildAt(root,i)) == VARIABLE))
                strcat(output, getNodeValue(getChildAt(root, i)));
            else
                printTreeExpressionRec(getChildAt(root,i), output, msg);
            if (i < (getNodeChildrenSize(root) - 1))
                strcat(output, ",");
        }
        strcat(output, "))");
        return;
    }

    // BINARY
    if (getNodeChildrenSize(root) == 2) {
        strcat(output, "(");
        for (i = 0; i < 2; ++i ) {
            if ((getNodeType(getChildAt(root, i)) == NUMBER) || (getNodeType(getChildAt(root, i)) == VARIABLE))
                strcat(output, getNodeValue(getChildAt(root, i)));
            else
                printTreeExpressionRec(getChildAt(root,i), output, msg);
            if (i < 1)
                strcat(output, getNodeValue(root));
        }
        strcat(output, ")");
        return;
    }

    // UNARY
    if ((getNodeChildrenSize(root) == 1) && ((getNodeType(root) == PLUS) || (getNodeType(root) == MINUS))) {
        if (sprintf(temp, "(%s", getNodeValue(root)) < 0){
            *msg = SP_TREE_ERROR_PRINT_FAILED;
            return;
        }
        strcat(output, temp);
        if ((getNodeType(getChildAt(root,0)) == NUMBER) || (getNodeType(getChildAt(root, 0)) == VARIABLE))
            strcat(output, getNodeValue(getChildAt(root, 0)));
        else
            printTreeExpressionRec(getChildAt(root, 0), output, msg);
        strcat(output, ")");
        return;
    }
}

*/
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
        if (!(*output = fopen(fname, "w"))) {
            printf("Output file is read-only or cannot be created\n");
            return false;
        }
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
        case MED: // should never reach here, not binary op, handled in calcTree()
        case AVG: // should never reach here, not binary op, handled in calcTree()
        case INVALID: // should only reach here for unrecognized variable
        default: *validResult = false;
    }
    return NaN;
}
/*
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
}*/

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
/*
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
    strcat(exp, buffer);
    buffer[strlen(buffer)] = ')';
    buffer[strlen(buffer)] = '\0';
    strcpy(buffer, src);
}
*/
