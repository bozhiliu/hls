#ifndef _DATA_TYPE
#define _DATA_TYPE

#include "stdio.h"
#include "stdlib.h"
#include <vector>
#include <string>
#include <map>

using namespace std;
enum operation_type {add, subtract, multiply, smaller, bigger, equals, evaluate, shift_right, shift_left, divide, modulo, increment, decrement, nop};
extern map<operation_type, string> otype_map;
void initial_otype();

enum signal_type { input, output, variable} ;
extern map<signal_type, string> stype_map;
void initial_stype();

enum sign_type {signs, unsigns};
enum branch_type {loop, condition};

extern vector<char> delimiter;
void initial_delimiter();

class signals;
class node;
class operation{
private:
    string name;
    unsigned int size;
    operation_type type;
    sign_type sign;
    vector<signals> from_list;
    vector<signals> to_list;
    node *s_node;
public:
    operation(string _name, operation_type _otype);
    void add_to_signal(signals& to);
    void add_from_signal(signals& from);
    void set_size(unsigned int _size);
    void set_sign(sign_type s);
    void set_type(operation_type _type);
    unsigned int get_size();
    string get_name();
    operation_type get_type();
    void set_node(node * _node);
    vector<signals>& get_from_list();
    vector<signals>& get_to_list();
    node& get_node();
};


class signals{
private:
    string name;
    unsigned int size;
    signal_type type;
    sign_type sign;
    vector<operation> from_list;
    vector<operation> to_list;
public:
    signals(string _name, signal_type _type, unsigned int _size, sign_type _sign); 
    string get_name();
    void add_from_op(operation& op);
    void add_to_op(operation& op);
    unsigned int get_size();
    sign_type get_sign();
    vector<operation>& get_from_list();
    vector<operation>& get_to_list();
};


class branch_block{
private:
    string name;
    branch_type type;
    vector<operation> from_list;
    vector<pair<operation,bool> > bb_list;
public:
    branch_block(string _name, branch_type _type);
    void add_bb_list(pair<operation, bool>& p_in);
    void add_from_list(operation& op);
};


class node{
private:
    operation_type type;
    string op_name;
    unsigned int latency;
    vector<node> from_list;
    vector<node> to_list;
public:
    void set_type(operation_type _type);
    void set_name(string _name);
    void set_latency(unsigned int _l);
    void add_from_list(node& nin);
    void add_to_list(node& nin);
    vector<node>& get_from_list();
    vector<node>& get_to_list();
    string get_name();
    unsigned int get_latency();
    operation_type get_type();
};




#endif