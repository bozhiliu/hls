#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include "data_type.h"


using namespace std;


void branch_parse(string sin, vector<branch_block> branch_list);
vector<string> tokenize(string sin);
void parse_line(fstream fin, vector<signals> signals_list, vector<operation> operation_list);