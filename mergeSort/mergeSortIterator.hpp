#include "IteratorFunctions.hpp"

template <typename Iterator>
void merge(Iterator begin, Iterator mid, Iterator end) {

    int size1 = myIterator::distance(begin, mid) + 1, size2 = myIterator::distance(mid + 1, end);
    
    using Type = Iterator::value_type;

    int *subArrOne = new Type[size1], *subArrTwo = new Type[size2];

    for (int i = 0; i < size1; ++i) {
        subArrOne[i] = *(myIterator::next(begin, i, mid)); // mid?
    }

    for (int i = 0; i < size2; ++i) {
        subArrTwo[i] = *(myIterator::next(begin, size1 + i, end))
    }

    int indexRightArr = 0, indexLeftArr = 0;
    Iterator mergedArrIterator = begin;

    while (indexLeftArr < size1 && indexRightArr < size2) {

        if (subArrOne[indexLeftArr] < subArrTwo[indexRightArr]) {
            
        }
    }
}

template <typename Iterator>
void mergeSort(Iterator begin, Iterator end) {

    int size = myIterator::distance(begin, end);

    if (size > 2) {

        Iterator mid = myIterator::next(begin, size / 2, end);
        mergeSort(begin, mid);
        mergeSort(++mid, end);
        merge()
    }
}