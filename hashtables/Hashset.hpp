#ifndef HASHSET_HPP
#define HASHSET_HPP

#include <vector>
#include <forward_list>

#include "utils\type_traits.hpp"

template <typename Value>
class Hashset {
    
private:
    typedef std::forward_list<Value> Bucket;
    typedef std::vector<Bucket> Table;

    typedef typename std::forward_list<Value>::const_iterator const_bucket_iterator;
    typedef typename std::forward_list<Value>::iterator bucket_iterator;

    typedef typename std::vector<Bucket>::const_iterator const_table_iterator;
    typedef typename std::vector<Bucket>::iterator table_iterator;

    Table table;

    int getIndex(const Value& value) const;
    const_table_iterator getBucket(const Value& value) const;
    table_iterator getBucket(const Value& value);

    const float MAX_LOAD_FACTOR = 0.7f;
    int count = 0;

    struct const_iterator {
    
    private:
        const_table_iterator bucket;
        const_bucket_iterator element;
        const_table_iterator end;

        friend class Hashset;

        void findNext() {

            while (bucket != end && element == bucket->cend()) {
                ++bucket;
                if (bucket != end) {

                    element = bucket->cbegin();
                }
            }
        }

        const_iterator(const_table_iterator b, const_bucket_iterator e, const_table_iterator end)
            : bucket(b), element(e), end(end)
        {
            findNext();
        }

    public:
        
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

        const Value& operator*() const {
            return *element;
        }

        
    };

public:

    typedef const_iterator iterator; // non-const iterator should behave like the const- one
                                     // otherwise changing elements in the container would invalidate the whole container
    

    iterator begin() {
        return const_iterator(table.begin(), table.front().begin(), table.end());
    }

    iterator end() {
        return const_iterator(table.end(), table.back().end(), table.end());
    }

    const_iterator cbegin() const {
        return const_iterator(table.cbegin(), table.front().cbegin(), table.cend());
    }

    const_iterator cend() const {
        return const_iterator(table.cend(), table.back().cend(), table.cend());
    }

    Hashset();
    
    float getLoadFactor() const;
    bool shouldResize() const;
    int size() const;

    void resize();
    
    const_iterator find(const Value& value) const;
    //iterator find(const Value& value);

    std::pair<bucket_iterator, bool> insert(const Value& value);
    void erase(const Value& value);
};

#include "Hashset.inl"

#endif