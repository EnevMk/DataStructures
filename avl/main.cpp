#include <iostream>
#include <string>
#include "avl.hpp"

using std::string;

void foo(const avl_tree<int, string>& calcio) {

    auto low = calcio.lower_bound(11);
    avl_tree<int, std::string>::const_iterator eq = calcio.upper_bound(10);
    std::cout << low->second << '\n';
    for (auto it = eq; it != calcio.cend(); ++it) {
        
        std::cout << it->second << ' ';
    }
}

int main() {

    avl_tree<int, string> calcio;

    calcio.insert(19, "Milan");
    calcio.insert(19, "Inter");
    /* calcio.insert(36, "Juventus");
    calcio.insert(7, "Bologna");
    calcio.insert(3, "Roma");
    calcio.insert(9, "Genoa");
    calcio.insert(11, "Napoli"); */
    //calcio.insert(14, "Udinese");
    std::cout << "----\n";
    
    calcio.erase(19);
    std::cout << calcio.unique_keys() << '\n';
    //calcio.erase(9);
    //calcio.erase(10);
    
    //std::cout << calcio.balance_factor(calcio.find(11)) << '\n';

    //std::cout << calcio.find_eq_or_greater(calcio.root, 17)->container.front().second << '\n';
    //std::cout << calcio.count(19) << '\n';

    /* for (auto &team: v) {

        std::cout << team.second << '\n';
    } */
    
    //foo(calcio);
    /* auto it = calcio.begin();
    std::cout << (++++++++++++++it != calcio.end());
    std::cout << it->second; */
}