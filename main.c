#include "API.h"

// decided not to implement getRootStr() and getChildAtIndex(),
// as it requires redundant dynamic memory allocation,
// while our design is based on constant strings and indices.

int main(int argc, char* argv[]) {
    bool validResult;
    SPHash hashTable = hashCreate();
    FILE* output = stdout;
    char* exp;
    if(hashTable == NULL) {
        printError();
        exit(1);
    }
    Node* tree = NULL;
    double res;
    char line[MAX_LINE_LEN + 1];
    if (!argsHandler(argc, argv, hashTable, &hashMsg, &output)){
        printHashMsgToOutput(hashMsg, output);
        // hashDestroy(hashTable);
        // no need to free hash because exit() clears dynamic memory
        if (output && output != stdout) { // output is file
            fclose(output);
        }
        exit(EXIT_FAILURE);
    }

    while (fgets(line, MAX_LINE_LEN, stdin) != NULL) {
        exp = strtok(line, DELS);
        if (expression == NULL) {
            fprintf(output, "Invalid Result\n");
            continue;
        }
        if (!strcmp(line, TERMINATION_LINE)) {
            if (output != stdout) { fprintf(output, "%s\n", TERMINATION_LINE); }
            printf("Exiting...\n");
            break;
        }
        validResult = true;
        //Node* tree = newNode();
        if (!tree) {
            fprintf(output, "Unexpected error occurred!\n");
            break;
        }
        tree = stringToTree(line);
        if (tree->Node_Type == VAR) { // var assignment
            res = calcTree(tree->children[0], &validResult);
            if (validResult) {
                printf("%s = %.2f\n", tree->val.var, res);
            }
        }
        else { // not var assignment
            res = calcTree(tree, &validResult);
            if (validResult) {
                printf("%s = %.2f\n", tree->val.var, res);
            }
        }
        freeTree(tree);
        if (!validResult) {
            printf("Invalid Result\n");
        }
    }
    // exit() clears all dynamic memory so no need to free(),
    // as told by Moab
    exit(EXIT_SUCCESS);
}
