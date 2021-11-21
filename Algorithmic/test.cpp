#include <list>
#include <iostream>

int main() {

    std::list<int> sh;
    sh.push_front(1);
    sh.push_front(2);
    sh.push_front(3);

    sh.erase(--sh.end());
    for (int& x : sh) {
        std::cout << x << ' ';
    }
    return 0;
}