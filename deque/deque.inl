#ifndef DEQUE_IMPL
#define DEQUE_IMPL

#include "deque.hpp"

template <typename Type, unsigned BlockSize>
deque<Type, BlockSize>::deque(): map(1, new array<Type, BlockSize>), size_(0) {

    

    size_t block = 0, block_mid = (block * BlockSize) + (BlockSize - 1)/ 2;
    front_index = block_mid, back_index = block_mid + 1;
}

template <typename Type, unsigned BlockSize>
deque<Type, BlockSize>::deque(const deque& obj) {

    size_t startArr = obj.front_index / BlockSize,
           endArr = obj.back_index / BlockSize;
    
    map.reserve(endArr - startArr + 1);
    for (size_t i = 0; i <= endArr - startArr; ++i) {

        this->map.push_back(new std::array<Type, BlockSize>);
        *this->map[i] = *obj.map[i + startArr];
    }

    this->front_index = obj.front_index % BlockSize;
    this->back_index = obj.back_index - startArr * BlockSize;

    this->size_ = obj.size_;
}

template <typename Type, unsigned BlockSize>
deque<Type, BlockSize>::deque(deque&& obj) 
: map(std::move(obj.map)), 
  front_index(obj.front_index),
  back_index(obj.back_index),
  size_(obj.size_)
{

    int len = obj.map.size();
    for (int i = 0; i < len; ++i) {
        //map.push_back(obj.map[i]);
        obj.map.pop_back();
    }
    //this->map = obj.map;
}

template <typename Type, unsigned BlockSize>
deque<Type, BlockSize>& deque<Type, BlockSize>::operator=(deque&& obj) {

    if (this != &obj) {
        destroy();
        this->map = std::move(obj.map);
        
        this->size_ = obj.size_;
        front_index = obj.front_index;
        back_index = obj.back_index;
    }
    
    return *this;
}

template <typename Type, unsigned BlockSize>
deque<Type, BlockSize>& deque<Type, BlockSize>::operator=(const deque& obj) {
    
    if (this != &obj) {
        destroy();
        clone(obj);
    }
    return *this;
}


template <typename Type, unsigned BlockSize>
deque<Type, BlockSize>::~deque() {
    
    destroy();
}

template <typename Type, unsigned BlockSize>
void deque<Type, BlockSize>::clone(const deque& obj) {
    size_t startArr = obj.front_index / BlockSize,
           endArr = obj.back_index / BlockSize;
    
    map.reserve(endArr - startArr + 1);
    //map.resize(endArr - startArr + 1);
    for (size_t i = 0; i <= endArr - startArr; ++i) {

        this->map.push_back(new std::array<Type, BlockSize>);// = new std::array<Type, BlockSize>;
        *this->map[i] = *obj.map[i + startArr];
    }

    this->front_index = obj.front_index % BlockSize;
    this->back_index = obj.back_index - startArr * BlockSize;

    this->size_ = obj.size_;
}

template <typename Type, unsigned BlockSize>
void deque<Type, BlockSize>::destroy() {
    size_t len = this->map.size() - 1;
    for (int i = len; i >= 0; --i) {
        delete map[i];
        map.pop_back();
    }
}

template <typename Type, unsigned BlockSize>
Type& deque<Type, BlockSize>::getAt(size_t i) {
    size_t map_index = i / (BlockSize);
    
    return (*map[map_index])[i % BlockSize];
}

template <typename Type, unsigned BlockSize>
typename deque<Type, BlockSize>::block* const* deque<Type, BlockSize>::getBlock(size_t i) const {
    size_t map_index = i / (BlockSize);
    return &(map[map_index]);
}

template <typename Type, unsigned BlockSize>
Type& deque<Type, BlockSize>::operator[](size_t i) {
    /* size_t map_index = (map.size() - 1) / i;
    
    return (*map[map_index])[BlockSize % i]; */
    return getAt(i + front_index + 1);
}

template <typename Type, unsigned BlockSize>
const Type& deque<Type, BlockSize>::operator[](size_t i) const {
    /* size_t map_index = (map.size() - 1) / i;
    
    return (*map[map_index])[BlockSize % i]; */
    return getAt(i + front_index);
}

template <typename Type, unsigned BlockSize>
void deque<Type, BlockSize>::push_back(const Type& val) {
    //auto ref = this->getAt(back_index);
    if (back_index % BlockSize == 0) {
        resize_back();
    }
    
    this->getAt(back_index++) = val;
    size_++;
}

template <typename Type, unsigned BlockSize>
void deque<Type, BlockSize>::push_front(const Type& val) {

    if (front_index == 0) {
        resize_front();
    }
    this->getAt(front_index--) = val;
    size_++;
}

template <typename Type, unsigned BlockSize>
void deque<Type, BlockSize>::resize_front() {
    //std::cout << "front realloc\n";

    map.insert(map.begin(), new std::array<Type, BlockSize>);
    front_index += BlockSize;
    back_index += BlockSize;
}

template <typename Type, unsigned BlockSize>
void deque<Type, BlockSize>::resize_back() {
    map.push_back(new std::array<Type, BlockSize>);
    
}

template <typename Type, unsigned BlockSize>
size_t deque<Type, BlockSize>::size() const {
    return size_;
}

template <typename Type, unsigned BlockSize>
void deque<Type, BlockSize>::pop_front() {
    front_index++;
}
    
template <typename Type, unsigned BlockSize>
void deque<Type, BlockSize>::pop_back() {
    back_index--;
}

template <typename Type, unsigned BlockSize>
void deque<Type, BlockSize>::shrink_to_fit() {

    size_t startArr = this->front_index / BlockSize,
           endArr = this->back_index / BlockSize;

    while (back_index < map.size()) {
        delete map.back();
        map.pop_back();
    }

    for (size_t i = 0; i < startArr; ++i) {
        delete map[i];
    }

    map.erase(map.begin(), map.begin() + startArr);

    map.shrink_to_fit();
}

#endif