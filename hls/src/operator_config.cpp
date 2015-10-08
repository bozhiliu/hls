/*
 * operator_config.cpp
 *
 *  Created on: 2015Äê10ÔÂ4ÈÕ
 *      Author: vito
 */


#include "operator_config.hpp"

using namespace std;

void operator_config(signals_list& signals_list, op_list& op_list)
{
	int reg_add = 0;
	// Check if any non-reg operator output is reg. If so, create a new signals and a REG operator
	for(list<op>::iterator it = op_list.begin(); it!= op_list.end(); it++)
	{
		op curr_op = *it;
		if(it->type != "REG")
		{
			for(list<signals>::iterator it2 = it->to.begin(); it2 != it->to.end(); it2++)
			{
				if (it2->type == "register")
				{
		//			printf("Happens!\n");
					stringstream ss;
					ss << reg_add;
					string count;
					ss >> count;
					signals s =*( new signals());
					op o =*( new op());

					s.name = count + "_tmp_signals";
					s.signs = it2->signs;
					s.type = "WIRE";
					s.from.push_back(*it);
					s.to.push_back(o);
					s.width = it2->width;
					signals_list.push_back(s);

					o.name = count + "_tmp_reg";
					o.signs = unsign;
					o.width = 0;
					o.from.push_back(s);
					o.to.push_back(*it2);
					o.type = "REG";
					op_list.push_back(o);

					it2->from.erase(it);
					it2->from.push_back(o);
					it->to.erase(it2);
					it->to.push_back(s);
					reg_add +=1;
				}
			}
		}
	}


    for(list<signals>::iterator it = signals_list.begin(); it!= signals_list.end(); it++)
    {
//    	printf("Curr signal %s type %s\n", it->name.c_str(), it->type.c_str());
    	if (it->type == "output")
    	{
//    		printf("Check %d %s\n", it->from.size(), it->from.begin()->type.c_str());
    		for(list<op>::iterator it2 = it->from.begin();  it2 != it->from.end(); it2++)
    		{
    			for(list<op>::iterator it3 = op_list.begin();  it3 != op_list.end();  it3++)
    			{
    				if (it2->name == it3->name)
    				{
    					if (it3->type != "REG")
    					{
    						stringstream ss;
    						ss << reg_add;
    						string count;
    						ss >> count;
    						signals s =*( new signals());
    						op o =*( new op());
//    						printf("S1\n");
    					    s.name = count + "_tmp_signals";
    					    s.type = "wire";
    					    s.signs = it->signs;
    					    s.width = it->width;
    					    s.to.push_back(o);
    					    s.from.push_back(*it3);
    					    signals_list.push_back(s);
//    					    printf("S2\n");
    					    o.name = count + "_tmp_reg";
    					    o.width =0;
    					    o.signs = unsign;
    					    o.from.push_back(s);
    					    o.to.push_back(*it);
    					    o.type = "REG";
    					    op_list.push_back(o);
//    					    printf("S3\n");

    					    it3->to.push_front(s);
    		//			    it->from.push_back(o);
    		//			    it3->to.erase(it);
    		//			    it->from.erase(it3); //
    					    reg_add +=1;
//    					    printf("S4\n");
    					   }
    				break;
    				}
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
		for(list<signals>::iterator it2 = curr_op.from.begin(); it2 != curr_op.from.end(); it2++)
		{

			signals curr_signals = *it2;
			if(curr_op.type.find("COMP") != curr_op.type.npos)
			{

				if (curr_signals.width > width)	width = curr_signals.width;
			}
			if (curr_signals.signs == sign)
			{
				input_sign = sign;
			}
		}

		// Find the width of current operator
		for(list<signals>::iterator it2 = curr_op.to.begin(); it2 != curr_op.to.end(); it2++)
		{
			signals curr_signals = *it2;
//			printf("curr op %s output signal width %d\n",curr_op.name.c_str(), it2->width);
			if(curr_op.type.find("COMP") ==curr_op.type.npos)
			{
				if(curr_signals.width > width)		width = curr_signals.width;
			}
		}
//		printf("Op sign %d width %d\n", input_sign, width);
		if(it->type == "REG" || it->type == "SHR" || it->type == "SHL")		it->signs = unsign;
		else		it->signs = input_sign;
		it->width = width;
	}
}
