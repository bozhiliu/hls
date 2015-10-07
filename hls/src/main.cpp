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
#include "critical_path.hpp"
#include "stdlib.h"
#include "stdio.h"
#include "fstream"
#include "vector"
#define DEBUG

using namespace std;


void print_op(list<op> lin){
	for(list<op>::iterator it = lin.begin(); it != lin.end(); it++)
	{
		printf("Type %s, Name %s Width %d Sign %d From size %d  To size %d\n", it->type.c_str(), it->name.c_str(), it->width, it->signs, it->from.size(), it->to.size());
	}
}

void print_signals(list<signals> lin){
	for(list<signals>::iterator it = lin.begin(); it != lin.end(); it++)
	{
		printf("Type %s, Name %s From size %d To size %d\n", it->type.c_str(), it->name.c_str(), it->from.size(), it->to.size());
	}
}

void print_path(critical_pair pair){

	list<vertex_op>::iterator curr_vertex = pair.op.pred.begin();
	printf("Path %s\n", pair.op.name.c_str());
	while(!curr_vertex->pred.empty())
	{
		printf("%s\n", curr_vertex->name.c_str());
		curr_vertex = curr_vertex->pred.begin();
//		vertex_op tmp_vertex = *curr_vertex.pred.begin();
	}
}





int main(int argc, char* argv[]){

	if(argc != 3)
	{
		printf("Need two arguments! Program exit\n");
		return 1;
	}


#ifdef DEBUG
	 printf("Test\n");
//   printf("First done 3\n");

#endif




#ifdef DEBUG
	printf("Instantiate file stream\n");
#endif



	ifstream infile(argv[1]);
	ofstream outfile(argv[2]);


	list<op> netlist_op;
	list<signals> netlist_signals;

	string line_in ;


//	copy(size_type_array, size_type_array+11, size_type.begin());
	size_type.push_back(size_type_array[0]);  size_type.push_back(size_type_array[1]);
	size_type.push_back(size_type_array[2]);  size_type.push_back(size_type_array[3]);
	size_type.push_back(size_type_array[4]);  size_type.push_back(size_type_array[5]);
	size_type.push_back(size_type_array[6]);  size_type.push_back(size_type_array[7]);
	size_type.push_back(size_type_array[8]);  size_type.push_back(size_type_array[9]);
	size_type.push_back(size_type_array[10]);  size_type.push_back(size_type_array[11]);

//	copy(op_type_array, op_type_array+11, op_type.begin());
	op_type.push_back(op_type_array[0]);  op_type.push_back(op_type_array[1]);
	op_type.push_back(op_type_array[2]);  op_type.push_back(op_type_array[3]);
	op_type.push_back(op_type_array[4]);  op_type.push_back(op_type_array[5]);
	op_type.push_back(op_type_array[6]);  op_type.push_back(op_type_array[7]);
	op_type.push_back(op_type_array[8]);  op_type.push_back(op_type_array[9]);
	op_type.push_back(op_type_array[10]);  op_type.push_back(op_type_array[11]);


//	copy(signals_type_array, signals_type_array+3, signals_type.begin());
	signals_type.push_back(signals_type_array[0]); signals_type.push_back(signals_type_array[1]);
	signals_type.push_back(signals_type_array[2]);	signals_type.push_back(signals_type_array[3]);

//	copy(delimiters_array, delimiters_array+4, delimiters.begin());
	delimiters.push_back(delimiters_array[0]); delimiters.push_back(delimiters_array[1]);
	delimiters.push_back(delimiters_array[2]); 	delimiters.push_back(delimiters_array[3]);



#ifdef DEBUG
	printf("Initialize arrays!\n");
//	printf("Parse line  %d\n", infile.eof());
#endif

	while(infile.eof() == false)
	{
#ifdef DEBUG
//		printf("parse line\n");
#endif
		getline(infile, line_in);
		if (line_in.length() == 0)	continue;
		parse_netlist(line_in, netlist_op, netlist_signals);
	}


//	    printf("signal list size %d\n", netlist_signals.size());
//	    printf("op list size %d\n", netlist_op.size());
		print_signals(netlist_signals);





#ifdef DEBUG
	printf("operator config\n");
#endif
	operator_config(netlist_signals, netlist_op);
	print_op(netlist_op);

#ifdef DEBUG
	printf("write file\n");
#endif
	verilog_write(argv[1], outfile, netlist_signals, netlist_op);

#ifdef DEBUG
	printf("close file\n");
#endif

	infile.close();
	outfile.close();

#ifdef DEBUG
	printf("Find critical path\n");
#endif

list<vertex_op> sorted_vertex;
critical_pair pair;
pair = critical_path(netlist_op, netlist_signals, sorted_vertex);
printf("Critical path length %g\n", pair.max);
print_path(pair);



}
