#include <iostream>

#include "vector.hpp"

int main() {

    /* vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);

    for (auto it = v.begin(); it != v.end(); ++it) {
        std::cout << *it << ' ';
    } */

    

    vector<int> v(1, int(0));

    int val(0);
   
    v.insert(v.begin(), ++val);
    v.insert(v.end(), ++val);
    
    std::cout << '\n';
    for (auto it = v.begin(); it != v.end(); ++it) {
        std::cout << *it << ' ';
    }

    v.erase(v.begin());

    std::cout << '\n';
    for (auto it = v.begin(); it != v.end(); ++it) {
        std::cout << *it << ' ';
    }

    
    return 0;
}