#pragma once

#include <forward_list>
#include <vector>

#include "utils\type_traits.hpp"

template <typename Key, typename Value>
class Hashmap {

    typedef std::pair<Key, Value> Item;
    typedef std::forward_list<Item> Bucket;
    typedef std::vector<Bucket> Table;

    typedef typename Bucket::iterator bucket_iterator;
    typedef typename Table::iterator table_iterator;

    typedef typename Bucket::const_iterator const_bucket_iterator;
    typedef typename Table::const_iterator const_table_iterator;

    Table table;

    const float MAX_LOAD_FACTOR = 0.7f;
    int count = 0;

public:

    template <typename TableType = table_iterator, typename BucketType = bucket_iterator, typename Type = std::pair<Key, Value>>
    struct base_iterator {
    
    private:
        TableType bucket;
        BucketType element;
        TableType end;

        void findNext()  {

            while (bucket != end && element == bucket->end()) {
                ++bucket;

                if (bucket != end) {
                    element = bucket->begin();
                }
            }
        }

        base_iterator(TableType begin, BucketType element, TableType end) 
        : bucket(begin),
          element(element),
          end(end)
        {
            findNext();
        }

        friend class Hashmap;
    public:

        template <typename Q = Type>
        const typename enable_if<is_const<Q>::value, Q>::type & operator*() const {
            return reinterpret_cast<const std::pair<Key, Value>&>(*element);;
        }

        template <typename Q = Type>
        typename enable_if<!is_const<Q>::value, Q>::type & operator*() {
            return reinterpret_cast<std::pair<const Key, Value>&>(*element);
        }

        template <typename Q = Type>
        const typename enable_if<is_const<Q>::value, Q>::type * operator->() const {
            return &reinterpret_cast<const std::pair<Key, Value>&>(*element);;
        }

        template <typename Q = Type>
        typename enable_if<!is_const<Q>::value, Q>::type * operator->() {
            return &reinterpret_cast<std::pair<const Key, Value>&>(*element);
        }

         base_iterator& operator++() {
            ++element;
            findNext();
            return *this;
        }

        bool operator==(const base_iterator &other) const {
            return bucket == other.bucket && element == other.element;
        }

        bool operator!=(const base_iterator &other) const {
            return !(*this == other);
        }
    };

    typedef base_iterator<table_iterator, bucket_iterator, std::pair<const Key, Value>> iterator;
    typedef base_iterator<const_table_iterator, const_bucket_iterator, const std::pair<Key, Value>> const_iterator;

    /* struct iterator {

    private:
        table_iterator bucket;
        bucket_iterator element;
        table_iterator end;

        friend class Hashmap<Key, Value>;

        void findNext() {

            while (bucket != end && element == bucket->end()) {
                ++bucket;
                if (bucket != end) {

                    element = bucket->begin();
                }
            }
        }
    
        
    public:
    
        iterator(table_iterator b, bucket_iterator e, table_iterator end)
            : bucket(b), element(e), end(end)
        {
            findNext();
        }

        iterator& operator++() {

            ++element;
            findNext();
            return *this;
        }

        bool operator==(const iterator &other) const {
            return bucket == other.bucket && element == other.element;
        }

        bool operator!=(const iterator &other) const {
            return !(*this == other);
        }

        std::pair<const Key, Value>& operator*() {

            return reinterpret_cast<std::pair<const Key, Value>&>(*element);
        }

        std::pair<const Key, Value>* operator->() {
            return &reinterpret_cast<std::pair<const Key, Value>&>(*element);
        }

    };

    struct const_iterator {
    
    private:
        const_table_iterator bucket;
        const_bucket_iterator element;
        const_table_iterator end;

        friend class Hashmap<Key, Value>;

        void findNext() {

            while (bucket != end && element == bucket->cend()) {
                ++bucket;
                if (bucket != end) {

                    element = bucket->cbegin();
                }
            }
        }
    public:

        const_iterator(const_table_iterator b, const_bucket_iterator e, const_table_iterator end)
            : bucket(b), element(e), end(end)
        {
            findNext();
        }
        
        const_iterator& operator++() {

            ++element;
            findNext();
            return *this;
        }

        bool operator==(const const_iterator &other) const {
            return bucket == other.bucket && element == other.element;
        }

        bool operator!=(const const_iterator &other) const {
            return !(*this == other);
        }
    
        const std::pair<Key, Value>& operator*() {

            return const_cast<const std::pair<Key, Value>&>(*element);
        }

        const std::pair<Key, Value>* operator->() {
            return &const_cast<std::pair<Key, Value>&>(*element);
        }
    }; */

