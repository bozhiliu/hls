/*
 * data_structs.cpp
 *
 *  Created on: 2015Äê10ÔÂ5ÈÕ
 *      Author: vito
 */




#include "data_structs.hpp"


std::string op_type_array[] ={ "ADD","SUB","MUL","COMP","MUX","SHR","SHL","DIV","MOD","INC","DEC","REG"} ;
std::string op_token_array[] = {"+",  "-", "*", ">", "<", "==", "?", "<<", ">>", "/", "%", "1"};

std::string signals_type_array[] = {
	"INPUT", "OUTPUT", "WIRE", "REG"
};

std::string size_type_array[] = {
	"Int1", "Int2", "Int8", "Int16"", Int32", "Int64",
	"UInt1", "UInt2", "UInt8", "UInt16", "UInt32", "UInt64"
};

size_info size_info_array[] = {int1_info, int2_info, int8_info, int16_info, int32_info, int64_info,
											uint1_info, uint2_info, uint8_info, uint16_info, uint32_info, uint64_info};

char delimiters_array[] = {
		','  ,
		' '  ,
		';' ,
		':'
};

size_info int1_info(sign, 1);		size_info int2_info (sign, 2);
size_info int8_info (sign, 8);		size_info int16_info (sign, 16);
size_info int32_info (sign, 32);    size_info int64_info (sign, 64);
size_info uint1_info (unsign, 1);  size_info uint2_info (unsign, 2);
size_info uint8_info (unsign, 8);   size_info uint16_info (unsign, 16);
size_info uint32_info (unsign, 32);  size_info uint64_info (unsign, 64);

