#include "data_type.h"


//string delimiter[] ={" ", ";", ","};
vector<char> delimiter;
void initial_delimiter(){
    delimiter.push_back(' ');
    delimiter.push_back(';');
    delimiter.push_back(',');
    delimiter.push_back('?');
    delimiter.push_back('(');
    delimiter.push_back(')');
}

map<signal_type, string> stype_map;
void initial_stype(){
    stype_map.insert(pair<signal_type, string>(input, "input"));
    stype_map.insert(pair<signal_type, string>(output, "output"));
    stype_map.insert(pair<signal_type, string>(variable, "variable"));
}

map<operation_type, string> otype_map;
void initial_otype(){
    otype_map.insert(pair<operation_type, string>(add, "+"));
    otype_map.insert(pair<operation_type, string>(subtract, "-"));
    otype_map.insert(pair<operation_type, string>(multiply, "*"));
    otype_map.insert(pair<operation_type, string>(smaller, "<"));
    otype_map.insert(pair<operation_type, string>(bigger, ">"));
    otype_map.insert(pair<operation_type, string>(equals, "=="));
    otype_map.insert(pair<operation_type, string>(evaluate, "?"));
    otype_map.insert(pair<operation_type, string>(shift_right, ">>"));
    otype_map.insert(pair<operation_type, string>(shift_left, "<<"));
    otype_map.insert(pair<operation_type, string>(divide, "/"));
    otype_map.insert(pair<operation_type, string>(modulo, "%"));
    otype_map.insert(pair<operation_type, string>(increment, "+1"));
    otype_map.insert(pair<operation_type, string>(decrement, "-1"));
}


operation::operation(string _name, operation_type _otype)
{
    name = _name;
    type = _otype;
}


void operation::add_to_signal(signals& to){
    to_list.push_back(to);
}

void operation::add_from_signal(signals& from){
    from_list.push_back(from);
}

void operation::set_size(unsigned int _size){
    size = _size;
}

void operation::set_sign(sign_type s){
    sign = s;
}

void operation::set_type(operation_type _type){
    this->type = _type;
}

unsigned int operation::get_size(){
    return size;
}

string operation::get_name(){
    return name;
}

operation_type operation::get_type(){
    return type;
}

void operation::set_node(node *_node){
    s_node = _node;
}


vector<signals>& operation::get_from_list(){
    return from_list;
}

vector<signals>& operation::get_to_list(){
    return to_list;
}

node& operation::get_node(){
    return *s_node;
}

signals::signals(string _name, signal_type _type, unsigned int _size, sign_type _sign){
    name = _name;
    size = _size;
    type = _type;
    sign = _sign;
}

string signals::get_name(){
    return this->name;
}

void signals::add_from_op(operation& op){
    from_list.push_back(op);
}

void signals::add_to_op(operation& op){
    to_list.push_back(op);
}

unsigned int signals::get_size(){
    return size;
}

sign_type signals::get_sign(){
    return sign;
}

vector<operation>& signals::get_from_list(){
    return from_list;
} 

vector<operation>& signals::get_to_list(){
    return to_list;
}


branch_block::branch_block(string _name, branch_type _type){
    name = _name;
    type = _type;
}



void branch_block::add_bb_list(pair<operation, bool>& p_in){
    bb_list.push_back(p_in);
}

void branch_block::add_from_list(operation& sin){
    from_list.push_back(sin);
}


void node::set_name(string _name){

    this->op_name = _name;
}

void node::set_type(operation_type _type){
    type = _type;
}

void node::set_latency(unsigned int _l){
    latency = _l;
}

void node::add_to_list(node& nin){
    this->to_list.push_back(nin);
}

void node::add_from_list(node& nin){
    this->from_list.push_back(nin);
}

vector<node>& node::get_from_list(){
    return from_list;
}

vector<node>& node::get_to_list(){
    return to_list;
}

string node::get_name(){
    return op_name;
}

unsigned int node::get_latency(){
    return latency;
}