    const_iterator cbegin() const {
        return const_iterator(table.cbegin(), table.front().cbegin(), table.cend());
    }

    const_iterator cend() const {
        return const_iterator(table.cend(), table.back().cend(), table.cend());
    }

    const_iterator begin() const {
        return const_iterator(table.cbegin(), table.front().cbegin(), table.cend());
    }

    const_iterator end() const {
        return const_iterator(table.cend(), table.back().cend(), table.cend());
    }

    iterator begin() {
        return iterator(table.begin(), table.front().begin(), table.end());
    }

    iterator end() {
        return iterator(table.end(), table.back().end(), table.end());
    }

    int getIndex(const Key& key) const {

        return std::hash<Key>()(key) % table.size();
    }
    
    int size() const {
        return this->count;
    }

    const_table_iterator getBucket(const Key& key) const {
        return table.cbegin() + getIndex(key);
    }

    table_iterator getBucket(const Key& key) {
        return table.begin() + getIndex(key);
    }

    float getLoadFactor() const {

        return float(count) / float(table.size());
    }

    bool shouldResize() const {
        return getLoadFactor() > MAX_LOAD_FACTOR;
    }

    void resize() {
        Table oldTable;
        oldTable.resize(size() * 2);
        std::swap(oldTable, table);

        this->count = 0;

        for (table_iterator bucket = oldTable.begin(); bucket != oldTable.end(); ++bucket) {

            for (bucket_iterator element = bucket.begin(); element != bucket.end(); ++element) {
                insert(element->first, element->second);
            }
        }
    }

    Hashmap() {
        table.resize(16);
    }

    void insert(const Key& key, const Value& value) {
        
        auto bucketIter = getBucket(key);
        
        for (auto it = bucketIter->begin(); it != bucketIter->end(); ++it) {
            
            if (it->first == key) {
                it->second == value;
            }
        }

        bucketIter->push_front(Item(key, value));
        count++;
    }

    Value& operator[](const Key& key) {
        auto bucket = getBucket(key);

        for (auto elem = bucket.begin(); elem != bucket.end(); ++elem) {

            if (elem->first == key) {
                return elem->second;
            }
        }
        
        if (this->shouldResize()) {
            this->resize();
        }

        bucket->push_front(Item(key, Value()));
        ++count;

        return bucket.front().second;
    }
    
    iterator find(const Key& key) {
        table_iterator bucket = getBucket(key);

        for (bucket_iterator elem = bucket->begin(); elem != bucket->end(); ++elem) {

            if (elem->first == key) {
                return iterator(bucket, elem, table.end());
            }
        }
        return this->end();
    }

    const_iterator find(const Key& key) const {
        const_table_iterator bucket = getBucket(key);

        for (const_bucket_iterator elem = bucket->cbegin(); elem != bucket->cend(); ++elem) {

            if (elem->first == key) {
                return const_iterator(bucket, elem, table.end());
            }
        }
        return this->cend();
    }

    bool contains(const Key& key) const {
        auto bucket = getBucket(key);

        for (auto elem = bucket->begin(); elem != bucket->end(); ++elem) {

            if (elem->first == key) {
                return true;
            }
        }
        return false;
    }

    void erase(const Key& key) {
        auto bucket = getBucket(key);

        for (auto elem = bucket->before_begin(); elem != bucket->end(); ++elem) {
            auto nextEl = elem;
            nextEl++;

            if (nextEl->first == key && nextEl != bucket->end()) {
                bucket->erase_after(elem);
                count--;
                return;
            }
        }
    }
};