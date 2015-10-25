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



void parse_line(fstream fin, vector<signals> signals_list, vector<operation> operation_list){
    string line;
    vector<string> tokens;
    
    while(!fin.eof())
    {
        tokens.clear();
        getline(fin, line);
        tokens = tokenize(line);
        if(tokens.size() == 0)  continue;
    
        bool type_found = false;
        bool size_found = false;
        int op_found = 0;
        bool equation_found = false;
        int op_count = 0;
        signal_type curr_stype;
        sign_type curr_sign;
        operation_type curr_otype;
        unsigned int curr_width;
        
        // Found variable type and size
        for(map<signal_type, string>::iterator it = stype_map.begin(); it!= stype_map.end(); it++)
        {
            if(it->second == tokens[0])
            {
                type_found = true;
                curr_stype = it->first;
                type_found = true;
                
                if(tokens[1].find("UInt") != string::npos)  curr_sign = unsign;
                else    curr_sign = sign;
                if(curr_sign == unsign) curr_width = atoi(tokens[1].erase(0,4).c_str());
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
                    printf("Found unrecongized token: %s. Program exit.\n", it);
                    return;
                }
            }
            
            if (tokens[1] == "=")
            {
                equation_found = true;
                tokens.erase(tokens.begin()+1);
                                
                // Found operator
                for(map<operation_type, string>::iterator it = otype_map.begin(); it != otype_map.end(); it++)
                {
                    for(vector<string>::iterator it2 = tokens.begin(); it2!= tokens.end(); it2++)
                    {
                        string tmp = *it2 + *(it2+1);
                        if(tmp == it->second)   
                        {
                            curr_otype = it->first;
                            op_found = 2;
                            char count[10];
                            sprintf(count, "%d", op_count);
                            op_count ++;
                            string  name = it->second + string(count);
                            operation o_new(name, it->first);
                            o_new.set_sign(unsign);
                            operation_list.push_back(o_new);
                            tokens.erase(it2);
                            tokens.erase(it2+1);
                            break;
                        }
                        if(*it2 == it->second)
                        {
                            curr_otype = it->first;
                            op_found = 1;
                            char count[10];
                            sprintf(count, "%d", op_count);
                            op_count ++;
                            string  name = it->second + string(count);
                            operation o_new(name, it->first);
                            o_new.set_sign(unsign);
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
                            if(it->get_sign() == sign)  operation_list.back().set_sign(sign);
                            tokens.erase(it1);
                            continue;
                        }
                    }
                }
            }
        }
    }
}