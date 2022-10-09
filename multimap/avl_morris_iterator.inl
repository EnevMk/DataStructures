#ifndef AVL_MORRIS_ITERATOR_IMPLEMENTATION
#define AVL_MORRIS_ ITERATOR_IMPLEMENTATION

#include "avl.hpp"


template<typename ListIterator = list_iterator, typename Type = std::pair<const Key, Value>>
class base_iterator {

private:

    ListIterator element;

    node* toShow;

    node* current;
    node* next;

public:
    int pointersToRemove = 0;
private:
    Key min;
    Key max;

    base_iterator(node* n): current(n) {
        toShow = findNextNode();
        if (toShow) element = toShow->container.begin();
    }

    base_iterator(node* n, ListIterator elem)
    : toShow(n), element(elem) {

        current = (n) ? n->right : nullptr;
    }

    base_iterator(node* n, const Key& bound, bool isMin)
    : current(n)
    {   
        if (isMin) {
            max = bound;
            toShow = findNextNodeLower();
            if (toShow) element = toShow->container.begin();
        } else {
            min = bound;
            toShow = findNextNodeUpper();
            if (toShow) element = toShow->container.begin();
        }
        
    }

    friend class avl_tree;

    void findNext() {


        if (element == toShow->container.end()) {
            toShow = findNextNode();
            if (toShow) element = toShow->container.begin();
        }
    }

    node* findNextNodeHelper(node* (base_iterator::*foo)()) {
        if (current->left == nullptr) {
            node* temp = current;
            current = current->right;
            return temp;
        } else {

            next = current->left;

            while (next->right && next->right != current) {
                next = next->right;
            }

            if (!next->right) {
                
                next->right = current;
                pointersToRemove++;

                

                current = current->left;
                return (this->*foo)();
            } else {
                next->right = nullptr;
                pointersToRemove--;

                node* temp = current;
                current = current->right;
                return temp;
            }
        }
    }

    node* findNextNode() {

        if(!current) return nullptr;

        return findNextNodeHelper(&findNextNode);
    }

    node* findNextNodeLower() {

        if(!current) return nullptr;
        
        while (current->getKey() < this->max) {
            current = current->right;
            return findNextNodeLower();
        }
        return findNextNodeHelper(&findNextNodeLower);
    }

    node* findNextNodeUpper() {
        if(!current) return nullptr;
        
        while (current->getKey() < this->min) {
            current = current->right;
            return findNextNodeUpper();
        }
        
        return findNextNodeHelper(&findNextNodeUpper);
    }
public:
    /* base_iterator(const base_iterator& obj)
    : element(obj.element), toShow(obj.toShow), current(obj.current), next(obj.next) {

        pointersToRemove = obj.pointersToRemove;
        min = obj.min;
        max = obj.max;
    } */

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
        
        /* std::cout << '\n' << (this->element == other.element) 
                    << (this->current == other.current) << (this->toShow == other.toShow) << '\n'; */
        return (this->element == other.element && this->current == other.current && this->toShow == other.toShow);
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

    ~base_iterator() {
        //std::cout << pointersToRemove << '\n';
        while (pointersToRemove > 0 && toShow) {
            toShow = findNextNode();
        }
    }
};
#endif