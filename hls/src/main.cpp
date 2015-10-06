/*
 * main.cpp
 *
 *  Created on: 2015Äê9ÔÂ28ÈÕ
 *      Author: vito
 */

#include "stdlib.h"
#include "stdio.h"
#include "data_structs.hpp"
#include "parse_netlist.hpp"
#include "operator_config.hpp"
#include "verilog_write.hpp"
#include "fstream"

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
	signals_type.assign(signals_type_array[0], signals_type_array[3]);
	delimiters.assign(delimiters_array[0], delimiters_array[2]);

	ifstream infile(argv[1]);
	ofstream outfile(argv[2]);


	list<op> netlist_op;
	list<signals> netlist_signals;

	string line_in ;

	while(!infile.eof())
	{
		getline(infile, line_in);
		parse_netlist(line_in, netlist_op, netlist_signals);
	}

	operator_config(netlist_signals, netlist_op);

	verilog_write(argv[1], outfile, netlist_signals, netlist_op);

	infile.close();
	outfile.close();

}
