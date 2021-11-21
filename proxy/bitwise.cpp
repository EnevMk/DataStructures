#include <iostream>

uint32_t decToBinary(uint32_t num) {

    uint32_t resrev = 0;

    while (num != 0) {

        resrev = resrev * 10 + num % 2;
        num /= 2;
    }

    uint32_t rev = 0;
    while (resrev != 0) {

        rev = rev * 10 + resrev % 10;
        resrev /= 10;
    }

    return rev;
}

int main() {
    uint32_t n = 113;
    //n |= 1 << (32 - 2);

    int firstBit = (n >> 7) & 1;
    std::cout << decToBinary(n) << '\n';
    n |= (1 << 2);

    int bbb = decToBinary(n);
    std::cout << ~n;
    

    std::cout << '\n' << ((n >> 0) & 1);
    std::cout << '\n' << (uint32_t(1) << 3);
    return 0;
}