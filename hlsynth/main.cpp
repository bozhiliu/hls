#include <stdio.h>
#include <vector>
#include "data_type.h"
#include <fstream>
#include <iostream>
#include "parser.h"
#include "scheduling.h"


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
    
    sequence_graph graph;
    graph.create_sequence_graph(signals_list, operator_list, branch_list);
    graph.asap_schedule();
    graph.alap_schedule(10);
    graph.force_directed_schedule(10);
    
    
    
	return 0;
}
