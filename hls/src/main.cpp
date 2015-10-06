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
#include "vector"

//#include "vector"
using namespace std;
int main(int argc, char* argv[]){


	if(argc != 3)
	{
		printf("Need two arguments! Program exit\n");
		return 1;
	}

	copy(size_type_array, size_type_array+11, size_type.begin());
	copy(op_type_array, op_type_array+11, op_type.begin());
	copy(signals_type_array, signals_type_array+3, signals_type.begin());
	copy(delimiters_array, delimiters_array+2, delimiters.begin());

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
