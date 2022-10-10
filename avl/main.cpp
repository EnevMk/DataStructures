#include <iostream>
#include <string>
#include "avl.hpp"

using std::string;

int main() {

    avl_tree<int, string> calcio;

    calcio.insert(19, "Milan");
    calcio.insert(19, "Inter");
    calcio.insert(36, "Juventus");
    calcio.insert(7, "Bologna");
    calcio.insert(3, "Roma");
    calcio.insert(9, "Genoa");
    calcio.insert(10, "Napoli");

    calcio.erase(3);
    calcio.erase(7);
    calcio.erase(10);
    
    //calcio.insert(2, "Napoli");
    //std::cout << reinterpret_cast<uintptr_t>(nullptr);
    std::cout << calcio.balance_factor(calcio.find(9)) << '\n';
    //std::cout << calcio.unmask(calcio.root)->container.front().second << '\n';
    //std::cout << calcio.unmask(calcio.unmask(calcio.root)->left)->container.front().second << '\n';
    /* std::cout << sizeof(int)<< '\n';


    int p = 619;
    avl_tree<int, string>::node *pp = new avl_tree<int, string>::node{ std::list<pair<const int, string>>(1, std::make_pair(7, "Genoa")) };
    std::cout << pp << '\n';
    uintptr_t cpp = reinterpret_cast<uintptr_t>(pp);
    std::cout << cpp << '\n';
    cpp |= 1;
    pp = reinterpret_cast<avl_tree<int, string>::node*>(cpp);
    
    std::cout << pp << '\n';
    std::cout << cpp << '\n';
    //cpp |= 3;
    std::cout << uintptr_t(cpp & 3) << '\n';
    //std::cout << cpp << '\n';
    
    pp = reinterpret_cast<avl_tree<int, string>::node*> (cpp &~ 3);
    std::cout << pp << '\n';
    delete pp; */
    
    /* calcio.insert(7, "Sampodoria");
    calcio.insert(0, "Salernitana"); */
    //calcio.erase("Milan");
    //std::cout << (calcio.find("Milan") != nullptr) << '\n';
}