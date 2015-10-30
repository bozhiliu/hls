#include "parser.h"


vector<string> tokenize(string sin){
    string tmp = "";
    vector<string> tokens;
    bool no_append = false;
    for(unsigned int count = 0; count < sin.length(); count++)
    {
        no_append = false;
        if(sin[count] == '/' and sin[count+1] == '/')   break;
        for(vector<char>::iterator it = delimiter.begin(); it != delimiter.end(); it++)
        {
            if(sin[count] == *it)
            {
                if(tmp.length() != 0)   tokens.push_back(tmp);
                tmp = "";
                no_append = true;
                break;
            }
        }
        if (no_append == false) tmp = tmp + sin[count];
    }
    if(tmp.length()!=0) tokens.push_back(tmp);
    return tokens;
}


string int2string(int op_count){
    char count[10];
    sprintf(count, "%d", op_count);
    op_count ++;
    return string(count);
}

bool is_signal(string sin, vector<signals>& signals_list){
    for(vector<signals>::iterator it = signals_list.begin(); it!= signals_list.end(); it ++){
        if (it->get_name() == sin)  return true;
    }
    return false;
}


void parse_ordinary_equation(unsigned int op_count, vector<string>& tokens, vector<signals>& signals_list, vector<operation>& operation_list){
    
    // handle normal equations a = b X c
    if(tokens[1] == "="){
        tokens.erase(tokens.begin()+1);
        int op_found = 0;
        // Found operator
        for(map<operation_type, string>::iterator it = otype_map.begin(); it != otype_map.end(); it++)
        {
            for(vector<string>::iterator it2 = tokens.begin(); it2!= tokens.end(); it2++)
            {
                string tmp = *it2 + *(it2+1);
                if(tmp == it->second)   
                {
                    op_found = 2;
                    string  name = it->second + "_" +int2string(op_count);
                    operation o_new(name, it->first);
                    o_new.set_sign(unsigns);
                    operation_list.push_back(o_new);
                    tokens.erase(it2);
                    tokens.erase(it2+1);
                    break;
                }
                if(*it2 == it->second)
                {
                    op_found = 1;
                    string  name = it->second + "_" +int2string(op_count);
                    operation o_new(name, it->first);
                    o_new.set_sign(unsigns);
                    operation_list.push_back(o_new);
                    tokens.erase(it2);
                    break;
                }
            }
            if (op_found != 0)   break;
        }
                
        // Found to signal
        for(vector<signals>::iterator it = signals_list.begin(); it!= signals_list.end(); it++)
        {
            if(tokens[0] == it->get_name())
            {
                it->add_from_op(operation_list.back());
                operation_list.back().add_to_signal(*it);
                operation_list.back().set_size(it->get_size());
                tokens.erase(tokens.begin());
                break;
            }
        }
        // Found from signals
        for(vector<string>::iterator it1 = tokens.begin(); it1 != tokens.end(); it1++)
        {
            for(vector<signals>::iterator it = signals_list.begin(); it!= signals_list.end(); it++)
            {
                if(*it1 == it->get_name())
                {
                    it->add_to_op(operation_list.back());
                    operation_list.back().add_from_signal(*it);
                    if(it->get_sign() == signs)  operation_list.back().set_sign(signs);
                    tokens.erase(it1);
                    continue;
                }
            }
        }        
    return;
    }

    // Handle comparison statements like a<b
    if(tokens[1] == "==" || tokens[1] =="<" || tokens[1] == ">"){
        // Create internal signal for the output of evaluation
        string signal_name = "condition_out_" + int2string(op_count);
        signals s_new(signal_name, variable, 1, unsigns);
        string op_name = tokens[1] + "_" + int2string(op_count);
        operation_type op_type;
        if(tokens[1] == "==")   op_type = equals;
        if(tokens[1] == "<")    op_type = smaller;
        if(tokens[1] == ">")    op_type = bigger;
        operation o_new(op_name, op_type);
        o_new.set_sign(unsigns);
        o_new.set_size(0);
        // handle first input signal
        for(vector<signals>::iterator it = signals_list.begin(); it != signals_list.end(); it++)
        {
            if(it->get_name() == tokens[0]) { 
                o_new.add_from_signal(*it); 
                o_new.set_size(max(o_new.get_size(), it->get_size()));
                if (it->get_sign() == signs)    o_new.set_sign(signs);
                break;  }
        }
        // handle the second one
        for(vector<signals>::iterator it = signals_list.begin(); it!= signals_list.end(); it++)
        {
            if(it->get_name() == tokens[2]) { 
                o_new.add_from_signal(*it); 
                o_new.set_size(max(o_new.get_size(), it->get_size()));
                if (it->get_sign() == signs)    o_new.set_sign(signs);
                break;}
        }
        o_new.add_to_signal(s_new);
        operation_list.push_back(o_new);
        s_new.add_from_op(o_new);
        signals_list.push_back(s_new);
        return;
    }

    // Handle only single variable presented
    if(tokens.size() == 1){
        string signal_name = "condition_out_" + int2string(op_count);
        signals s_new(signal_name, variable, 1, unsigns);
        string op_name = "eval_"+int2string(op_count);
        operation o_new(op_name, equals);
        o_new.set_sign(unsigns);
        o_new.set_size(0);
        for(vector<signals>::iterator it = signals_list.begin(); it!= signals_list.end(); it++)
        {
            if(it->get_name() == tokens[0]){
                o_new.add_from_signal(*it);
                o_new.set_size(max(o_new.get_size(), it->get_size()));
                if (it->get_sign() == signs)    o_new.set_sign(signs);
                break;  }
        }
        o_new.add_to_signal(s_new);
        operation_list.push_back(o_new);
        s_new.add_from_op(o_new);
        signals_list.push_back(s_new);
        return;
    }

}

