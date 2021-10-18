#include <iostream>
#include <vector>

#include "Vector.hpp"
#include "IteratorFunctions.hpp"

int main() {
    int arr[] = {1, 5, 2, 14, 33, -4, 7};

    Vector<int> v(arr, sizeof(arr) / sizeof(int));

    std::vector<int> v2{1, 2, 5, 3, 9, -2};
    

    std::cout << myIterator::distance(v.begin(), v.end()) << '\n';
    std::cout << *(myIterator::next(v2.begin(), 3, v2.end())) << '\n';
    std::cout << *(v.begin() + 5);


    /* for (typename Vector<int>::Iterator it = v.begin();
                                        it != v.end(); ++it) 

    {
        std::cout << *it << ' ';
    } */

    return 0;
}