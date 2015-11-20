#include "scheduling.h"

// set type to true to add n2 to n1's "from list", false to add n2 to n1's "to list"
void sequence_graph::add_node_to_list(unsigned int _n1, unsigned int _n2, bool type){
	node n1 = s_graph.at(_n1);
	node n2 = s_graph.at(_n2);
	if (type == true) {
		if (n1.get_from_list().size() != 0) {
			vector<node*> from_list = n1.get_from_list();
			for (vector<node*>::iterator it = (from_list.begin()); it != (from_list.end()); it++) {
				if ((*it)->get_name().compare(n2.get_name()))  return;
			}
		}
        s_graph[_n1].add_from_list(&s_graph.at(_n2));
    }
    
    if(type == false){
		if (n1.get_to_list().size() != 0) {
			vector<node*> to_list = n2.get_to_list();
			for (vector<node*>::iterator it = to_list.begin(); it != to_list.end(); it++) {
				if ((*it)->get_name() == n2.get_name())   return;
			}
		}
        s_graph[_n1].add_to_list(&s_graph.at(_n2));
    }
}


long sequence_graph::get_distance(node nin) {
	long result = -1;
	for (vector<node>::iterator it = s_graph.begin(); it != s_graph.end(); it++) {
		if (nin.get_name() == (*it).get_name())	result = distance(s_graph.begin(), it);
	}
	return result;
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
        it->set_node(s_graph.size()-1);
    }
    
	node sink_node;
	sink_node.set_name("sink_node");
	sink_node.set_type(nop);
	sink_node.set_latency(0);
	s_graph.push_back(sink_node);

    for(vector<operation>::iterator it = operation_list.begin(); it!= operation_list.end(); it++)
    {
        vector<signals*> s_from_list = it->get_from_list();
        vector<signals*> s_to_list = it->get_to_list();
        
        // Add from list for a given node
        for(vector<signals*>::iterator it2 = s_from_list.begin(); it2 != s_from_list.end(); it2++){
            vector<operation*> op_from_list = (*it2)->get_from_list();
			if (op_from_list.size() == 0)
			{
				add_node_to_list(it->get_node(), 0, true);
				add_node_to_list(0, it->get_node(), false);
			}
			for(vector<operation*>::iterator it3 = op_from_list.begin(); it3 != op_from_list.end(); it3++){
                add_node_to_list(it->get_node(), (*it3)->get_node(), true);
            }
        }
        
        // Add to list for a given node
        for(vector<signals*>::iterator it2 = s_to_list.begin(); it2!= s_to_list.end(); it2++){
            vector<operation*> op_to_list = (*it2)->get_to_list();
			if (op_to_list.size() == 0)
			{
				add_node_to_list(it->get_node(), s_graph.size() - 1, false);
				add_node_to_list(s_graph.size()-1, it->get_node(), true);
			}
			for (vector<operation*>::iterator it3 = op_to_list.begin(); it3 != op_to_list.end(); it3++) {
                add_node_to_list(it->get_node(), (*it3)->get_node(), false);
            }
        }
    }
}


void sequence_graph::asap_schedule(){
    asap.resize(s_graph.size(), 0);
    long size_count = s_graph.size();
	for (vector<node>::iterator it = s_graph.begin(); it != s_graph.end(); it++) {
		vector<node*> from_tmp = it->get_from_list();
		bool source_node_flag = true;
		for(vector<node*>::iterator it1 = from_tmp.begin(); it1 != from_tmp.end(); it1++)
		{
			if ((*it1)->get_name() != "source_node")	source_node_flag = false;
		}
		if (from_tmp.size() == 0)
		{
			source_node_flag = false;
			size_count--;
		}	
		if (source_node_flag == true)
		{
			asap[distance(s_graph.begin(), it)] = 1;
			size_count--;
		}
	}
    
    while(size_count != -1)
    {
		for (vector<node>::iterator it = s_graph.begin(); it != s_graph.end(); it++) {
			long pos = distance(s_graph.begin(), it);
			if (asap[pos] == 0) {
				bool ready = true;
				unsigned int max_time = 0;
				// check if from predecessor has all completed
				vector<node*> from_tmp = it->get_from_list();
				for (vector<node*>::iterator it2 = from_tmp.begin(); it2 != from_tmp.end(); it2++) {
					long pos2 = get_distance(*(*it2));
					if (asap[pos2] == 0) { ready = false;    break; }
					else { max_time = std::max(max_time, asap[pos2] + (*it2)->get_latency()); }
				}
				if (ready == true) { asap[pos] = max_time; size_count--; }
			}

		}
    }
}


void sequence_graph::alap_schedule(unsigned int bound){
    alap.resize(s_graph.size(),0);
    unsigned long size_count = s_graph.size();
    for(vector<node>::iterator it = s_graph.begin(); it != s_graph.end(); it++){
        vector<node*> to_tmp = it->get_to_list();
        if(to_tmp.size() == 0) { alap[distance(s_graph.begin(),it)] = bound + 1 - it->get_latency();
            size_count -- ;}
    }
    
    while (size_count != 0){
        for(vector<node>::iterator it = s_graph.begin(); it != s_graph.end(); it++){
            long pos = distance(s_graph.begin(), it);
            if (alap[pos] == 0){
                bool ready = true;
                unsigned int min_time = bound;
                // check if successor has all been scheduled
                vector<node*> to_tmp = it->get_from_list();
                for(vector<node*>::iterator it2 = to_tmp.begin(); it2 != to_tmp.end(); it2++){
                    long pos2 = get_distance(**it2);
                    if(alap[pos2] == 0) {   ready = false; break;}
                    else    {   min_time = std::min(min_time, alap[pos2] - it->get_latency());}
                }
                if(ready == true)   {alap[pos] = min_time; size_count -- ;}
            }
        }
    }
}



