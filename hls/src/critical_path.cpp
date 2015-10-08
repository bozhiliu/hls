/*
 * critical_path.cpp
 *
 *  Created on: 2015Äê10ÔÂ6ÈÕ
 *      Author: vito
 */

#include "critical_path.hpp"

float delay_table(int width, string type)
{
	int row, col;
	if (width == 1)	col = 0;
	if (width == 2)	col = 1;
	if (width == 8)	col = 2;
	if (width == 16)	col = 3;
	if (width == 32)	col = 4;
	if (width == 64)	col = 5;

	if(type == "REG")		row = 0;
	if(type == "ADD")		row = 1;
	if(type == "SUB")		row = 2;
	if(type == "MUL")		row = 3;
	if(type == "COMP>" || type == "COMP<" || type == "COMP==")		row = 4;
	if(type == "MUX")		row = 5;
	if(type == "SHR")		row = 6;
	if(type == "SHL")		row = 7;
	if(type == "DIV")		row = 8;
	if(type == "MOD")		row = 9;
	if(type == "INC")		row = 10;
	if(type == "DEC")		row = 11;

float table [12][6] = {
		 2.616,  2.644,  2.879,  3.061,  3.602,  3.966,
		 2.704,  3.713,  4.924,  5.638,  7.270,  9.566,
		 3.024,  3.412,  4.890,  5.569,  7.253,  9.566,
		 2.438,  3.651,  7.453,  7.811,  12.395,  15.354,
		 3.031,  3.934,  5.949,  6.256,  7.264,  8.416,
		 4.083,  4.115,  4.815,  5.623,  8.079,  8.766,
		 3.644,  4.007,  5.178,  6.460,  8.819,  11.095,
		 3.614,  3.980,  5.152,  6.549,  8.565,  11.220,
		 0.619,  2.144,  15.439,  33.093,  86.312,  243.233,
		 0.758,  2.149,  16.078,  35.563,  88.142,  250.583,
		 1.792,  2.218,  3.111,  3.471,  4.347,  6.200,
		 1.792,  2.218,  3.108,  3.701,  4.685,  6.503
};
	return table[row][col];
}



void visit(vertex_op &s_op, list<vertex_op> &vertex_list, list<vertex_op> &sorted_vertex )
{
	vertex_op next;
	for(list<string>::iterator it2 = s_op.to.begin();  it2 != s_op.to.end();  it2++)
	{
		for(list<vertex_op>::iterator it = vertex_list.begin();  it != vertex_list.end(); it ++)
		{
			if (it->name == *it2)
			{
				next =*it;
				if (next.status == false)		visit(*it, vertex_list, sorted_vertex);
				continue;
			}
		}
//		printf("Curr vertex %s Next vertex %s\n", s_op.name.c_str(), next.name.c_str());

	}

	if(s_op.status == false)
	{
		s_op.status = true;
		sorted_vertex.push_front(s_op);
//	printf("Adding to sorted vertex %s\n", s_op.name.c_str());
	for(list<vertex_op>::iterator it = sorted_vertex.begin(); it != sorted_vertex.end(); it++)
		{
//	printf("Name %s ", it->name.c_str());
		}
//	printf("\n");
	}
}




void topological_sort(op_list &op_list, signals_list &signals_list, list<vertex_op> &vertex_list, list<vertex_op> &sorted_vertex){
	for(list<op>::iterator it = op_list.begin(); it!=op_list.end(); it++)
	{
		vertex_op v;
		v.status = false;
		v.dist = 0;
		v.name = it->name;
		v.type = it->type;
		v.delay = delay_table(it->width, it->type);
		for(list<signals>::iterator it2 = it->from.begin();  it2 != it->from.end(); it2++)
		{
			signals curr_signals;
			for(list<signals>::iterator it3 = signals_list.begin(); it3!= signals_list.end(); it3++)
			{
				if(it3->name == it2->name)		curr_signals = *it3;
			}
	//		printf("Curr %s signal %s %d %d\n",it->name.c_str(), curr_signals.name.c_str(), curr_signals.from.size(),  curr_signals.to.size());
			for(list<op>::iterator it3 = curr_signals.from.begin(); it3 != curr_signals.from.end(); it3++)
			{
				v.from.push_back(it3->name);
			}
		}
		for(list<signals>::iterator it2 = it->to.begin();  it2 != it->to.end(); it2++)
		{
			signals curr_signals;
			for(list<signals>::iterator it3 = signals_list.begin(); it3!= signals_list.end(); it3++)
			{
				if(it3->name == it2->name)		curr_signals = *it3;
			}
			for(list<op>::iterator it3 = curr_signals.to.begin(); it3 != curr_signals.to.end(); it3++)
			{
				v.to.push_back(it3->name);
			}
		}
		vertex_list.push_back(v);
//		printf("###########################\n");
	}

	for(list<vertex_op>::iterator it = vertex_list.begin(); it != vertex_list.end(); it++)
	{
		for(list<string>::iterator it2 = it->from.begin(); it2 != it->from.end(); it2++)
		{
			for(list<vertex_op>::iterator it3 = vertex_list.begin(); it3 != vertex_list.end(); it3++ )
			{
				if (*it2 == it3->name)	it->from_op.push_back(*it3);
			}
		}
		for(list<string>::iterator it2 = it->to.begin(); it2 != it->to.end(); it2++)
		{
			for(list<vertex_op>::iterator it3 = vertex_list.begin(); it3 != vertex_list.end(); it3++ )
			{
				if (*it2 == it3->name)	it->to_op.push_back(*it3);
			}
		}
	}



	for(list<vertex_op>::iterator it = vertex_list.begin(); it != vertex_list.end();  it++)
	{
		if(it->status == false)	visit(*it, vertex_list, sorted_vertex);
	}

}



critical_pair critical_path(op_list & op_list, signals_list & signals_list, list<vertex_op>& sorted_vertex)
{
	float max = 0;
	vertex_op max_op;
	list<vertex_op> vertex_list;
	topological_sort(op_list, signals_list, vertex_list, sorted_vertex);

	for(list<vertex_op>::iterator it= sorted_vertex.begin(); it != sorted_vertex.end(); it++)
	{
		// this is starting vertex
		if (it->from.empty() == true)
			{
				it->dist = it->delay;
//				printf("Set Starting Node delay value %s  %f\n", it->name.c_str(), it->dist);
			}
		for(list<vertex_op>::iterator it2 = it->to_op.begin(); it2 != it->to_op.end(); it2++ )
		{
			for(list<vertex_op>::iterator it3 = sorted_vertex.begin(); it3 != sorted_vertex.end(); it3++)
			{
				if (it3->name == it2->name)
				{

//					printf("Curr node1 %s node2 %s\n", it->name.c_str(), it3->name.c_str());
					if(it->type == "REG")
					{
						it3->dist = it3->delay;
//						printf("Curr dist1 %g  dist2 %g\n\n", it->dist, it3->dist);
						continue;
					}

					if(it->dist + it3->delay > it3->dist)
					{
						it3->dist = it->dist + it3->delay;
						it3->pred.clear();
						it3->pred.push_back(*it);
//						printf("Curr dist1 %g  dist2 %g Pred %s\n\n", it->dist, it3->dist, it3->pred.begin()->name.c_str());
						continue;
					}


				}
			}
		}
	}



	for(list<vertex_op>::iterator it = sorted_vertex.begin(); it != sorted_vertex.end(); it++)
	{

		if(it->dist > max)
			{
				max = it->dist;
				max_op = *it;
			}
	}
	critical_pair pair;
	pair.max = max;
	pair.op = max_op;
	return pair;
}
