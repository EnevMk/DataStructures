#ifndef _SORTED_ARRAY_IMPLEMENTATION
#define _SORTED_ARRAY_IMPLEMENTATION

#include "sortedVector.hpp"
//#include "sortedVectorProxy.hpp"

template <typename T, class Comparator>
void SortedVector<T, Comparator>::add(const T& elem) {
    container.push_back(elem);

    drawBackward(container.size() - 1);
}

template <typename T, class Comparator>
const T& SortedVector<T, Comparator>::operator[](size_t index) const {
    return container[index];
}

template <typename T, class Comparator>
size_t SortedVector<T, Comparator>::size() const {
    return container.size();
}

template <typename T, class Comparator>
void SortedVector<T, Comparator>::drawBackward(size_t index) {
        
    if (index == 0 || Comparator()(container[index - 1], container[index]) /* container[index - 1] < container[index] */) {
        return;
    }

    T data = container[index];

    while (index && Comparator()(data, container[index - 1])/*  data < container[index - 1] */) {
        container[index] = container[index - 1];
        --index;
    }
    container[index] = data;
}

template <typename T, class Comparator>
void SortedVector<T, Comparator>::pushForward(size_t index) {
    
    if (index == this->container.size() || Comparator()(container[index], container[index + 1])  /* container[index] < container[index + 1] */) {
        return;
    }

    T data = container[index];

    while (index < container.size() - 1 && Comparator()(container[index + 1], data) /* container[index + 1] < data */ ) {
        container[index] = container[index + 1];
        index++;
    }
    container[index] = data;
}

template <typename T, class Comparator>
typename Vector<T>::iterator SortedVector<T, Comparator>::begin() const {
    return this->container.begin();
}

#endif