#ifndef _SCHEDULING
#define _SCHEDULING

#include <algorithm>
#include "data_type.h"


class sequence_graph{
private:
    vector<node> s_graph;
    vector<unsigned int> asap;
    vector<unsigned int> alap;
    vector<unsigned int> force;
    
public:
    void create_sequence_graph(vector<signals>& signals_list, vector<operation>& operation_list, vector<branch_block>& branch_list);
    void asap_schedule();
    void alap_schedule(unsigned int bound);
    void force_directed_schedule(unsigned int bound);
	void add_node_to_list(unsigned int _n1, unsigned int _n2, bool type);
	long get_distance(node nin);
};









#endif