#ifndef AVL_ITERATOR_IMPL
#define AVL_ITERATOR_IMPL

#include "avl.hpp"

template<typename ListIterator = list_iterator, typename Type = value_type>
class base_iterator {

    friend class avl_tree;
private:

    std::stack<node*> stack;
    node* current;
    ListIterator element;

    node* getLeftMost(node* n) {
        n = unmask(n);

        while (n && n->left) {
            stack.push(n);
            n = unmask(n->left);
        }
        //stack.pop();
        return n;
    }

    void getNext() {
        
        ++element;

        if (element == current->container.end()) {
            if (current->right) {
                current = getLeftMost(current->right);
            } 
            else if (stack.empty()) {
                return; 
            }
            else {
                
                current = stack.top();
                stack.pop();
            }
            element = current->container.begin();
        }
    }

    base_iterator(node* n) {
        current = getLeftMost(unmask(n));

        element = current->container.begin();
    }

    base_iterator(node* n, ListIterator listIter) :
        current(n),
        element(listIter)
    {   
        
        /* current = n;
        element = listIter; */
    }

    base_iterator(node* n, ListIterator listIter, std::stack<node*>&& parent_stack) :
        current(n),
        element(listIter),
        stack(parent_stack)
    {   

        /* current = n;
        element = listIter; */
    }

public:

    base_iterator& operator++() {
        getNext();
        return *this;
    }

    base_iterator operator++(int) {
        base_iterator it = *this;
        getNext();

        return it;
    }

    template <typename Q = Type>
    typename enable_if<!is_const<Q>::value, Q>::type * operator->() {
        return &reinterpret_cast<std::pair<const Key, Value>&>(*element);
    }

    template <typename Q = Type>
    typename enable_if<is_const<Q>::value, Q>::type * operator->() const {
        return &*element;
    }

    template <typename Q = Type>
    typename enable_if<is_const<Q>::value, Q>::type & operator*() const {
        return *element;
    }

    template <typename Q = Type>
    typename enable_if<!is_const<Q>::value, Q>::type & operator*() {
        return reinterpret_cast<std::pair<const Key, Value>&>(*element);
    }

    template <typename ListIter = ListIterator, typename T = value_type>
    bool operator==(base_iterator<ListIter, T> other) {
        return (current == other.current && element == other.element);
    }

    template <typename ListIter = ListIterator, typename T = value_type>
    bool operator!=(base_iterator<ListIter, T> other) {
        return !(*this == other);
    }

};

#endif