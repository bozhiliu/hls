/*
 * verilog_write.hpp
 *
 *  Created on: 2015Äê10ÔÂ4ÈÕ
 *      Author: vito
 */

#ifndef VERILOG_WRITE_HPP_
#define VERILOG_WRITE_HPP_
#include "data_structs.hpp"
#include "fstream"


using namespace std;
void verilog_write(char * input_name, ofstream outfile, signals_list signals_list, op_list op_list);




#endif /* VERILOG_WRITE_HPP_ */
