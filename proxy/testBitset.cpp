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

    StaticBitset sb(50, 0);
    //std::cout << sb.get(0) << ' ' << sb.get(31) << ' ' << sb.get(32);
    for (int i = 0; i < sb.size(); ++i) {
        std::cout << sb[i] << ' ';
    }

    sb.set(47);
    sb.clear(47);
    sb.set(1);
    std::cout << '\n';
    for (int i = 0; i < sb.size(); ++i) {
        std::cout << sb[i] << ' ';
    }

    sb.set(46);
    std::cout << '\n' << sb.get(46);

/*     sb.clear(1);

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
    } */
    
}