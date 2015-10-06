/*
 * data_structs.hpp
 *
 *  Created on: 2015Äê9ÔÂ28ÈÕ
 *      Author: vito
 */

#ifndef DATA_STRUCTS_HPP_
#define DATA_STRUCTS_HPP_

#include <list>
#include <vector>
#include <string>
#include <stdlib.h>
#include "stdio.h"

using namespace std;


#define sign 1
#define unsign 0


std::vector<string> op_type;
std::string op_type_array[] ={ "ADD","SUB","MUL","COMP","MUX","SHR","SHL","DIV","MOD","INC","DEC","REG"} ;
std::string op_token_array[] = {"+",  "-", "*", ">", "<", "==", "?", "<<", ">>", "/", "%", "1"};



std::vector<string> signals_type ;
std::string signals_type_array[] = {
	"INPUT", "OUTPUT", "WIRE", "REG"
};


std::vector<string> size_type ;//
std::string size_type_array[] = {
	"Int1", "Int2", "Int8", "Int16"", Int32", "Int64",
	"UInt1", "UInt2", "UInt8", "UInt16", "UInt32", "UInt64"
};

typedef struct size_info {
	bool signs;
	int width;
	size_info(bool s, int w) {signs = s; width = w;}
} size_info;

size_info int1_info(sign, 1);      size_info int2_info (sign, 2);
size_info int8_info (sign, 8);      size_info int16_info (sign, 16);
size_info int32_info (sign, 32);      size_info int64_info (sign, 64);
size_info uint1_info (unsign, 1);      size_info uint2_info (unsign, 2);
size_info uint8_info (unsign, 8);      size_info uint16_info (unsign, 16);
size_info uint32_info (unsign, 32);      size_info uint64_info (unsign, 64);
size_info size_info_array[] = {int1_info, int2_info, int8_info, int16_info, int32_info, int64_info,
											uint1_info, uint2_info, uint8_info, uint16_info, uint32_info, uint64_info};



std::vector<char> delimiters;
char delimiters_array[] = {
		','  ,
		' '  ,
		';' ,
		':'
};
struct op;
//struct signals;

struct signals{
	string name;
	int width;
	bool signs;
	string type;
	std::list<op> from;
	std::list<op> to;
};


struct op{
	string name;
	int width;
	bool signs;
	string type;
	std::list<struct::signals> from;
	std::list<struct::signals> to;
};

/*
typedef struct signals{
	string name;
	int width;
	bool signs;
	string type;
	std::list<op> from;
	std::list<op> to;
} signals;


typedef struct op{
	string name;
	int width;
	bool signs;
	string type;
	std::list<signals> from;
	std::list<signals> to;
} op;
*/


typedef std::list<signals> signals_list;
typedef std::list<op> op_list;





#endif /* DATA_STRUCTS_HPP_ */
