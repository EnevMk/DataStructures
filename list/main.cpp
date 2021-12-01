#include <iostream>
#include "list.hpp"
//#include "C:\Users\Asus VivoBook X510UF\Desktop\dev\SD\common\type_traits.hpp"

int main() {

    list<int> myList(5, 9);

    list<int>::base_iterator<false> iter = myList.begin();
    ++iter;
    *iter = 17;
    auto it = myList.insert(iter, 3, 93);

    //std::cout << '\n' << *it;

    std::vector<int> myVec(2, 20);

    myList.insert(myList.cend(), myVec.cbegin(), myVec.cend());

    for (int &x : myList) {

        std::cout << x << ' ';
    }
    std::cout << '\n';

    auto afterErase = myList.erase(iter);

    for (int &x : myList) {

        std::cout << x << ' ';
    }
    std::cout << '\n' << *afterErase;
    return 0;
}