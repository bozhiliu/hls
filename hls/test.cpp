/*
 * test.cpp
 *
 *  Created on: 2015Äê10ÔÂ3ÈÕ
 *      Author: vito
 */


#include "stdlib.h"
//#include "string.h"
#include "iostream"
#include "string.h"
using namespace std;
#include "vector"
#include "algorithm"
#include <fstream>
 int main(int argc, char * argv[]){

	 /*
    ifstream table;                            // 1. Create instance
    table.open(argv[1]);                   // 2. Open the file
    char line[200];

    string x;
    getline(table, x);

   if(x.find("ffff") == -1)
   {
	   printf("No ffff\n");
   }
   printf("pos %d", x.find("cccc"));
    printf("%s\n", line);
    string t1 = "This, sss,, ddd";
    for(std::string::iterator it = t1.begin(); it != t1.end(); it++)
      {
	std::cout<<*it << "\n";
      }

    string xx = "";
    printf("xx ##%s##\n", xx.c_str());
    xx = xx + " BBBBBBBBBEBBB";
    printf("xx ##%s##\n", xx.c_str());
    printf("xx size %d\n", strlen(xx.c_str()));
    string x1 = "ABC";
    for(string::iterator it = x1.begin(); it!=x1.end(); it++)
      {
        if((*it == 'B')) printf("Found B!\n");
	printf("%c ",*it);
      }

    vector<string> v1;
    v1.push_back("AA");
    v1.push_back("bb");
    v1.push_back("cc");
    for(std::vector<string>::iterator it = v1.begin(); it!=v1.end(); v1++)
    {
    	string tmp = *it;
    	printf("length %d\n", tmp.length());
    }
*/

	 std::vector<string> op_type ; //("ADD","SUB","MUL","COMP","MUX","SHR","SHL","DIV","MOD","INC","DEC","REG") ;
	 op_type.push_back("ADD");
	 op_type.push_back("SUB");
	 op_type.push_back("UInt");
	 op_type.push_back("MUL");
	 op_type.push_back("COMP");
	 vector<string>::iterator tmp ;
	 string t1;
//	 vector<string> test;
//	 test.push_back("ADD");
	for(tmp = op_type.begin(); tmp != op_type.end(); tmp++)
	{
		printf("%s \n", tmp->c_str());
		t1 = *tmp;
		t1 = "new";
		printf("%s\n", tmp->c_str());
		printf("%s\n", t1.c_str());
		//		printf("T1\n");
//		if(t1.find("Int") != t1.npos)	printf("Int Found!  %s\n", t1.c_str());
	}
	ofstream test_out("test_out.txt");
	test_out << "TEST  AAA";
	int pos = test_out.tellp();
	printf("pos %d", pos);
	test_out.seekp(pos+5);
	test_out << "5"*3;
 }
