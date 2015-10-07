/*
 * parse_netlist.hpp
 *
 *  Created on: 2015Äê9ÔÂ28ÈÕ
 *      Author: vito
 */

#ifndef PARSE_NETLIST_HPP_
#define PARSE_NETLIST_HPP_
#include "data_structs.hpp"
#include "algorithm"
#include "sstream"



using namespace std;

int parse_netlist(std::string in, op_list& op_list, signals_list& signals_list);

std::vector<std::string> find_size(std::string);

vector<std::string> tokenize(std::string in, std::vector<std::string> delimiters, std::vector<std::string>& tokens);


#endif /* PARSE_NETLIST_HPP_ */
