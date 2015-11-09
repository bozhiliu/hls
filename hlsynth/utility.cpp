#include "utility.h"

void show_str_vec(vector<string>& sin){

    for(vector<string>::iterator it = sin.begin(); it != sin.end(); it++){
        std::cout << *it << " ";
    }
    std::cout << endl;
}