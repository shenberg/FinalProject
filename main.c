#include "API.h"

// decided not to implement getRootStr() and getChildAtIndex(),
// as it requires redundant dynamic memory allocation,
// while our design is based on constant strings and indices.

int main() {
    bool validResult;
    Node* tree = NULL;
    char line[MAX_LINE_LEN + 1];
    while (fgets(line, MAX_LINE_LEN, stdin) != NULL) {
        if (strcmp(line, TERMINATION_LINE) == 0) {
            printf("Exiting...\n");
            break;
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