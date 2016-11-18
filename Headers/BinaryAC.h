#ifndef BINARYAC_H_INCLUDED
#define BINARYAC_H_INCLUDED



#endif // BINARYAC_H_INCLUDED




#include <iostream>
#include <string.h>
#include <malloc.h>
#include <queue>

extern const char pattern[patternnum][30];

void startmatch(char *buff, int len, int successnum[]);
bool judge(int successnum[]);
