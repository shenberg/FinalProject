//
// Created by Mattan on 19/12/2015.
//

#ifndef SP_EX3_FUNCS_H
#define SP_EX3_FUNCS_H

#include "Tree.h"

void printError();
int compare (const void* a, const void* b);
int atoiForSubstring(const char string[], size_t start, size_t end);
double generalBinaryCalc(double a, double b, Op op, bool* validResult);

void printTreeExpression(Node *node, FILE *output);
bool argsHandler(int argc, char** argv, SPHash table, HashResult* msg, FILE** output);
void printHashMsgToOutput(HashResult msg, FILE* output);

#endif //SP_EX3_FUNCS_H
