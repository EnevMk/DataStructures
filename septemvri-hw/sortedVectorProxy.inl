#ifndef SORTED_VECTOR_PROXY_IMPLEMENTATION
#define SORTED_VECTOR_PROXY_IMPLEMENTATION

#include "sortedVector.hpp"


class SortedVectorProxy {

private:
    SortedVector& ownerObject;
    size_t index;

    SortedVectorProxy(SortedVector& owner, size_t index) : ownerObject(owner), index(index) {}

    friend class SortedVector;

public:

    operator const T&() {
        return ownerObject.container[index];
    }

    SortedVectorProxy& operator=(const T& elem) {
        
        if (Comparator()(ownerObject[index], elem)) {
            
            ownerObject.container[index] = elem;
            
            ownerObject.pushForward(index);
        }
        else if (Comparator()(elem, ownerObject[index])) {
            
            ownerObject.container[index] = elem;

            ownerObject.drawBackward(index);
        }
        else {
            
            ownerObject[index] = elem;
        }
        
        return *this;
    }

    SortedVectorProxy& operator=(const SortedVectorProxy& obj) {
        return this->operator=((const T&)obj);
    }
};

#endif