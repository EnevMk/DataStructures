#pragma once

#include <cassert>
#include <cstring>
#include <cstdint>

class StaticBitset {

private:
    uint32_t *data;
    int count;

    constexpr static size_t containerBitsCount = 8 * sizeof(uint32_t);

    constexpr inline size_t getCountOfContainersNeeded(size_t bitsCount) {
        
        return (bitsCount / this->containerBitsCount + ((bitsCount % this->containerBitsCount) ? 1 : 0));
        
    }


public:
    StaticBitset(int count, bool value);
    ~StaticBitset();

    bool get(size_t ind) const;
    void set(size_t ind);
    void clear(size_t ind);

    int size() const {
		return count;
	}

    ///proxy wrapper for one bit from the bitset
    class proxy {
    
    private:

		StaticBitset *bitset;
		const int index;
		friend struct StaticBitset;

		proxy(StaticBitset *owner, int index);
    
    public:
        proxy operator=(bool val);
        
        operator bool() const;
    };

    proxy operator[](int index);
    bool operator[](size_t index) const;
    
};

StaticBitset::StaticBitset(int count_, bool value) : data(new uint32_t[getCountOfContainersNeeded(count_)]), count(count_) {
        
    if (value) {
        memset(this->data, 0b11111111, getCountOfContainersNeeded(count_) * sizeof(uint32_t));
    }
    else {
        memset(this->data, 0, getCountOfContainersNeeded(count_) * sizeof(uint32_t));
    }
}

StaticBitset::~StaticBitset() {
    delete[] data;
}

StaticBitset::proxy::proxy(StaticBitset *owner, int index)
    : bitset(owner)
    , index(index)
{}

StaticBitset::proxy::operator bool() const {
    return this->bitset->get(this->index);
}

StaticBitset::proxy StaticBitset::proxy::operator=(bool val) {
    if (val) {
        this->bitset->set(this->index);
    }
    else {
        this->bitset->clear(this->index);
    }

    return *this;
}

void StaticBitset::set(size_t index) {
    
    if (index > this->count) {
        throw "Out of bitset range";
    }
    this->data[index / this->containerBitsCount] |= (1 << (index % containerBitsCount));
}

bool StaticBitset::get(size_t index) const {

    if (index > this->count) {
        throw "Out of bitset range";
    }

    //return (data[index / this->containerBitsCount] >> index) & 1;
    return this->operator[](index);
}

void StaticBitset::clear(size_t index) {

    if (index > this->count) {
        throw "Out of bitset range";
    }
    this->data[index / this->containerBitsCount] &= ~(1 << (index % containerBitsCount));
    
}

StaticBitset::proxy StaticBitset::operator[](int index) {
    return proxy{this, index};
}

bool StaticBitset::operator[](size_t index) const {
    
    return (data[index / this->containerBitsCount] >> (index % containerBitsCount) & 1);
}