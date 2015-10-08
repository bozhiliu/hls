/*
 * verilog_write.cpp
 *
 *  Created on: 2015Äê10ÔÂ4ÈÕ
 *      Author: vito
 */



#include "verilog_write.hpp"
//#define DEBUG



void width_extension(ofstream& outfile, signals& from_first, list<op>::iterator it)
{
	if(from_first.width > it->width)
				{
					outfile << from_first.name << "[" << it->width -1 << ":0]";
				}
				if(from_first.width < it->width)
				{
					if(it->signs == sign)
						outfile << "{{" << -from_first.width+it->width << "{" << from_first.name << "[" << from_first.width -1 << "]}}, " << from_first.name << "[" << from_first.width-1 << ":0]}";
					if(it->signs == unsign )
						outfile << "{{" << -from_first.width+it->width << "{" << 0 << "}}, " << from_first.name << "[" << from_first.width-1 << ":0]}";
				}
				if(from_first.width == it->width)
				{
					outfile << from_first.name;
				}
}





void verilog_write(char * input_name, ofstream& outfile, signals_list& signals_list, op_list& op_list)
{
#ifdef DEBUG
	printf("Start writing!\n");
#endif
	outfile << "// Verilog generated from input netlist\n";

	outfile << "// Netlist file name: " << input_name << "\n";

	string file_name(input_name);
	size_t pos = file_name.find(".txt");
	outfile << "module  " << file_name.substr(0,pos) << "_v (\n";
	string signals_prev = "";
	string signals_name = "";


	for(list<signals>::iterator it = signals_list.begin(); it != signals_list.end(); it++)
	{
		if(it->type == "input" || it-> type == "output")
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
		if(it->type == "input")
		{
			int pos;
			 outfile << "input ";
			 if(it->signs == sign)
			 {
				outfile << " signed ";
			 }
			 if(it->width != 1)
			 {
	//			 printf("signal width %d\n", it->width);
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
		if(it->type == "output")
		{
			int pos;
			 outfile << "output ";
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
		if(it->type == "wire")
		{
			int pos;
			 outfile << "wire ";
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
	for(list<op>::iterator it = op_list.begin(); it != op_list.end(); it++)
	{
		if(it->type == "COMP>" || it->type == "COMP<" || it->type == "COMP==")
		{
			outfile << "wire " << "[" << it->width-1 << ":0] " << it->name << "_placeholder1;\n";
			outfile << "wire " << "[" << it->width-1 << ":0] " << it->name << "_placeholder2;\n";
		}
	}




   // write reg
	for(list<signals>::iterator it = signals_list.begin(); it != signals_list.end(); it++)
	{
		if(it->type == "register")
		{
			int pos;
			 outfile << "reg ";
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


	outfile << endl << endl;
	// write operators

	for(list<op>::iterator it = op_list.begin(); it!=op_list.end(); it++)
	{
//		printf("Start writing op\n");
//		printf("Curr op type %s name %s\n", it->type.c_str(), it->name.c_str());
		bool signs = it->signs;
		int width = it->width;
		string name = it->name;
		signals from_first = *(it->from.begin());
		signals to_first = *(it->to.begin());
#ifdef DEBUG
		printf("Stamp1\n");
#endif

		if(it->type == "REG")
		{
			outfile << "REG #(" << width << ") "<< name << "(.d(" << from_first.name << "), .Clk(Clk), .Rst(Rst), .q(" << to_first.name << "));" << endl;
		}

		if(it->type == "ADD" || it->type == "SUB" || it->type == "MUL" || it->type == "DIV" || it->type == "MOD")
		{
		//	std::advance(it, 1);
		//	signals second = std::advance(it->from.begin(),1);
			list<signals>::iterator ss = it->from.begin();
			std::advance(ss, 1);
			signals second = *(ss);
			if (it->signs == sign)	outfile << "S";
			outfile << it->type;
			outfile <<" #(" << width << ") " << name << "(.a(";
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
		    continue;;
		}

		if(it->type == "COMP>" || it->type == "COMP<" || it->type == "COMP==")
		{
//			printf("COMP TYPE %s\n", it->type.c_str());
			list<signals>::iterator ss = it->from.begin();
			std::advance(ss, 1);
			signals second = *(ss);
			if(it->signs == sign) outfile << "S";
			outfile << "COMP";
			outfile << " #(" << width << ") " << name << "(.a(";
			width_extension(outfile, from_first, it);
			outfile << "), b(" ;
		    width_extension(outfile, second, it);
		    outfile << "), .";
		    if(it->type.find(">") != it->type.npos)		outfile << "gt(" << to_first.name << "), .lt(" << it->name << "_placeholder1), .eq(" << it->name << "_placeholder2));" << endl ;
		    if(it->type.find("<") != it->type.npos)		outfile << "gt(" << it->name << "_placeholder1), .lt(" <<   to_first.name << "), .eq(" << it->name << "_placeholder2));" << endl ;
		    if(it->type.find("==") != it->type.npos)		outfile << "gt(" << it->name << "_placeholder1), .lt(" << it->name << "_placeholder2), .eq(" << to_first.name << "));" << endl ;
		    continue;
		}

		if(it->type == "MUX")
		{
//			printf("MUX TYPE %s\n", it->type.c_str());
			list<signals>::iterator ss = it->from.begin();
			std::advance(ss, 1);
			signals second = *(ss);
			std::advance(ss,1);
			signals third = *(ss);
			outfile << "MUX2x1 ";
			outfile << " #(" << width << ") " << name << "(.a(";
			width_extension(outfile, second, it);
			outfile << "), b(" ;
			width_extension(outfile, third, it);
			outfile << "), sel(";
			width_extension(outfile, from_first, it);
			outfile << "), .d(" << to_first.name << "));" << endl;
			continue;
		}

		if(it->type == "SHR" || it->type == "SHL")
		{
			list<signals>::iterator ss = it->from.begin();
			std::advance(ss, 1);
			signals second = *(ss);
			outfile << it->type << " #(" << width << ") " << name << "(.a(";
			width_extension(outfile, from_first, it);
			outfile << "), sh_amt(" ;
			width_extension(outfile, second, it);
			outfile << "), .d(" << to_first.name << "));" << endl;
			continue;
		}

		if(it->type == "INC" || it->type == "DEC")
		{
			if(it->signs == sign)		outfile << "S";
			outfile << it->type << " #(" << width << ") " << name << "(.a(";
			width_extension(outfile, from_first, it);
			outfile <<  "), d(" << to_first.name << "));" << endl;
			continue;
		}
	}
	// write file end
	outfile << "endmodule" << endl;

}
