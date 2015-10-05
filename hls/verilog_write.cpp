/*
 * verilog_write.cpp
 *
 *  Created on: 2015Äê10ÔÂ4ÈÕ
 *      Author: vito
 */



#include "verilog_write.hpp"


void verilog_write(char * input_name, ofstream outfile, signal_list signal_list, op_list op_list)
{
	outfile << "// Verilog generated from input netlist\n";
	outfile << "// Netlist file name: " << input_name  << "\n";
	outfile << "module" << input_name << "_v (\n";
	string signal_prev = "";
	string signal_name = "";

	for(list<signal>::iterator it = signal_list.begin(); it != signal_list.end(); it++)
	{
		if(it->type == "INPUT" || it-> type == "OUTPUT")
		{
			string signal_name = it->name;
			if(signal_prev != "")
				outfile << "    	" << signal_prev << ",\n";
			signal_prev = signal_name;
		}
	}
	outfile << "    " << signal_name << ");\n";

	for(list<signal>::iterator it = signal_list.begin(); it != signal_list.end(); it++)
	{
		if(it->type == "INPUT")
		{
			int pos;
			 outfile << "input";
			 if(it->signs == sign)
			 {
				outfile << " signed ";
			 }
			 if(it->width != 1)
			 {
				 outfile << "[" << (it->width-1) << ":0]";
				 pos = outfile.tellp();
			 }
			 while(pos < 26)
			 {
				 outfile << " ";
				 pos += 1;
			 }
			 outfile << it->name << ";\n";
		}
	}

	for(list<signal>::iterator it = signal_list.begin(); it != signal_list.end(); it++)
	{
		if(it->type == "OUTPUT")
		{
			int pos;
			 outfile << "output";
			 if(it->signs == sign)
			 {
				outfile << " signed ";
			 }
			 if(it->width != 1)
			 {
				 outfile << "[" << (it->width-1) << ":0]";
				 pos = outfile.tellp();
			 }
			 while(pos < 26)
			 {
				 outfile << " ";
				 pos += 1;
			 }
			 outfile << it->name << ";\n";
		}
	}

	for(list<signal>::iterator it = signal_list.begin(); it != signal_list.end(); it++)
	{
		if(it->type == "WIRE")
		{
			int pos;
			 outfile << "wire";
			 if(it->signs == sign)
			 {
				outfile << " signed ";
			 }
			 if(it->width != 1)
			 {
				 outfile << "[" << (it->width-1) << ":0]";
				 pos = outfile.tellp();
			 }
			 while(pos < 26)
			 {
				 outfile << " ";
				 pos += 1;
			 }
			 outfile << it->name << ";\n";
		}
	}

	for(list<signal>::iterator it = signal_list.begin(); it != signal_list.end(); it++)
	{
		if(it->type == "REG")
		{
			int pos;
			 outfile << "reg";
			 if(it->signs == sign)
			 {
				outfile << " signed ";
			 }
			 if(it->width != 1)
			 {
				 outfile << "[" << (it->width-1) << ":0]";
				 pos = outfile.tellp();
			 }
			 while(pos < 26)
			 {
				 outfile << " ";
				 pos += 1;
			 }
			 outfile << it->name << ";\n";
		}
	}

	for(list<op>::iterator it = op_list.begin(); it!=op_list.end(); it++)
	{

	}


}
