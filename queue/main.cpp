#include <iostream>
#include "q.hpp"
#include <queue>

int main() {
    
    Queue<int> q;
    q.push(1);
    q.push(3);
    q.push(5);
    q.push(7);
    q.push(11);
    q.pop();
    q.pop();

    std::cout << "\nFront of q: " << q.front(); // expecting 1
    std::cout << "\nBack of q: " << q.back(); // expecting 11

    int arr[] = {1, 3, 5, 7, 11};
    std::cout << "\narr length1: " << sizeof(arr) / sizeof(arr[0]);

    return 0;
}
