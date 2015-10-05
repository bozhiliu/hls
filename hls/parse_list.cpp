/*
 * parse_list.cpp
 *
 *  Created on: 2015��9��28��
 *      Author: vito
 */

#include "parse_netlist.hpp"




void tokenize(std::string in, std::vector<char> delimiters, std::vector<std::string> tokens)
{
	string buffer = "";
	for(int count=0; count < in.length(); count++)
	{
		bool find = false;
		for(const std::vector<char>::iterator it = delimiters.begin(); it != delimiters.end(); it++)
		{
			if(in[count] == *it)
			{
				if(buffer.length() != 0)
					tokens.push_back(buffer);
				buffer = "";
				find = true;
				continue;
			}
		}
		if(find == true) continue;
		buffer = buffer + in[count];
		continue;
	}
}


int parse_netlist(std::string in, op_list op_list, signal_list signal_list)
{
	vector<string> tokens;
	tokenize(in, delimiters, tokens);

	int op_count = 0;
	bool signs;
	int width;
	size_info curr_info {sign, -1};
	string curr_signal_type = "";
	bool size_found = false;
	bool signal_type_found = false;
	bool op_type_found = false;

	// Find size tokens of current statement
	for(vector<string>::iterator it2 = tokens.begin(); it2 != tokens.end(); it2++)
	{
		for(vector<string>::iterator it = size_type.begin(); it !=size_type.end(); it++)
		{
			string curr_type = *it;
			string curr_token = *it2;
			if (curr_type == curr_token)
				{
					int 	pos = std::distance(size_type.begin(), it);
					curr_info = size_info_array[pos];
					size_found = true;
					break;
				}
		}
		if(size_found == true)	break;
	}

	// Find type tokens of current statement
	for(vector<string>::iterator it2 = tokens.begin(); it2 != tokens.end(); it2++)
	{
		// When signal type determined, consider the rest tokens as signals
		if(signal_type_found == true)
			{
				string curr_token = *it2;
				signal  s  = new signal();
				s.signs = curr_info.signs;
				s.width = curr_info.width;
				s.type.assign(curr_signal_type);
				s.name.assign(curr_token);
				signal_list.push_back(s);
				continue;
			}
		// Find the signal type
		for(vector<string>::iterator it = signal_type.begin(); it !=signal_type.end(); it++)
			{
				string curr_type = *it;
				string curr_token = *it2;
				if (curr_type == curr_token)
					{
						signal_type_found = true;
						if(size_found == false)
						{
							signal_type_found = false;
							return 1;
						}
						int pos = std::distance(signal_type.begin(), it);
						curr_signal_type = signal_type_array[pos];
						break;
					}
			}
	}

	// Find operator tokens of current statement
	if(tokens[1] == "=")
	{
		list<signal> curr_signal_from_list;
		signal curr_signal_to;
		bool reg_statement = true;
		bool valid_statement = false;
		bool constant_one = false;
		// LHS of equation: find the sink signal
		for(list<signal>::iterator it = signal_list.begin(); it != signal_list.end(); it++)
		{
			signal curr_signal = *it;
			if(tokens[0] == curr_signal.name)
			{
				valid_statement = true;
				curr_signal_to = curr_signal;
				continue;
			}
		}

		// RHS: find the source signals and operator
		for(vector<string>::iterator it2 = tokens[2]; it2 != tokens.end(); it2++)
		{
			bool curr_token_found = false;

			// find signals for potential operator
			if(curr_token_found == false)
			{
				for(list<signal>::iterator it = signal_list.begin(); it != signal_list.end(); it++)
				{
					string curr_token = *it2;
					signal curr_signal = *it;
					if (curr_token == curr_signal.name)
					{
						reg_statement = reg_statement && 1;
						curr_signal_from_list.push_back(*it);
						curr_token_found = true;
						continue;
					}
				}
			}
			// find operator
			if(curr_token_found == false)
			{
				// Handle increment / decrement
				if (*it2 == "1")
				{
					constant_one = true;
					op_list.back().name = op_list.back().name+"1";
					if(op_list.back().type == "ADD")	op_list.back().type = "INC";	continue;
					if(op_list.back().type == "SUB")	op_list.back().type = "INC";	continue;
				}
				for(vector<string>::iterator it = op_type.begin(); it != op_type.end(); it++)
				{
					string curr_token = *it2;
					op curr_op = *it;
					int 	pos = std::distance(op_type.begin(), it);
					if(curr_token == op_token_array[pos])
					{
						reg_statement = false;
						op_type_found = true;
						op op_found = new op();
						string op_count_name = "";		stringstream ss;
						ss << op_count;		op_count_name << ss; 		op_count_name = op_count_name + "_";
						op_found.name = op_count_name+curr_op;
						op_count ++;
						op_found.type = *it;
						op_list.push_back(op_found);
						continue;
					}
				}
			}
			valid_statement = false;
			reg_statement = false;
		}

		// After processing RHS, deal with registers
		if(reg_statement == true)
		{
			op op_found = new op();
			string op_count_name = "";		stringstream ss;
			ss << op_count;		op_count_name << ss; 		op_count_name = op_count_name + "_";
			op_found.name = op_count_name + "REG";
			op_count ++;
			op_found.type = "REG";
			op_list.push_back(op_found);
		}

		// Process the from and to relations between signals and operators
		if(valid_statement == true)
		{
			op_list.back().to.push_back(curr_signal_to);
			curr_signal_to.from.push_back(op_list.back());

			for(list<signal>::iterator it = curr_signal_from_list.begin(); it != curr_signal_from_list.end(); it++)
			{
				op_list.back().from.push_back(*it);
				it->to.push_back(op_list.back());
			}
		}
	}


	size_info curr_info {sign, -1};
	string curr_signal_type = "";
	bool size_found = false;
	bool signal_type_found = false;
	bool op_type_found = false;
	return 0;
}
