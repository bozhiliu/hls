/*
 * critical_path.hpp
 *
 *  Created on: 2015Äê10ÔÂ6ÈÕ
 *      Author: vito
 */

#ifndef SRC_CRITICAL_PATH_HPP_
#define SRC_CRITICAL_PATH_HPP_
#include "data_structs.hpp"


struct vertex_op;

struct vertex_op{
	string name;
	bool status;
	float dist;
	float delay;
	list<string> from;
	list<string> to;
	list<vertex_op> from_op;
	list<vertex_op> to_op;
	list<vertex_op> pred;
	string type;
};

struct critical_pair {
	vertex_op op;
	float max;
};

void visit(vertex_op &s_op, list<vertex_op> &vertex_list, list<vertex_op> &sorted_vertex );

critical_pair critical_path(op_list & op_list, signals_list & signals_list, list<vertex_op>& sorted_vertex);




void topological_sort(op_list &op_list, signals_list &signals_list, list<vertex_op> &vertex_list, list<vertex_op> &sorted_vertex);




#endif /* SRC_CRITICAL_PATH_HPP_ */
