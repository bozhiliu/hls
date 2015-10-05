/*
 * operator_config.cpp
 *
 *  Created on: 2015Äê10ÔÂ4ÈÕ
 *      Author: vito
 */


#include "operator_config.hpp"

using namespace std;

void operator_config(signal_list signal_list, op_list op_list)
{
	int reg_add = 0;
	// Check if any non-reg operator output is reg. If so, create a new signal and a REG operator
	for(list<op>::iterator it = op_list.begin(); it!= op_list.end(); it++)
	{
		op curr_op = *it;
		if(it->type != "REG")
		{
			for(list<signal>::iterator it2 = it->to.begin(); it2 != it->to.end(); it2++)
			{
				if (it2->type == "REG")
				{
					stringstream ss;
					ss << reg_add;
					string count;
					count << ss;
					signal s = new signal();
					op o = new op();

					s.name = count + "_tmp_signal";
					s.signs = it2->signs;
					s.type = "WIRE";
					s.from.push_back(*it);
					s.to.push_back(o);
					s.width = it2->width;
					signal_list.push_back(s);

					o.name = count + "_tmp_reg";
					o.signs = sign;
					o.width = 0;
					o.from.push_back(s);
					o.to.push_back(*it2);
					o.type = "REG";
					op_list.push_back(o);

					it2->from.erase(it);
					it->to.erase(it2);
					reg_add +=1;
				}
			}
		}
	}





	for(list<op>::iterator it = op_list.begin()   ; it!= op_list.end(); it++)
	{
		op curr_op = *it;
		bool input_sign = unsign;
		int width = 0;


		// Find the sign of current operator
		for(list<signal>::iterator it2 = curr_op.from.begin(); it2 != curr_op.from.end(); it2++)
		{
			signal curr_signal = *it2;
			if(curr_op.type == "COMP")
			{
				if (curr_signal.width > width)	width = curr_signal.width;
			}
			if (curr_op.signs == sign)
			{
				input_sign = sign;
			}
		}

		// Find the width of current operator
		for(list<string>::iterator it2 = curr_op.to.begin(); it2 != curr_op.to.end(); it2++)
		{
			signal curr_signal = *it2;
			if(curr_op.type != "COMP")
			{
				if(curr_signal.width > width)		width = curr_signal.width;
			}
		}

		it->signs = input_sign;
		it->width = width;
	}
}
