//
// Created by Mattan on 19/12/2015.
//

#ifndef SP_EX3_FUNCS_H
#define SP_EX3_FUNCS_H

#include "Tree.h"

void printError();
int compare (const void* a, const void* b);
int atoiForSubstring(const char string[], size_t start, size_t end);
int doubleIsInt(double a);
double generalBinaryCalc(float a, float b, Op op, bool* validResult);


#endif //SP_EX3_FUNCS_H
