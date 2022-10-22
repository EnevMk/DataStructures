#include <iostream>
#include "deque.hpp"

deque<int, 12> foo() {
    deque<int, 12> d;

    for (size_t i = 0; i < 8; ++i) {
        d.push_front(i * 3);
    }

    for (size_t i = 0; i < 3; ++i) {
        d.push_back(i * 4);
    }

    return d;
}

deque<int, 12> foo(deque<int, 12> copy) {

    copy.push_back(7);
    return copy;
}

struct A {
    int a;
    float b;
};

int main() {

    std::vector<int> v{12, 98, 65, 43, 34, 17};

    auto it = v.begin() + 2;

    deque<A, 12> da;

    da.push_back(A());
    da.push_back(A());
    da.push_back(A());
    da.push_back(A());

    auto t = da.begin();
    t->a = 4;

    //std::vector<int>::iterator itt = v.cbegin();

    /* it[2] = 0;
    std::cout << (it[2]); */

    std::cout << -25 / 12;

    /* array<int, 6> a;
    std::cout << a.size(); */
    deque<int, 12> d = foo();
    d = foo(d);

    for (size_t i = 0; i < d.size(); ++i) {
        std::cout << d[i] << ' ';
    }
    std::cout << '\n';
    for (auto it = d.begin(); it != d.end(); ++it) {
        *it = 5;
        std::cout << *it << ' ';
    }
    
    /* for (size_t i = 0; i < 8; ++i) {
        d2.push_front(i * 3);
    }

    for (size_t i = 0; i < 3; ++i) {
        d2.push_back(i * 4);
    } */

    /* d2.push_back(12);
    d2.push_back(16);
    d2.push_back(16);
    d2.push_back(16); */
    /* auto d3 = foo(d2);
    deque<int, 12> d = d3;;
    //std::cout << d[0];
    for (size_t i = 0; i < d.size(); ++i) {
        std::cout << d[i] << ' ';
    } */


    /* std::array<int, 5> a{1, 2, 3, 4, 5};
    auto b = a;

    b[2] = 17;

    for (int i = 0; i < a.size(); ++i) {
        std::cout << a[i] << ' ';
    } */
    return 0;
}