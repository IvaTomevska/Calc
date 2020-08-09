#ifndef MYSEARCH_H
#define MYSEARCH_H

#include<iostream>
#include<string>
#include<cmath>
#include<stack>
#include<ctype.h>
#include<vector>
#include<fstream>
#include<map>
#include<limits>

using namespace std;

bool isoperator(char); 
int convertOpToInt (char); 
bool isleq(char, char);
string infix2postfix(string);  
float evaluate(string, map <string,float> &);
bool isnumber(string);
vector <string> divideinput(string);

#endif