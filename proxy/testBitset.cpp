#include <iostream>
#include "StaticBitset.hpp"

int main() {

    /* int num = 5;
    //num |= 3;

    std::cout << (num);

    num |= 1 << 1;
    std::cout << '\n' << (num);

    num &= ~(1 << 0);
    std::cout << '\n' << (num); */

    StaticBitset sb(10, 0);

    for (int i = 0; i < sb.size(); ++i) {
        std::cout << sb[i] << ' ';
    }

    sb.set(1);
    std::cout << '\n';
    for (int i = 0; i < sb.size(); ++i) {
        std::cout << sb[i] << ' ';
    }

    sb.clear(1);

    std::cout << '\n';
    for (int i = 0; i < sb.size(); ++i) {
        std::cout << sb[i] << ' ';
    }

    for (int i = 0; i < sb.size(); ++i) {

        if (i % 2) {
            sb[i] = true;
        }
    }

    std::cout << '\n';
    for (int i = 0; i < sb.size(); ++i) {
        std::cout << sb[i] << ' ';
    }
    
}