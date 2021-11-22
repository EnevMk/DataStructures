#include <iostream>
#include "list.hpp"
//#include "C:\Users\Asus VivoBook X510UF\Desktop\dev\SD\common\type_traits.hpp"

int main() {

    list<int> myList(5, 9);

    list<int>::base_iterator<false> iter = myList.begin();
    std::cout << *iter;

    *iter = 18;
    std::cout << *iter;



    //std::cout << is_const<const int>::value << '\n';

    //enable_if<true, const int>::type x = 4;
    //enable_if<false, int>::type z = 4; // syntax err
    //std::cout << x;

    /* x = 10;
    std::cout << x; */

    return 0;
}