void sequence_graph::force_directed_schedule(unsigned int bound){
    
    force.resize(asap.size(), 0);
    
    map<operation_type, vector<float> > probability;
    for(map<operation_type,string>::iterator it = otype_map.begin(); it != otype_map.end(); it++){
        vector<float> array;
        array.resize(bound, 0);
        pair<operation_type, vector<float> > pair_n(it->first, array);
        probability.insert(pair_n);
    }
    
    unsigned int size_count = asap.size();
    
    while( size_count != 0)
    {
        // Calculate type distribution
        for(unsigned int index=0; index < asap.size(); index ++){
            operation_type curr_type = s_graph[index].get_type();
            if(force[index] != 0)
            {
                probability[curr_type][force[index]] += 1;
                continue;
            }
            unsigned int left = asap[index];
            unsigned int right = alap[index];
            float prob = 1 / (right + 1 - left);
            for(unsigned int it = left-1; it != right; it++){
                probability[curr_type][it] += prob;
            }
        }
        
        // Calculate total force for all nodes
        // Latency

        float iteration_max_force = 0;
        unsigned int iteration_schedule = 0;
        unsigned int iteration_index = 0;
        for(vector<node>::iterator it = s_graph.begin(); it != s_graph.end(); it++){
            int pos  = distance(s_graph.begin(), it);
            if(force[pos] != 0)     continue;
            
            vector<float> self_force;
            vector<float> other_force;
            vector<float> total_force;
            unsigned int left = asap[distance(s_graph.begin(), it)];
            unsigned int right = alap[distance(s_graph.begin(), it)];
            // Modified interval
            
            unsigned int interval = right-left + 1;
            unsigned int sweeper = left-1;
            float prob = 1 / interval;
            self_force.resize(interval, 0); other_force.resize(interval,0); total_force.resize(interval,0);
            operation_type curr_type = it->get_type();
            for( unsigned int index = left-1; index < right; index ++){
                
                // Calculate self force
                float self = 0;
                while(sweeper < interval){
                    if(sweeper == index)    self += (1-prob)*probability[curr_type][index];
                    else    self += (0-prob)*probability[curr_type][index];
                    sweeper ++;
                }
                self_force[index - left +1] = self;
                
                // Calculate predecessor force
                float pred = 0;
                for(vector<node*>::iterator it1 = it->get_from_list().begin(); it1 != it->get_from_list().end(); it1++){
                    int pred_pos = get_distance(**it1);
                    if(force[pred_pos] != 0)    continue;
                    unsigned int pred_left = asap[get_distance(**it1)];
                    unsigned int pred_right = alap[get_distance(**it1)];
                    unsigned int pred_right_n = min(index+1-(*it1)->get_latency(), pred_right);
                    unsigned int pred_interval = pred_right - pred_left +1;
                    operation_type pred_type = (*it1)->get_type();
                    if(pred_left == pred_right_n){
                        unsigned int pred_sweeper = pred_left-1;
                        while(pred_sweeper < pred_right){
                            if(pred_sweeper == pred_left-1)   pred += (1-1/pred_interval)*probability[pred_type][pred_sweeper];
                            else pred += (0 - 1/pred_interval)*probability[pred_type][pred_sweeper];
                            pred_sweeper ++;
                        }
                    }
                }
                
                // Calculate successor force
                float suc = 0;
                for(vector<node*>::iterator it1 = it->get_to_list().begin(); it1 != it->get_to_list().end(); it1++){
                    int suc_pos = get_distance(**it1);
                    if(force[suc_pos] != 0)     continue;
                    unsigned int suc_left = asap[get_distance(**it1)];
                    unsigned int suc_right = alap[get_distance(**it1)];
                    unsigned int suc_left_n = max(index+1+(*it1)->get_latency(), suc_left);
                    unsigned int suc_interval = suc_right - suc_left +1;
                    operation_type suc_type = (*it1)->get_type();
                    if(suc_right == suc_left_n){
                        unsigned int suc_sweeper = suc_left-1;
                        while(suc_sweeper < suc_right){
                            if(suc_sweeper == suc_right-1)   suc += (1-1/suc_interval)*probability[suc_type][suc_sweeper];
                            else pred += (0 - 1/suc_interval)*probability[suc_type][suc_sweeper];
                            suc_sweeper ++;
                        }
                    }
                }
                other_force[index - left + 1] = suc + pred;
                total_force[index - left + 1] = other_force[index-left+1] + self_force[index-left+1];
            }
            
            // Find the maximum tot_force amoung all possibilities and store the force_value/node/schedule_time to outside
            
            float force_max = 0;
            unsigned int schedule = 0;
            for(vector<float>::iterator it = total_force.begin(); it != total_force.end(); it++){
                if(*it > force_max) { force_max = *it;  schedule = left + distance(total_force.begin(), it); } 
            }
           if(force_max > iteration_max_force)  {
               iteration_max_force = force_max;
               iteration_index = pos;
               iteration_schedule = schedule;
           }
        }
        
        // Update the schedule
        force[iteration_index] = iteration_schedule;
        size_count --;
        
        // Clear the distribution graph
        probability.clear();
        for(map<operation_type,string>::iterator it = otype_map.begin(); it != otype_map.end(); it++){
            vector<float> array;
            array.resize(bound, 0);
            pair<operation_type, vector<float> > pair_n(it->first, array);
            probability.insert(pair_n);
        }
    }
    
}








