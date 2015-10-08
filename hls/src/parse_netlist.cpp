/*
 * parse_list.cpp
 *
 *  Created on: 2015Äê9ÔÂ28ÈÕ
 *      Author: vito
 */

#include "parse_netlist.hpp"
//#define DEBUG
#define SWITCH
#include "stdio.h"

void tokenize(std::string in, std::vector<char> delimiters, std::vector<std::string>& tokens)
{
	string buffer = "";
	bool comment = false;
//	printf("Curr string %d\n##%s##\n", in.length(), in.c_str());
	for(unsigned int count=0; count < in.length(); count++)
	{
		bool find = false;
//		printf("char %c ,%d\n",in[count], in[count]);
		if(in[count] =='/' && in[count+1] == '/')
		{
			comment = true;
			break;
		}

		for(std::vector<char>::iterator it = delimiters.begin(); it != delimiters.end(); it++)
		{
//			printf("curr char  %d  delimiter %d\n", in[count], *it);
			if(in[count] == *it)
			{
				if(buffer.length() != 0)
				{
					tokens.push_back(buffer);
//					printf("Tokens %s\n", buffer.c_str());
				}
				buffer = "";
				find = true;
				continue;
			}
		}
		if(find == true)
			{
				continue;
			}
		buffer = buffer + in[count];

	}
//	printf("Leave sentence %s\n", buffer.c_str());
	if(buffer.length()!=0)
	{
		tokens.push_back(buffer);
//		printf("Tokens %s\n", buffer.c_str());
	}
}


