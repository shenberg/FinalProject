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
    char line[MAX_LINE_LEN + 1];
    if (!handleArguments(argc, argv, hashTable, &hashMsg, &output)){
        printHashMsg(hashMsg, output);
        hashTableDestroy(hashTable);
        if ((output != NULL) && (output != stdout))
            fclose(output);
        return EXIT_FAILURE;
    }

    while (fgets(line, MAX_LINE_LEN, stdin) != NULL) {
        if (strcmp(line, TERMINATION_LINE) == 0) {
            printf("Exiting...\n");
            break;
        }
        exp = strtok(line, DELS);
        if (expression == NULL) {
            fprintf(output, INVALID_RESULT_MSG);
            continue;
        }
        validResult = true;
        tree = stringToTree(line);
        double res = calcTree(tree, &validResult);
        freeTree(tree);
        if (validResult) {
            printf("res = %.2f\n", res);
        }
        else {
            printf("Invalid Result\n");
        }
    }
    // exit() clears all dynamic memory so no need to free(),
    // as told by Moab
    exit(0);
}
