#include "API.h"
#include "SPHash.h"

#define TERMINATION_LINE "(<>)\n"

// decided not to implement getRootStr() and getChildAtIndex(),
// as it requires redundant dynamic memory allocation,
// while our design is based on constant strings and indices.

int main(int argc, char* argv[]) {
    bool validResult;
    SPHash hashTable = hashCreate();
    HashResult hashMsg;
    FILE* output = stdout;
    if(hashTable == NULL) {
        printError();
        exit(1);
    }
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
        if (!strcmp(line, TERMINATION_LINE)) {
            if (output != stdout) { fprintf(output, "%s\n", TERMINATION_LINE); }
            printf("Exiting...\n");
            break;
        }
        validResult = true;
        Node *tree = stringToTree(line);
        if (NULL == tree) {
            printError();
            exit(1);
        }
        if (output != stdout || true) { //TODO!
            printTreeExpression(tree, output);
            fprintf(output,"\n");
        }
        res = calcTree(tree, hashTable, &validResult);
        if (tree->type == TYPE_EQU) { // var assignment
            if (validResult) {
                fprintf(output, "%s = %.2f\n", tree->val.var, res);
                // successful assignment, store result
                if (!hashInsert(hashTable, tree->val.var, res, NULL)) {
                    // hashTable != NULL so we're out of memory on failure
                    printError();
                    exit(1);
                }
            } else {
                fprintf(output, "Invalid Assignment\n");
            }
        }
        else { // not var assignment
            if (validResult) {
                fprintf(output, "res = %.2f\n", res);
            } else {
                fprintf(output, "Invalid Result\n");
            }
        }
        printf("adsf");
        fflush(stdout);
        freeTree(tree);
    }
    if (output != stdout) {
        fclose(output);
    }
    hashDestroy(hashTable);
    // exit() clears all dynamic memory so no need to free(),
    // as told by Moab
    exit(EXIT_SUCCESS);
}
