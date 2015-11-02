#ifndef _SCHEDULING
#define _SCHEDULING


#include "data_type.h"

class sequence_graph{
private:
    vector<node> s_graph;
    vector<unsigned int> asap;
    vector<unsigned int> alap;
    
public:
    void create_sequence_graph(vector<signals>& signals_list, vector<operation>& operation_list, vector<branch_block>& branch_list);
    void asap_schedule();
    void alap_schedule(unsigned int bound);

};









#endif