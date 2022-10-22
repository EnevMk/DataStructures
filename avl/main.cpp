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
    calcio.insert(11, "Napoli");
    calcio.insert(14, "Udinese");
    std::cout << "----\n";
    calcio.erase(3);
    //calcio.erase(7);
    //calcio.erase(10);
    
    std::cout << calcio.find(9)->container.front().second << '\n';
    std::cout << calcio.count(19) << '\n';

    auto v = calcio.inorder_traversal();

    /* for (auto &team: v) {

        std::cout << team.second << '\n';
    } */
    
    for (auto it = calcio.begin(); it != calcio.end(); ++it) {
        
        std::cout << it->second << ' ';
    }
    /* auto it = calcio.begin();
    std::cout << (++++++++++++++it != calcio.end());
    std::cout << it->second; */
}