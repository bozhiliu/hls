#include "scheduling.h"

// set type to true to add to from list, false to add to to list
void add_node_to_list(node& n1, node& n2, bool type){
    if (type == true){
        for(vector<node>::iterator it = n1.get_from_list().begin(); it != n1.get_from_list().end(); it++){
            if (it->get_name().compare(n2.get_name()))  return;
        }
        n1.get_from_list().push_back(n2);
    }
    
    if(type == false){
        for(vector<node>::iterator it = n1.get_to_list().begin(); it != n1.get_to_list().end(); it++){
            if (it->get_name() == n2.get_name())   return;
        }
        n1.get_to_list().push_back(n2);
    }
}


void sequence_graph::create_sequence_graph(vector<signals>& signals_list, vector<operation>& operation_list, vector<branch_block>& branch_list){
    node source_node;
    source_node.set_name("source_node");
    source_node.set_type(nop);
    source_node.set_latency(0);
    s_graph.push_back(source_node);
    
    for(vector<operation>::iterator it = operation_list.begin(); it!= operation_list.end(); it++){
        node n_node;
        n_node.set_name(it->get_name());
        n_node.set_type(it->get_type());
        n_node.set_latency(1);
        if(it->get_type() == multiply)  n_node.set_latency(2);
        if(it->get_type() == divide || it->get_type() == modulo)    n_node.set_latency(3);
        s_graph.push_back(n_node);
        it->set_node(&n_node);
    }
    
    for(vector<operation>::iterator it = operation_list.begin(); it!= operation_list.end(); it++)
    {
        vector<signals> s_from_list = it->get_from_list();
        vector<signals> s_to_list = it->get_to_list();
        
        // Add from list for a given node
        for(vector<signals>::iterator it2 = s_from_list.begin(); it2 != s_from_list.end(); it2++){
            vector<operation> op_from_list = it2->get_from_list();
            for(vector<operation>::iterator it3 = op_from_list.begin(); it3 != op_from_list.end(); it3++){
                add_node_to_list(it->get_node(), it3->get_node(), true);
            }
        }
        
        // Add to list for a given node
        for(vector<signals>::iterator it2 = s_to_list.begin(); it2!= s_to_list.end(); it2++){
            vector<operation> op_to_list = it2->get_to_list();
            for(vector<operation>::iterator it3 = op_to_list.begin(); it3!=op_to_list.end(); it3++){
                add_node_to_list(it->get_node(), it3->get_node(), false);
            }
        }
    }
}


void sequence_graph::asap_schedule(){
    asap.resize(s_graph.size(), 0);
    int size_count = s_graph.size();
    for(vector<node>::iterator it = s_graph.begin(); it != s_graph.end(); it++){
        vector<node> from_tmp = it->get_from_list();
        if(from_tmp.size() == 0)    asap[distance(s_graph.begin(), it)] = 1;
        size_count --;
    }
    
    while(size_count != 0)
    {
        for(vector<node>::iterator it = s_graph.begin(); it!= s_graph.end(); it++){
            pos = distance(s_graph.begin(), it);
            if(asap[pos] == 0){
                bool ready = true;
                int max_time = 0;
                // check if from predecessor has all completed
                vector<node> from_tmp = it->get_from_list();
                for(vector<node>::iterator it2 = from_tmp.begin(); it2 != from_tmp.end(); it2++){
                    pos2 = distance(s_graph.begin(), it2);
                    if(asap[pos2] == 0) { ready = false;    break;}
                    else {  max_time = max(max_time, asap[pos2] + it->get_latency()); }
                }
                if(ready == true){  asap[pos]   = max_time; size_count --;}
            }
        }
    }
}