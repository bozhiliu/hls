/*
 * main.cpp
 *
 *  Created on: 2015��9��28��
 *      Author: vito
 */

#include "stdlib.h"
#include "stdio.h"
#include "parse_netlist.hpp"
#include "data_structs.hpp"
#include "operator_config.hpp"
#include "verilog_write.hpp"


//#include "vector"
using namespace std;
int main(int argc, char* argv[]){


	if(argc != 3)
	{
		printf("Need two arguments! Program exit\n");
		return 1;
	}

	size_type.assign(size_type_array[0], size_type_array[11]);
	op_type.assign(op_type_array[0], op_type_array[11]);
	signal_type.assign(signal_type_array[0], signal_type_array[3]);
	delimiters.assign(delimiters_array[0], delimiters_array[2]);

	ifstream infile(argv[1]);
	ofstream outfile(argv[2]);


	list<op> netlist_op;
	list<signal> netlist_signal;

	string line_in ;

	while(!infile.eof())
	{
		getline(infile, line_in);
		parse_netlist(line_in, netlist_op, netlist_signal);
	}

	operator_config(netlist_signal, netlist_op);

	verilog_write(argv[1], outfile, netlist_signal, netlist_op);

	infile.close();
	outfile.close();








}
