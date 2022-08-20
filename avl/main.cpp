#include <iostream>
#include <string>
#include <map>

#include "avl.hpp"

void test_iter(const avl_tree<int, std::string>& t) {
    avl_tree<int, std::string>::const_iterator itup = t.upper_bound(17);
    auto itdown = t.lower_bound(3);

    for (auto iter = itdown; iter != itup; ++iter) {
        std::cout << iter->second << ':' << iter->first << '\n';
        
    }
}

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
    tr.insert(28, "REal");

    //tr.erase(2);


    std::cout << '\n' << tr.height() << '\n' << tr.size();

    avl_tree<int, std::string>::iterator itup = tr.upper_bound(17);
    auto itdown = tr.lower_bound(3);
    
    /* ++itdown;
    ++itdown;
    ++itdown; */
    //++itdown;

    std::cout << (itdown == itup);
    //test_iter(tr);
    //std::cout << itup->second;
    /* ++itup;
    std::cout << itup->second;
    ++itup;
    std::cout << itup->second;
    ++itup;
    std::cout << itup->second; */
    auto iter = itdown;
    /* for (iter; iter != itup; ++iter) {
        std::cout << "???\n";
        std::cout << iter->second << ':' << iter->first << '\n';
        
    } */
    
    
    auto range = tr.equal_range(2);

    for (auto it = range.first; it != range.second; ++it) {
        std::cout << it->second << ' ';
    }


    return 0;
}