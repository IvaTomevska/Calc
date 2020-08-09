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

//=============Main funciton==============
int main(int argc, char** argv)
{
	string line;
	vector <string> devidedlines;
	//opening text file for reading
	ifstream inFile;
	inFile.open(argv[1]); 
	//check if the dictionary file is succesfully open 
	if (!inFile)
	{
		cerr<<"Failed to open the file"<<endl;
		exit(1);
	}

	//create a string vector array out of the equations in the file
	vector <string> tmp;
	while(getline(inFile,line))
	{
		tmp = divideinput(line);//devides the variables from expressions
		for (int i=0;i<tmp.size();i++) {
			devidedlines.push_back(tmp[i]);
		}
	}

	inFile.close();
	//creating a map of the variables and float values
	map <string, float> myMap;
	for(int i=0;i<devidedlines.size();i+=2)
	{
		myMap[devidedlines[i]]= numeric_limits<float>::infinity();//initializing the variables to infinity
	}
	

	bool check=1;//if all values in the map are evaluated(not infinity) it becomes 0

	//opening the output file
	ofstream outFile;
	outFile.open(argv[2]);

	while (check)
	{
		int cnt=1;
		for(int i=1;i<devidedlines.size();i++)
		{
			if (myMap[devidedlines[i-1]]==numeric_limits<float>::infinity())
			{
				string topostfix=infix2postfix(devidedlines[i]);
				float ans=evaluate(topostfix, myMap);
				myMap[devidedlines[i-1]]=ans;
			}

			else
				cnt+=1;
			if (cnt==myMap.size()) {
				for(int i=0;i<devidedlines.size();i+=2)
				{
					outFile << devidedlines[i] << " = " << myMap[devidedlines[i]] << endl;
				}
				check=0;
				break;
			}

		}

	}
	outFile.close();
	return EXIT_SUCCESS;
}
//=====================================================================
