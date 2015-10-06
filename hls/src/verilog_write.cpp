/*
 * verilog_write.cpp
 *
 *  Created on: 2015Äê10ÔÂ4ÈÕ
 *      Author: vito
 */



#include "verilog_write.hpp"

void width_extension(ofstream& outfile, signals from_first, list<op>::iterator it)
{
	if(from_first.width > it->width)
				{
					outfile << from_first.name << "[" << it->width -1 << ":0]";
				}
				if(from_first.width < it->width)
				{
					if(it->signs == unsign)
						outfile << "{{" << from_first.width-it->width << "{" << from_first.name << "[" << from_first.width -1 << "]}}, " << from_first.name << "[" << from_first.width-1 << ":0]}";
					if(it->signs == sign )
						outfile << "{{" << from_first.width-it->width << "{" << 0 << "}}, " << from_first.name << "[" << from_first.width-1 << ":0]}";
				}
				if(from_first.width == it->width)
				{
					outfile << from_first.name;
				}
}





void verilog_write(char * input_name, ofstream& outfile, signals_list signals_list, op_list op_list)
{
	outfile << "// Verilog generated from input netlist\n";
	outfile << "// Netlist file name: " << input_name  << "\n";
	outfile << "module" << input_name << "_v (\n";
	string signals_prev = "";
	string signals_name = "";

	for(list<signals>::iterator it = signals_list.begin(); it != signals_list.end(); it++)
	{
		if(it->type == "INPUT" || it-> type == "OUTPUT")
		{
			string signals_name = it->name;
			if(signals_prev != "")
				outfile << "    	" << signals_prev << ",\n";
			signals_prev = signals_name;
		}
	}
	outfile << "    " << signals_name << ");\n";
     // write input
	for(list<signals>::iterator it = signals_list.begin(); it != signals_list.end(); it++)
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
    // write output
	for(list<signals>::iterator it = signals_list.begin(); it != signals_list.end(); it++)
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
    // write wire
	for(list<signals>::iterator it = signals_list.begin(); it != signals_list.end(); it++)
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
   // write reg
	for(list<signals>::iterator it = signals_list.begin(); it != signals_list.end(); it++)
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
	// write operators
	for(list<op>::iterator it = op_list.begin(); it!=op_list.end(); it++)
	{
		bool signs = it->signs;
		int width = it->width;
		string name = it->name;
		signals from_first = *(it->from.begin());
		signals to_first = *(it->to.begin());

		if(it->type == "REG")
		{
			outfile << "REG #(" << width << ") "<< name << "(.d(" << from_first.name << "), .Clk(Clk), .Rst(Rst), .q(" << to_first.name << "));" << endl;
		}

		if(it->type == "ADD" || it->type == "SUB" || it->type == "MUL" || it->type == "DIV" || it->type == "MOD")
		{
			signals second = *(it->from.begin() ++);
			if (it->signs == sign)	outfile << "S";
			outfile << it->type;
			outfile <<" #(" << width << ") " << name << "(.a(";
/*
			if(from_first.width > it->width)
			{
				outfile << from_first.name << "[" << it->width -1 << ":0]";
			}
			if(from_first.width < it->width)
			{
				outfile << "{{" << from_first.width-it->width << "{" << from_first.name << "[" << from_first.width -1 << "]}}, " << from_first.name << "[" << from_first.width-1 << ":0]}";
			}
			if(from_first.width == it->width)
			{
				outfile << from_first.name;
			}
*/
			width_extension(outfile, from_first, it);
		    outfile << "), b(" ;
		    width_extension(outfile, second, it);
		    outfile << "), .";
		    if(it->type == "ADD")	outfile << "sum";
		    if(it->type == "SUB")		outfile << "diff";
		    if(it->type == "MUL") 	outfile << "prod";
		    if(it->type == "DIV") 		outfile << "quot";
		    if(it->type == "MOD") 	outfile << "rem";
		    outfile << "(" << to_first.name <<"));" <<endl;
		}

		if(it->type.find("COMP"))
		{
			signals second = *(it->from.begin()++);
			if(it->signs == sign) outfile << "S";
			outfile << "COMP";
			outfile << " #(" << width << ") " << name << "(.a(";
			width_extension(outfile, from_first, it);
			outfile << "), b(" ;
		    width_extension(outfile, second, it);
		    outfile << "), .";
		    if(it->type.find(">"))		outfile << "gt(" << to_first.name << "), .lt(), .eq());" << endl ;
		    if(it->type.find("<"))		outfile << "gt(), .lt(" <<   to_first.name << "), .eq());" << endl ;
		    if(it->type.find("=="))		outfile << "gt(), .lt(), .eq(" << to_first.name << "));" << endl ;
		}

		if(it->type == "MUX")
		{
			signals second = *(it->from.begin() ++);
			signals third = *(it->from.begin() ++ ++);
			outfile << "MUX2x1 ";
			outfile << " #(" << width << ") " << name << "(.a(";
			width_extension(outfile, second, it);
			outfile << "), b(" ;
			width_extension(outfile, third, it);
			outfile << "), sel(";
			width_extension(outfile, from_first, it);
			outfile << "), .d(" << to_first.name << "));" << endl;

		}

		if(it->type == "SHR" || it->type == "SHL")
		{
			signals second =*( it->from.begin() ++);
			outfile << it->type << " #(" << width << ") " << name << "(.a(";
			width_extension(outfile, from_first, it);
			outfile << "), sh_amt(" ;
			width_extension(outfile, second, it);
			outfile << "), .d(" << to_first.name << "));" << endl;
		}

		if(it->type == "INC" || it->type == "DEC")
		{
			if(it->signs == sign)		outfile << "S";
			outfile << it->type << " #(" << width << ") " << name << "(.a(";
			width_extension(outfile, from_first, it);
			outfile <<  "), d(" << to_first.name << "));" << endl;
		}
	}
	// write file end
	outfile << "endmodule" << endl;

}
