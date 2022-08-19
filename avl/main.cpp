#include <iostream>
#include <string>
#include <map>

#include "avl.hpp"

int main() {

    /* std::multimap<int, std::string> stl;

    stl.insert(std::make_pair(3, "Roma"));
    stl.insert(std::make_pair(3, "Roma"));

    stl.erase(3);

    std::cout << stl.empty();

    auto it = stl.equal_range(3);

    for (auto iter = it.first; iter != it.second; ++iter) {
        std::cout << iter->second << ' ';
    } */

    //stl.insert()

    avl_tree<int, std::string> tr;
    
    tr.insert(3, "Roma");
    tr.insert(18, "Inter");
    tr.insert(19, "Milan");
    tr.insert(36, "Juve");
    tr.insert(9, "Genoa");
    tr.insert(2, "Fiorentina");
    tr.insert(2, "Napoli");
    tr.insert(7, "Bologna");

    //tr.erase(2);

    //tr.insert(1, "Napoli");

    /* auto vec = tr.inorder_traversal();

    for (auto x: vec) {
        std::cout << x.second << '\n';
    } */

    std::cout << '\n' << tr.height() << '\n' << tr.size();

    /* auto it = tr.begin();
    std::cout << (*it).second;
    ++it;
    std::cout << (*it).second;
    ++it;
    std::cout << it->second;
    ++(++(++(++(++it))));
    ++it;
    std::cout << '\n' << (it == tr.end()) << '\n'; */

    for (auto iter = tr.begin(); iter != tr.cend(); ++iter) {

        std::cout << iter->second << ':' << iter->first << '\n';

        //if (iter->second == "Juve") break;
    }
    std::cout << "exit??\n";

    /* it->second = "Real madrid";
    std::cout << it->second; */
    //it->first = 123;

    /* std::cout << "\n\n";
    auto iter = tr.find(19);

    if (iter != tr.end()) {
        std::cout << iter->second << '\n';
    } */
    /* std::cout << (*it).data;
    ++it;
    std::cout << (*it).data;
    ++it;
    std::cout << (*it).data;
    ++it;
    std::cout << (*it).data; */

    return 0;
}