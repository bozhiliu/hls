#include <stdio.h>
#include <vector>
#include "data_type.h"
#include <fstream>
#include <iostream>
#include "parser.h"



int main(int argc, char **argv)
{
    vector<signals> signals_list;
    vector<operation> operator_list;
    vector<branch_block> branch_list;
    initial_delimiter();
    initial_otype();
    initial_stype();
    
    fstream fin;
    fin.open(argv[1], fstream::in);
    parse_line(fin, signals_list, operator_list, branch_list);
    
	return 0;
}
