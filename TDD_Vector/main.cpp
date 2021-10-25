#include <iostream>

#include "vector.hpp"

int main() {

    vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);

    for (auto it = v.begin(); it != v.end(); ++it) {
        std::cout << *it << ' ';
    }

    v.insert(v.begin() + 1, 4);

    for (auto it = v.begin(); it != v.end(); ++it) {
        std::cout << *it << ' ';
    }

    return 0;
}