#include<iostream>
#include<string>
#include<cmath>
#include<stack>
#include<ctype.h>
#include<vector>
#include<fstream>
#include<map>
#include<limits>

#include "mycalc.h"

using namespace std;

//devides the equations ftom the file into a variable and an expression
vector <string> divideinput(string inputline)
{
	vector <string> result;
	string var=inputline.substr(0,inputline.find('=')-1);//-1 to remove the empy space
	int pos=inputline.find(';');
	int leng= pos-inputline.find('=')-2; //length of the substing without ''
	string equn=inputline.substr(inputline.find('=')+2,leng); //creates the equation

	//removes ampry space (if there is) from the end of the expression
	while(equn.back()==' ')
		equn.pop_back();

	result.push_back(var);
	result.push_back(equn);

	return result;
}


//Checks if a character is operator
bool isoperator(char ch)
{
	if( ch=='+' || ch=='-' || ch=='*' || ch=='/' || ch=='^' || ch=='%' || ch=='$')
		return true;
	else
		return false;

}
//Helper mehtod which converts operators into int so their precdence can be compared
int convertOpToInt(char ch) 
{
    if (ch=='+' || ch=='-') return 1;
    if (ch=='*' || ch=='/' || ch=='%' || ch=='$') return 2;
    if (ch=='^') return 3;
    return 0;
}

//Helper method which compare two operators and return True if first operator
//has less or equal predence than the right operator
bool isleq(char opA, char opB)
{
	return (convertOpToInt(opA)<=convertOpToInt(opB));
}

//checks if a string is a number
bool isnumber(string check)
{
	if (isdigit(check[0]))
		return true;
	else if ((check[0])=='-' && isdigit(check[1]))
		return true;
	else
		return false;
}

//chacks if a string is a variable
bool isvar(string check)
{
	if (isalpha(check[0]))
		return true;
	else
		return false;
}

// Helper Method which converts an Infix Notaiton to a postfix notation
string infix2postfix(string x)
{
	stack <char> mystack;
	string y="";

	//convert UNARY TO BINARY operand
	for (int i=0; i<x.length(); i++)
		{
		// 1st -
			if (x[0]=='-'&& x[1]!='-')
			{
				x.insert(1,"(n1)*");
				x.erase(0,1);
			}
		//-(
			if (x[i]=='-' && x[i-1]=='(')
			{
				x.insert(i+1,"(n1)*"); //n will normally not be exactly before 1 == for making negative numbers
				x.erase(i,1);
			}	
			if (x[i]  == '-' && x[i+1] == '-')  // find 2 * after each other
			{
				x.insert(i+2,"n1+");
				x.erase(i,2);
			}
		// **
			if (x[i]  == '*' && x[i+1] == '*')  // find 2 * after each other
			{
				if (x[i+2]== '(') // if there is a bracket behind ** isert ^2 after the closing bracket
				{
					int j=x.find(')',i+3);
					x.insert(j+1,"^2");
					x.erase(i,2);
				}
				else 				// insert ^2 after the end of the number/var
				{	
					int j=i+2;
					while (isalnum(x[j])== 1)
						j++;
					x.insert(j+1,"^2");
					x.erase(i,2);
				}
			}
		// ++
			if (x[i]  == '+' && x[i+1] == '+')  // find 2 * after each other
			{

				x.insert(i+2,"1+");
				x.erase(i,2);
			}

			if (x[i]=='m' && x[i+1]=='o' && x[i+2]=='d')
			{
				x.insert(i,"$");
				x.erase(i+1,3);
			}
		}
	//Push “(“onto Stack, and add “)” to the end of X.
	x=x+')';
	mystack.push('(');

	//Scan X from left to right and repeat Step 3 to 6 for each element of X 
	//until the Stack is empty.3.	If an operand 

	int i=0;
	while(!mystack.empty())
	{	
		char ch = x[i++];
		//If an operand is encountered, add it to Y.
		if (isalnum(ch))
		{
			if (isalnum(y.back()) == 0 && y.back()!=' ')
				y+=' ';
			if (ch=='1' && y.back()=='n')
				y.back()='-';
			y=y+ch;

		}
		//If a left parenthesis is encountered, push it onto Stack.
		else if(ch=='(')
			mystack.push(ch);
		//If an operator is encountered, then: 
		else if(isoperator(ch))
		{	if (y.back()!=' ')
				y+=' ';
			//Repeatedly pop from Stack and add to Y each operator (on the top of Stack) 
			//which has the same precedence as or higher precedence than operator.
			while (isoperator(mystack.top()) and isleq(ch,mystack.top()))
			{
				if (y.back()!=' ')
					y+=' ';
				y=y+mystack.top();
				mystack.pop();
			}
			//Add operator to Stack.
			mystack.push(ch);

		}
		//If a right parenthesis is encountered, then: 
		else if(ch==')')
		{
			//Repeatedly pop from Stack and add to Y each operator 
			//(on the top of Stack) until a left parenthesis is encountered.
			while(mystack.top()!='(')
			{
				if (y.back()!=' ')
					y+=' ';
				y+=mystack.top();
				mystack.pop();
			}
			//Remove the left Parenthesis.
			mystack.pop();
		}
	}
	y.erase(0,1);
	y+=' ';
	return y;
}

float evaluate(string y, map <string,float> &myMap1)//Method which will evaluate a PostfixExpression and return the result
{	vector <string> z;
	stack <float> evStack;//Create a stack of type float to store the operands
	string sstr = y;
	while(sstr.length()>0)//Scan the postfix expression from left to right for every element
	{	
		int pos;
		string eval;
		pos=sstr.find(' ');
		eval=sstr.substr(0,pos);
		sstr=sstr.substr(pos+1);

		if(isnumber(eval))//if the element is an operand push it to the stack	
			evStack.push(stof(eval));

		else if(isvar(eval))
		{
			if (myMap1[eval]!=numeric_limits<float>::infinity())
			{
				evStack.push(myMap1[eval]);
			}
			if (myMap1[eval]==numeric_limits<float>::infinity())
				return numeric_limits<float>::infinity();
		}

		//if its a var acces the map and if its not infinity treat it as a number 
		// if myMap[eval] != infinity then stack.push(myMap[eval])
		
		else if(isoperator(eval[0]))// if the element is an operator pop 2 elements from the stack,
		{//apply the operator on it and push the result back to the stack
			float a = evStack.top(); evStack.pop();
			float b = evStack.top(); evStack.pop();
			switch(eval[0])
			{
				case '+': evStack.push(b+a); break;
				case '-': evStack.push(b-a); break;
				case '*': evStack.push(b*a); break;
				case '/': evStack.push(b/a); break;
				case '^': evStack.push(pow(b,a)); break;
				case '%': evStack.push(floor(b/a)); break;
				case '$': evStack.push(int(b)% int(a)); break;
			}
		}
	}
	return evStack.top();//return the value from the top of the stack (i.e. the final answer)
}