void parse_line(fstream fin, vector<signals>& signals_list, vector<operation>& operation_list, vector<branch_block>& branch_list){
    string line;
    vector<string> tokens;
    int branch_level = -1;
    int branch_level_base = 0;
    int branch_inc = 0;
    bool branch_direction = true;
    while(!fin.eof())
    {
        tokens.clear();
        getline(fin, line);
        tokens = tokenize(line);
        if(tokens.size() == 0)  continue;
    
        bool type_found = false;
        bool size_found = false;
        int op_count = 0;
        signal_type curr_stype;
        sign_type curr_sign;
        unsigned int curr_width;
    
        // Handle if statements
        if(tokens[0] == "if")
        {
            string name = "condition_" + int2string(op_count);
            branch_type type = condition;
            branch_block b_new(name, type);
            
            tokens.erase(tokens.begin());
            if(tokens.back() == "{")    tokens.erase(tokens.end());
            
            parse_ordinary_equation(op_count, tokens, signals_list, operation_list);
            b_new.add_from_list(operation_list.back());
            branch_list.push_back(b_new);
            branch_level ++;
            branch_inc ++;
            branch_direction = true;
        }
    
        // Handle for loops
        if(tokens[0] == "for")
        {
            string name = "loop_" + int2string(op_count);
            branch_type type = loop;
            branch_block b_new(name, type);
            
            // Seperate tokens to single statements
            tokens.erase(tokens.begin());
            if(tokens.back() == "{")    tokens.erase(tokens.end());
            vector<string> tmp;
            while(!tokens.empty()){
                tmp.push_back(tokens[0]);
                tokens.erase(tokens.begin());
                if (is_signal(tmp.back(), signals_list)){
                    parse_ordinary_equation(op_count, tmp, signals_list, operation_list);
                    b_new.add_from_list(operation_list.back());
                    tmp.clear();
                }
            }
            
            
            branch_list.push_back(b_new);
            branch_level ++;
            branch_inc ++;
            branch_direction = true;
            
        }

        if(tokens[0] == "else")
        {
            branch_direction = false;
        }

        if(tokens[0] == "}" || tokens.back() == "}")
        {
            branch_level --;
            branch_direction = true;
            if(branch_level == -1)  { branch_level_base = branch_inc;   branch_inc = 0;}
        }


    
        // Found variable type and size
        for(map<signal_type, string>::iterator it = stype_map.begin(); it!= stype_map.end(); it++)
        {
            if(it->second == tokens[0])
            {
                type_found = true;
                curr_stype = it->first;
                type_found = true;
                
                if(tokens[1].find("UInt") != string::npos)  curr_sign = unsigns;
                else    curr_sign = signs;
                if(curr_sign == unsigns) curr_width = atoi(tokens[1].erase(0,4).c_str());
                else curr_width = atoi(tokens[1].erase(0,3).c_str());
                size_found = true;
                
                break;
            }
        }
        
        // Found variable names
        if(type_found == true && size_found== true)
        {
            for(vector<string>::iterator it = tokens.begin()+2; it != tokens.end(); it++)
            {
                signals s_new = signals(*it, curr_stype, curr_width, curr_sign);
                signals_list.push_back(s_new);
            }
            continue;
        }
        
        
        // Found equation
        if(type_found == false && size_found == false)
        {
            // handle unrecongnized token
            for(vector<string>::iterator it = tokens.begin(); it!= tokens.end(); it++)
            {
                bool defined = false;
                for(vector<signals>::iterator it1 = signals_list.begin(); it1 != signals_list.end(); it1++)
                {
                    if(it1->get_name() == *it)  
                    {
                        defined = true;
                        break;
                    }
                }
                for(map<operation_type, string>::iterator it1 = otype_map.begin(); it1 != otype_map.end(); it1++)
                {
                    if(it1->second == *it)
                    {
                        defined = true;
                        break;
                    }
                }
                if(defined == false)    
                {
                    //printf("Found unrecongized token: %s. Program exit.\n", *it->c_str());
                    cout << *it << endl;
                    return;
                }
            }
            
            
            //Parse ordinary equation
            parse_ordinary_equation(op_count, tokens, signals_list, operation_list);
            if(branch_level_base + branch_level != -1)  branch_list[branch_level].add_bb_list(make_pair(operation_list.back(), branch_direction));
        }
    }
}