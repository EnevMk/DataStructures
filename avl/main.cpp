#include <iostream>
#include <string>
#include <map>

#include "avl.hpp"

/* void test_iter(const avl_tree<int, std::string>& t) {
    avl_tree<int, std::string>::const_iterator itup = t.upper_bound(17);
    auto itdown = t.lower_bound(3);

    for (auto iter = itdown; iter != itup; ++iter) {
        std::cout << iter->second << ':' << iter->first << '\n';
        
    }
} */

int main() {


    avl_tree<int, std::string> tr;
    
    tr.insert(3, "Roma");
    tr.insert(18, "Inter");
    tr.insert(19, "Milan");
    tr.insert(36, "Juve");
    tr.insert(9, "Genoa");
    tr.insert(2, "Fiorentina");
    tr.insert(2, "Napoli");
    tr.insert(7, "Bologna");
    //tr.insert(28, "REal");

    //tr.erase(2);


    std::cout << '\n' << tr.height() << '\n' << tr.size() << '\n';

    auto itdown = tr.lower_bound(7), itup = tr.upper_bound(18);

    for (auto it = itdown; it != itup; ++it) {
        std::cout << it->second << " : " << it->first << '\n';
    }
    
    
    
    auto range = tr.equal_range(2);

    for (auto it = range.first; it != range.second; ++it) {
        std::cout << it->second << ' ';
    }


    return 0;
}