#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include <vector>

#include "avl.hpp"

using namespace std;

class IntWrapper {

public:
    int num{};

    //IntWrapper(int n) : num(n) {}
};

struct comp {
    bool operator() (const int* a, const int* b) {
        return *a < *b;
    }
};

int main() {


    avl_tree<std::string, int> tr;

    tr.insert("Banana", 1);
    tr.insert("Lemon", 1);
    tr.insert("Pineapple", 1);
    tr.insert("Tomato", 1);
    tr.insert("StrawBerry", 1);

    auto pairIt = tr.equal_range("Cucumber");

    for (auto it = pairIt.first; it != tr.cend(); ++it) {
        std::cout << it->first << ' ';
        
    }    
    //std::cout << (++pairIt.first)->first;
    /* //tr.insert(28, "REal");

    std::cout << "unique: " << tr.unique_keys() << '\n';
    std::cout << tr.find(33).current->right->container.front().first;
    auto iteq = tr.equal_range(24);
    
    tr.erase(iteq.first);
    auto bronx = tr.find(15);
    tr.erase(bronx);

    int i = 0;
    for (auto iter = tr.cbegin(); iter != tr.cend(); iter++) {
        i++;
        if (i == 25) break;
        std::cout << iter->second << " : " <<  iter->first << '\n';
    }

    auto one = tr.find(12);
    std::cout << tr.balance_factor(one); */

    return 0;
}