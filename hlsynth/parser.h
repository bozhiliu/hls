#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include "data_type.h"


using namespace std;


vector<string> tokenize(string sin);
void parse_ordinary_equation(unsigned int op_count, vector<string>& tokens, vector<signals>& signals_list, vector<operation>& operation_list);
void parse_line(fstream fin, vector<signals>& signals_list, vector<operation>& operation_list, vector<branch_block>& branch_list);