/*
 * main.cpp
 *
 *  Created on: 2015Äê9ÔÂ28ÈÕ
 *      Author: vito
 */


#include "data_structs.hpp"
#include "parse_netlist.hpp"
#include "operator_config.hpp"
#include "verilog_write.hpp"
#include "stdlib.h"
#include "stdio.h"
#include "fstream"
#include "vector"
#define DEBUG

using namespace std;
int main(int argc, char* argv[]){

	if(argc != 3)
	{
		printf("Need two arguments! Program exit\n");
		return 1;
	}
#ifdef DEBUG
	printf("Initialize arrays!\n");
#endif
//	copy(size_type_array, size_type_array+11, size_type.begin());
//	size_type.push_back(size_type_array[0]);  size_type.push_back(size_type_array[1]);
//	size_type.push_back(size_type_array[2]);  size_type.push_back(size_type_array[3]);
//	size_type.push_back(size_type_array[4]);  size_type.push_back(size_type_array[5]);
//	size_type.push_back(size_type_array[6]);  size_type.push_back(size_type_array[7]);
//	size_type.push_back(size_type_array[8]);  size_type.push_back(size_type_array[9]);
//	size_type.push_back(size_type_array[10]);  size_type.push_back(size_type_array[11]);

//	copy(op_type_array, op_type_array+11, op_type.begin());
//	op_type.push_back(op_type_array[0]);  op_type.push_back(op_type_array[1]);
//	op_type.push_back(op_type_array[2]);  op_type.push_back(op_type_array[3]);
//	op_type.push_back(op_type_array[4]);  op_type.push_back(op_type_array[5]);
//	op_type.push_back(op_type_array[6]);  op_type.push_back(op_type_array[7]);
//	op_type.push_back(op_type_array[8]);  op_type.push_back(op_type_array[9]);
//	op_type.push_back(op_type_array[10]); // op_type.push_back(op_type_array[11]);


//	copy(signals_type_array, signals_type_array+3, signals_type.begin());
//	signals_type.push_back(signals_type_array[0]); signals_type.push_back(signals_type_array[1]);
//	signals_type.push_back(signals_type_array[2]);//	signals_type.push_back(signals_type_array[3]);

//	copy(delimiters_array, delimiters_array+4, delimiters.begin());
//	delimiters.push_back(delimiters_array[0]); delimiters.push_back(delimiters_array[1]);
//	delimiters.push_back(delimiters_array[2]); //	delimiters.push_back(delimiters_array[3]);

#ifdef DEBUG
   printf("First done 3\n");
#endif

   printf("Test");


#ifdef DEBUG
	printf("Instantiate file stream");
#endif



	ifstream infile(argv[1]);
	ofstream outfile(argv[2]);


	list<op> netlist_op;
	list<signals> netlist_signals;

	string line_in ;

	while(!infile.eof())
	{
		getline(infile, line_in);
#ifdef DEBUG
		printf("parse line");
#endif
		parse_netlist(line_in, netlist_op, netlist_signals);
	}

#ifdef DEBUG
	printf("operator config");
#endif
	operator_config(netlist_signals, netlist_op);

#ifdef DEBUG
	printf("write file");
#endif
	verilog_write(argv[1], outfile, netlist_signals, netlist_op);

	infile.close();
	outfile.close();

}
