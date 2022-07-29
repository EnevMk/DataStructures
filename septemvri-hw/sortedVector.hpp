
#ifndef SORTED_VECTOR_HPP
#define SORTED_VECTOR_HPP

#include "vector.hpp"

template<typename T, class Comparator = std::less<T>>
class SortedVector {

private:
    Vector<T> container;
    
    friend class SortedVectorProxy;

    #include "sortedVectorProxy.inl"
public:

    void add(const T& elem);

    const T& operator[](size_t index) const;
    
    SortedVectorProxy operator[](size_t index) {
        return SortedVectorProxy(*this, index);
    }

    size_t size() const;

    void remove(int i);

    //typename Vector<T>::iterator begin() const;

private:

    void drawBackward(size_t index);
    void pushForward(size_t index);

};

#include "sortedVector.inl"

#endif