int parse_netlist(std::string in, op_list& op_list, signals_list& signals_list)
{
	vector<string> tokens ;
#ifdef 	DEBUG
	printf("Function start\n");
#endif
	tokenize(in, delimiters, tokens);
#ifdef 	DEBUG
	printf("Get tokens\n");
	printf("Token vector size: %d\n", tokens.size());
#endif
	int op_count = op_list.size();
	bool signs;
	int width;
	size_info curr_info (unsign, -1);
	string curr_signals_type = "";
	bool size_found = false;
	bool signals_type_found = false;
	bool op_type_found = false;

#ifdef 	DEBUG
	printf("Parse initialize\n");
#endif


	// Find size tokens of current statement
#ifdef 	DEBUG
		printf("Finding size token\n");
#endif


	for(vector<string>::iterator it2 = tokens.begin(); it2 != tokens.end(); it2++)
	{

#ifdef 	DEBUG
		printf("Current token %s \n", it2->c_str());
#endif
		for(vector<string>::iterator it = size_type.begin(); it !=size_type.end(); it++)
		{
			string curr_type = *it;
			string curr_token = *it2;
			if (curr_type == curr_token)
				{
					int 	pos = std::distance(size_type.begin(), it);
					curr_info.signs = size_info_array[pos].signs;
					curr_info.width = size_info_array[pos].width;
	//				printf("Curr statement %s sign %d width %d\n", in.c_str(), curr_info.signs, curr_info.width);
					size_found = true;
#ifdef 	DEBUG
	printf("Find size token: ");
	printf("%s\n" , it2->c_str());
#endif
					tokens.erase(it2);
					break;
				}
		}
		if(size_found == true)
			{
				break;
			}
	}

#ifdef 	DEBUG
	printf("Stamp Point1 \n");
#endif


	// Find type tokens of current statement
	for(vector<string>::iterator it2 = tokens.begin(); it2 != tokens.end(); it2++)
	{
#ifdef 	DEBUG
	printf("Finding type token: curr %s\n", it2->c_str());
#endif

		// When signals type determined, consider the rest tokens as signalss
		if(signals_type_found == true)
			{
#ifdef 	DEBUG
		printf("Find signal token %s \n", it2->c_str());
#endif
				string curr_token = *it2;
				signals  s  = *(new signals());
				s.signs = curr_info.signs;
				s.width = curr_info.width;
				s.type.assign(curr_signals_type);
				s.name.assign(curr_token);
				signals_list.push_back(s);
				continue;
			}
		// Find the signals type
		for(vector<string>::iterator it = signals_type.begin(); it !=signals_type.end(); it++)
			{

				string curr_type = *it;
				string curr_token = *it2;
#ifdef 	DEBUG
		printf("Compare with signal type array %s  %s \n", it2->c_str(), it->c_str());
#endif
				if (curr_type == curr_token)
					{
#ifdef 	DEBUG
	printf("Find type token: %s\n", it->c_str());
#endif

						signals_type_found = true;
						if(size_found == false)
						{
							signals_type_found = false;
							printf("Return pos 0\n");
							return 1;
						}
						int pos = std::distance(signals_type.begin(), it);
						curr_signals_type = signals_type_array[pos];
						break;
					}
			}
	}


#ifdef 	DEBUG
	printf("Stamp Point2 \n");
#endif

	// Find operator tokens of current statement
	if(tokens[1] == "=")
	{
		string message ;
		list<signals> curr_signals_from_list;
		signals curr_signals_to;
		bool reg_statement = true;
		bool valid_statement = true;
		bool constant_one = false;
		// LHS of equation: find the sink signals
		for(list<signals>::iterator it = signals_list.begin(); it != signals_list.end(); it++)
		{
			signals curr_signals = *it;
			if(tokens[0] == curr_signals.name)
			{
				valid_statement = true;
				curr_signals_to = curr_signals;
				break;
			}
			else
			{
				valid_statement = false;
				message = "Unrecongnized Left hand side of equation\n";
//				printf("Curr token %s\n", tokens[0].c_str());
				continue;
			}
		}
#ifdef 	DEBUG
	printf("Stamp Point3 \n");
#endif
		// RHS: find the source signalss and operator
		for(vector<string>::iterator it2 = tokens.begin()+2; it2 != tokens.end(); it2++)
		{
			bool curr_token_found = false;
//			printf("Curr str3 %s  token %s  reg %d\n", in.c_str(), it2->c_str(), reg_statement);
			// find signals for potential operator
			if(curr_token_found == false)
			{
				for(list<signals>::iterator it = signals_list.begin(); it != signals_list.end(); it++)
				{
					string curr_token = *it2;
					signals curr_signals = *it;
					if (curr_token == curr_signals.name)
					{
						reg_statement = reg_statement & 1;
						curr_signals_from_list.push_back(*it);
						curr_token_found = true;

						break;
					}
				}
				if(curr_token_found == true)	continue;
			}
			// find operator
			if(curr_token_found == false)
			{
				// Handle increment / decrement
				if (*it2 == "1")
				{
					constant_one = true;
					reg_statement = false;
					curr_token_found = true;
					op_list.back().name = op_list.back().name+"1";
//					printf("Curr str inc/dec %s type %s\n", in.c_str(), op_list.back().type.c_str());
					if(op_list.back().type == "ADD")	op_list.back().type = "INC";
					if(op_list.back().type == "SUB")		op_list.back().type = "DEC";
					continue;
//					printf("Curr str inc/dec %s type %s\n", in.c_str(), op_list.back().type.c_str());
				}
				for(vector<string>::iterator it = op_type.begin(); it != op_type.end(); it++)
				{
					string curr_token = *it2;
					string curr_op = *it;
					int 	pos = std::distance(op_type.begin(), it);


					// special case handling for comp
					if(curr_op == "COMP")
					{
						if(curr_token == ">" || curr_token == "<" || curr_token == "==")
						{
							reg_statement = false;
							op_type_found = true;
							curr_token_found = true;
							op op_found =*( new op());
							string op_count_name = "";		stringstream ss;
							ss << op_count;		ss >> op_count_name ; 		op_count_name = op_count_name + "_";
							op_found.name = op_count_name+curr_op;
							op_count = 1+op_count;
							op_found.type = *it;
							op_found.type = op_found.type + curr_token;
							op_list.push_back(op_found);
							break;
						}
					}

					if(curr_token == op_token_array[pos])
					{
						reg_statement = false;
						op_type_found = true;
						curr_token_found = true;
						op op_found =*( new op());
						string op_count_name = "";		stringstream ss;
						ss << op_count;		ss >> op_count_name ; 		op_count_name = op_count_name + "_";

						op_found.name = op_count_name+curr_op;
						op_count = 1+op_count;
						op_found.type = *it;
						if(curr_op  == "COMP")		op_found.type = op_found.type + curr_token;
						op_list.push_back(op_found);
						break;
					}
				}
				if(curr_token_found == true)	continue;
			}
			valid_statement = false;
			message = "Unrecongnized right hand side token\n";
//            printf("Curr token2 %s\n", it2->c_str());
			reg_statement = false;

		}
		// After processing RHS, deal with registers
		if(reg_statement == true)
		{
//	 		printf("##########\nCurr str %s\n############\n", in.c_str());
			op op_found = *(new op());
			string op_count_name = "";		stringstream ss;
			ss << op_count;		ss>> op_count_name ; 		op_count_name = op_count_name + "_";
			op_found.name = op_count_name + "REG";
			op_count ++;
			op_found.type = "REG";
			op_list.push_back(op_found);
		}

		// Process the from and to relations between signalss and operators
//		printf("Curr statement %s valid %d\n", in.c_str(), valid_statement);
		if(valid_statement == true)
		{

//			op_list.back().to.push_back(curr_signals_to);
//			curr_signals_to.from.push_back(op_list.back());
			for(list<signals>::iterator it = signals_list.begin(); it != signals_list.end(); it ++)
			{
				if(it->name == curr_signals_to.name)
				{
					it->from.push_back(op_list.back());
					op_list.back().to.push_back(*it);
				}
			}


			for(list<signals>::iterator it = curr_signals_from_list.begin(); it != curr_signals_from_list.end(); it++)
			{
//				op_list.back().from.push_back(*it);
//				it->to.push_back(op_list.back());
				for(list<signals>::iterator it2 = signals_list.begin(); it2 != signals_list.end(); it2 ++)
				{
					if(it->name == it2->name)
					{
						it2->to.push_back(op_list.back());
						op_list.back().from.push_back(*it2);
					}
				}
			}
		}
		else
		{
//			printf("Return pos 1\n");
			printf("%s", message.c_str());
			return 1;

		}
	}



	curr_info.signs = unsign;
	curr_info.width = -1;
	curr_signals_type = "";
	size_found = false;
	signals_type_found = false;
	op_type_found = false;
 //   printf("signal list size %d\n", signals_list.size());
 //   printf("op list size %d\n", op_list.size());

	return 0;
}
