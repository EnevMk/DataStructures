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


    avl_tree<int, std::string> tr;

    int a = 3, b = 18, c = 19, d = 36, e= 9, f= 2, g = 7;
    
    tr.insert(12, "Conor");
    tr.insert(33, "Showtime");
    tr.insert(1, "Covington");
    tr.insert(10, "Adesanya");
    tr.insert(15, "Mghty mouse");
    tr.insert(2, "Dillashaw");
    tr.insert(90, "DO Bronx");
    tr.insert(87, "Poirier");
    tr.insert(88, "Oretge");
    tr.insert(11, "Aspinall");
    tr.insert(24, "Paddy");

    //tr.insert(28, "REal");

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
    std::cout << tr.balance_factor(one);

    return 0;
}