#ifndef ITERATOR_FUNCTIONALITY
#define ITERATOR_FUNCTIONALITY

#include "Vector.hpp"

namespace myIterator {
    template <typename Iter>
    bool equals(const Iter &left, const Iter &right);

    template <typename Iter>
    std::ptrdiff_t distance(Iter left, Iter right);

    template <typename Iter>
    Iter next(Iter left, int count, Iter end);
}

template <typename Iter>
bool myIterator::equals(const Iter &left, const Iter &right) {
    return (left.getPtr() == right.getPtr());
}

template <typename Iter>
std::ptrdiff_t myIterator::distance(Iter left, Iter right) {

    int dist = 0;

    while (left != right) {
        ++left;
        dist++;
    }

    return dist;
}

template <typename Iter>
Iter myIterator::next(Iter left, int count, Iter end) {
        int steps = 0;

        while (steps < count && left != end) {
            ++left;
            steps++;
        }

        return left;
    }
#endif