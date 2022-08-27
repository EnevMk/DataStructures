#ifndef AVL_ITERATOR_IMPLEMENTATION
#define AVL_ITERATOR_IMPLEMENTATION

#include "avl.hpp"

template<typename ListIterator = list_iterator, typename Type = std::pair<const Key, Value>>
class base_iterator {

private:

    ListIterator element;

    node* current;
public:
    
private:

    base_iterator(node* n): current(n) {
        if (current) element = current->container.begin();
    }

    base_iterator(node* n, ListIterator elem)
    : current(n), element(elem) {}

    friend class avl_tree;

    void findNext() {


        if (element == current->container.end()) {
            findNextNode();
            if (current) element = current->container.begin();
        }
    }

    void findPrev() {
        if (element == current->container.begin()) {
            findPreviousNode();
            if (current) element = --(current->container.end());
        } else {
            --element;
        }
    }

    void findNextNode() {

        if(!current) return;

        if (current->right) {
            current = current->right;

            while (current->left) current = current->left;
        }
        else if (current->parent) {
            node* p = current->parent;

            while (p != nullptr && p->right == current) {
                current = p;
                p = p->parent;
            }
            current = p;
        }
    }

    void findPreviousNode() {

        if (!current) return;

        if (current->left) {
            current = current->left;

            while(current->right) current = current->right;
        }
        else if (current->parent) {
            node *p = current->parent;

            while (p && p->left == current) {
                current = p;
                p = p->parent;
            }
            current = p;
        }
    }

public:

    template <typename Q = Type>
    typename enable_if<!is_const<Q>::value, Q>::type & operator*() {
        return reinterpret_cast<std::pair<const Key, Value>&>(*element);
    }

    template <typename Q = Type>
    const typename enable_if<is_const<Q>::value, Q>::type & operator*() const {
        return *element;
    }

    template <typename Q = Type>
    typename enable_if<!is_const<Q>::value, Q>::type * operator->() {
        return &reinterpret_cast<std::pair<const Key, Value>&>(*element);
    }

    template <typename Q = Type>
    const typename enable_if<is_const<Q>::value, Q>::type * operator->() const {
        return &(*element);
    }

    template <typename T = ListIterator, typename Q = Type>
    bool operator==(const base_iterator<T, Q>& other) {
        
        return (!current && !other.current) || (this->element == other.element && this->current == other.current);
    }

    template <typename T = ListIterator, typename Q = Type>
    bool operator!=(const base_iterator<T, Q>& other) {
        return !(*this == other);
    }

    base_iterator& operator++() {

        ++element;
        findNext();

        return *this;
    }

    base_iterator operator++(int) {
        base_iterator toReturn(*this);

        ++*this;
        return toReturn;
    }

    base_iterator& operator--() {
        findPrev();
        return *this;
    }

    base_iterator operator--(int) {
        base_iterator res(*this);

        --*this;
        return res;
    }
};
#endif