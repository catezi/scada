#ifndef TRANSFORM_H_INCLUDED
#define TRANSFORM_H_INCLUDED



#endif // TRANSFORM_H_INCLUDED


#include <iostream>
#include <string>
#include <string.h>
#include <cstdio>
#include <sstream>

using namespace std;

string string2hex(string const &s);
int hexchar2int(char ch1, char ch2);
void transformpattern(char origin[][10], char transformed[][10], int length);
