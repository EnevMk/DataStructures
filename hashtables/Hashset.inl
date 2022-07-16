#ifndef HASHSET_IMPLEMENTATION
#define HASHSET_IMPLEMENTATION

#include "Hashset.hpp"

template <typename Value>
Hashset<Value>::Hashset() {
    this->table.resize(16);
}

template <typename Value>
int Hashset<Value>::getIndex(const Value& value) const {
    return std::hash<Value>()(value) % table.size();
}

template <typename Value>
typename Hashset<Value>::const_table_iterator Hashset<Value>::getBucket(const Value& value) const {
    return table.cbegin() + getIndex(value);
}

template <typename Value>
typename Hashset<Value>::table_iterator Hashset<Value>::getBucket(const Value& value) {
    return table.begin() + getIndex(value);
}

template <typename Value>
float Hashset<Value>::getLoadFactor() const {
    return float (count) / float (table.size());
}

template <typename Value>
bool Hashset<Value>::shouldResize() const {
    return getLoadFactor() > MAX_LOAD_FACTOR;
}

template <typename Value>
int Hashset<Value>::size() const {
    return this->count;
}

template <typename Value>
void Hashset<Value>::resize() {
    Table oldTable(size() * 2);
    std::swap(oldTable, table);

    this->count = 0;

    for(table_iterator it = oldTable.begin(); it != oldTable.end(); ++it) {

        for (bucket_iterator elem = it->begin(); elem != it->end(); ++elem) {

            insert(*elem);
        }
    }
}

template <typename Value>
std::pair<typename Hashset<Value>::iterator, bool> Hashset<Value>:: insert(const Value& value) {

    auto bucket = getBucket(value);

    for (auto elem = bucket->begin(); elem != bucket->end(); elem++) {

        if (*elem == value) {
            return std::make_pair(const_iterator(bucket, elem, table.cend()), false);
        }
    }

    if (shouldResize()) {
        resize();
    }

    bucket->push_front(value);
    count++;

    return make_pair(const_iterator(bucket, bucket->begin(), table.cend()), true);
}

/* template <typename Value>
typename Hashset<Value>::iterator Hashset<Value>::find(const Value& value) {

    table_iterator bucket = getBucket(value);

    for (bucket_iterator element = bucket->begin(); element != bucket->end(); ++element) {

        if (*element == value) {
            return iterator(bucket, element, table.end());
        }
    }

    return end();
} */

template <typename Value>
typename Hashset<Value>::const_iterator Hashset<Value>::find(const Value& value) const {
    const_table_iterator bucket = getBucket(value);

    for (const_bucket_iterator element = bucket->cbegin(); element != bucket->cend(); ++element) {

        if (*element == value) {
            return const_iterator(bucket, element, table.end());
        }
    }

    return cend();
}

#endif