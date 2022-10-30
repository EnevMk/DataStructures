#include <iostream>
#include <list>

#include "avl.hpp"

using namespace std;

int main() {   
    
    list<std::pair<int, int>> l;

    std::cout << sizeof(avl_tree<int, int>::node*);
    return 0;
}