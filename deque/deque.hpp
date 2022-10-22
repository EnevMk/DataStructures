#ifndef DEQUE_HPP
#define DEQUE_HPP

//#include "../TDD_Vector/vector.hpp"

#include <array>
#include <vector>
#include <cmath>

using std::array;
using std::vector;
using std::enable_if;
using std::is_const;

template <typename Type, unsigned BlockSize>
class deque {

private:

    size_t front_index;
    size_t back_index;

    unsigned size_;
       
    typedef std::array<Type, BlockSize> block; 
    

    vector<block*> map;

public:

    deque();
    deque(const deque& obj);

    deque& operator=(const deque& obj);

    deque(deque&& obj);
    deque& operator=(deque&& obj);

    ~deque();

    const Type& operator[](size_t i) const;
    Type& operator[](size_t i);

    void pop_front();
    void pop_back();

    void push_front(const Type&);
    void push_back(const Type&);

    const Type& front() const;
    Type& front();

    const Type& back() const;
    Type& back();

    size_t size() const;

    void shrink_to_fit();

    /* typedef typename vector<block*>::iterator vec_iterator;
    typedef typename vector<block*>::const_iterator const_vec_iterator;
    typedef typename vector<block*>::reverse_iterator vec_reverse_iterator;
    typedef typename vector<block*>::const_reverse_iterator vec_const_reverse_iterator; */

    #include "deque_iterator.inl"

    typedef base_iterator<const Type, forward/* const_vec_iterator, block::const_iterator */> const_iterator;
    typedef base_iterator<const Type, reverse> const_reverse_iterator;
    typedef base_iterator<Type, reverse> reverse_iterator;
    typedef base_iterator<Type, forward> iterator;
    /* typedef base_iterator<Type, forward> iterator;
    typedef base_iterator<const Type, reverse> const_reverse_iterator;
    typedef base_iterator<Type, reverse> reverse_iterator; */

public:
    const_iterator cbegin() const {

        block* const * b = getBlock(front_index);

        return const_iterator(b, (front_index + 1) % BlockSize);
    }

    const_iterator cend() const {

        block* const * b = getBlock(back_index);

        return const_iterator(b, (back_index) % BlockSize);
    }

    const_reverse_iterator crbegin() const {
        block* const * b = getBlock(back_index);

        return const_reverse_iterator(b, (back_index - 1) % BlockSize);
    }
    
    const_reverse_iterator crend() const {
        block* const * b = getBlock(front_index);
        return const_reverse_iterator(b, (front_index) % BlockSize);
    }

    iterator begin() {
        block* const * b = getBlock(front_index);
        return iterator(b, (front_index + 1) % BlockSize);
    }

    const_iterator begin() const {
        /* block* const * b = getBlock(front_index);
        return const_iterator(b, (front_index + 1) % BlockSize); */
        return cbegin();
    }

    iterator end() {
        block* const * b = getBlock(back_index);
        return iterator(b, (back_index) % BlockSize);
    }

    const_iterator end() const {
        /* block* const * b = getBlock(back_index);
        return const_iterator(b, (back_index) % BlockSize); */
        return cend();
    }

    const_reverse_iterator rbegin() const {
        return crbegin();
    }

    const_reverse_iterator rend() const {
        return crend();
    }

    reverse_iterator rbegin() {
        block* const * b = getBlock(back_index);

        return reverse_iterator(b, (back_index - 1) % BlockSize);
    }

    reverse_iterator rend() {
        block* const * b = getBlock(front_index);
        return const_reverse_iterator(b, (front_index) % BlockSize);
    }

    
private:

    void resize_front();
    void resize_back();

    void clone(const deque& obj);
    void destroy();

    Type& getAt(size_t i);
    block* const* getBlock(size_t i) const;
};

#include "deque.inl"

#endif