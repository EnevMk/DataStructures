#include <iostream>
#include <vector>

template <typename T>
int binarySearch(T element, std::vector<T> container) {
    
    int begin = 0, end = container.size(),
    mid = end + begin / 2;

    while (element != container[mid] && end - begin > 1) {

        if (element > container[mid]) {
            begin = mid + 1;
        }
        else if (element < container[mid]) {
            end = mid - 1;
        }

        mid = end + begin / 2;
    }

    if (end - begin <= 1) return -1;
    return mid;
}

int main() {

    std::vector<int> myVec{1, 3, 5, 7, 11, 13, 17, 19};
    std::cout << binarySearch(4, myVec);
    return 0;
}
