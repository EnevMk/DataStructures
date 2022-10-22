#ifndef DEQUE_ITERATOR_IMPL
#define DEQUE_ITERATOR_IMPL

#include "deque.hpp"

enum iterator_direction {
    forward = 1,
    reverse = -1
};

template <typename T = Type, iterator_direction dir = forward/* typename vector_iterator = vec_iterator, typename block_iterator = block::iterator */>
class base_iterator {

friend class deque;
private:

    int index;
    
    array<Type, BlockSize>* const* blockRef;

    /* vector_iterator blockIt;
    block_iterator element; */

    //block::iterator it;

    base_iterator(block* const * blockRef, int i) : blockRef(blockRef), index(i) {} ;
    //base_iterator(vector_iterator vec, block_iterator elem) : blockIt(vec), element(elem) {}


    void getPrevious() {

        if (index < 0) {
            index = BlockSize - 1;
            --blockRef;
        }
    }

    void getNext(int n) {
        index += n * (int)dir;

        size_t block_offset = index / BlockSize;
        blockRef += block_offset;

        index %= BlockSize;
    }

    void getPrevious(int pos) {
        //index += n * iterator_direction;

        if (pos > index) {

            size_t block_offset = ((pos - index) / BlockSize) + 1;

            blockRef -= block_offset;
            index = BlockSize - ((pos - index) % BlockSize);
        } else {
            index -= pos;
        }
    }

    void getAdjacent(int pos) {

        if (pos * dir < 0) getPrevious(abs(pos));
        else { getNext(abs(pos)); }
    }
public:

    template <typename Q = T>
    typename enable_if<is_const<Q>::value, Q>::type & operator*() const {
        return (**blockRef)[index];
        //return *element;
    }
    
    template <typename Q = T>
    typename enable_if<!is_const<Q>::value, Q>::type & operator*() {
        return (**blockRef)[index];
    }

    template <typename Q = T>
    typename enable_if<!is_const<Q>::value, Q>::type * operator->() const {
        return std::addressof((**blockRef)[index]);
    }

    template <typename Q = T>
    typename enable_if<!is_const<Q>::value, Q>::type * operator->() {
        return std::addressof((**blockRef)[index]);
    }

    base_iterator& operator++() {
        //index += int(dir);

        getAdjacent(1);

        /* ++element;
        getNext(); */
        return *this;
    }

    base_iterator operator++(int) {
        base_iterator toReturn = *this;
        ++(*this);
        getAdjacent(1);

        return toReturn;
    }

    base_iterator& operator--() {
        index -= int(dir);
        getPrevious();
    }

    base_iterator operator--(int) {
        auto it = *this;
        --(*this);
        getPrevious();

        return it;
    }

    base_iterator operator+(int pos) {
        base_iterator it(*this);
        it.getAdjacent(pos);

        return it;
    }

    base_iterator operator-(int pos) {
        base_iterator it(*this);
        it.getAdjacent(pos);

        return it;
    }

    base_iterator& operator+=(int pos) {
        *this = *this + pos;
        return *this;
    }

    base_iterator& operator-=(int pos) {
        *this = *this - pos;
        return *this;
    }

    const T& operator[](std::ptrdiff_t pos) const {
        return *this + pos;
    }

    template <typename OtherType, iterator_direction OtherDir>
    bool operator==(const base_iterator<OtherType, OtherDir> &obj) const {

        return (this->blockRef == obj.blockRef && this->index == obj.index);
    }

    template <typename OtherType, iterator_direction OtherDir>
    bool operator!=(const base_iterator<OtherType, OtherDir> &obj) const {
        return !(*this == obj);
    }

};

#endif