#include "stdio.h"
#include "stdlib.h"
#include <vector>
#include <string>
#include <map>

using namespace std;
enum operation_type {add, subtract, multiply, smaller, bigger, equals, evaluate, shift_right, shift_left, divide, modulo, increment, decrement};
extern map<operation_type, string> otype_map;
void initial_otype();

enum signal_type { input, output, variable} ;
extern map<signal_type, string> stype_map;
void initial_stype();

enum sign_type {sign, unsign};
enum branch_type {loop, condition};

extern vector<char> delimiter;
void initial_delimiter();

class signals;
class operation{
private:
    string name;
    unsigned int size;
    operation_type type;
    sign_type sign;
    vector<signals> from_list;
    vector<signals> to_list;
public:
    operation(string _name, operation_type _otype);
    void add_to_signal(signals to);
    void add_from_signal(signals from);
    void set_size(unsigned int _size);
    void set_sign(sign_type s);
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
    void add_from_op(operation op);
    void add_to_op(operation op);
    unsigned int get_size();
    sign_type get_sign();
};


class branch_block{
private:
    string name;
    branch_type type;
    vector<signals> from_list;
    vector<operation> bb_list;
public:
    void add_bb_list(operation op);
    
    
};