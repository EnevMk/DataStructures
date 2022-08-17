#include <iostream>
#include <string>
#include <map>

#include "avl.hpp"

int main() {

    std::multimap<int, std::string> stl;

    //stl.insert()

    avl_tree<int, std::string> tr;
    
    tr.insert(3, "Roma");
    tr.insert(18, "Inter");
    tr.insert(19, "Milan");
    tr.insert(36, "Juve");
    tr.insert(9, "Genoa");
    tr.insert(2, "Fiorentina");
    tr.insert(7, "Bologna");

    //tr.erase(3);

    //tr.insert(1, "Napoli");

    auto vec = tr.inorder_traversal();

    for (auto x: vec) {
        std::cout << x << '\n';
    }

    std::cout << '\n' << tr.height() << '\n' << tr.size();

    auto it = tr.begin();
    std::cout << (*it).second;
    ++it;
    std::cout << (*it).second;
    ++it;
    std::cout << it->second;
    ++it;


    std::cout << "\n\n";
    auto iter = tr.find(19);

    if (iter != tr.end()) {
        std::cout << iter->second << '\n';
    }
    /* std::cout << (*it).data;
    ++it;
    std::cout << (*it).data;
    ++it;
    std::cout << (*it).data;
    ++it;
    std::cout << (*it).data; */

    return 0;
}