#ifndef AVL_TREE_HPP
#define AVL_TREE_HPP

#include <vector>
#include <exception>
#include <list>

#include "type_traits.hpp"

template <typename Key, typename Value>
class avl_tree {

private:

    struct node {
        typedef std::list<std::pair<Key, Value>> value_type;
        
        value_type container;

        /* Key key;
        Value data; */
        
        node *left{};
        node *right{};
        
        int height{};

        Key getKey() const {
            return container.front().first;
        }
    } *root = nullptr;

    int count = 0;
public:

    avl_tree();
    int height() const;

private:
    int height(const node* n) const;
    int balance_factor(const node* n) const;

    typedef typename std::list<std::pair<Key, Value>>::iterator list_iterator;
    typedef typename std::list<std::pair<Key, Value>>::const_iterator const_list_iterator;

public:
    
    template<typename ListIterator = list_iterator, typename Type = std::pair<const Key, Value>>
    class base_iterator {

    private:

        ListIterator element;

        node* toShow;

        node* current;
        node* next;

        int pointersToRemove = 0;

        base_iterator(node* n): current(n) {
            toShow = findNextNode();
            if (toShow) element = toShow->container.begin();
        }

        base_iterator(node* n, ListIterator elem): current(n), toShow(n), element(elem) {}

        friend class avl_tree;

        void findNext() {


            if (element == toShow->container.end()) {
                toShow = findNextNode();
                if (toShow) element = toShow->container.begin();
            }
        }

        node* findNextNode() {

            if(!current) return nullptr;
            
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
                } else {
                    next->right = nullptr;
                    pointersToRemove--;

                    node* temp = current;
                    current = current->right;
                    return temp;
                }

                return findNextNode();
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
            while (pointersToRemove) {
                toShow = findNextNode();
            }
        }
    };

    typedef base_iterator<list_iterator, std::pair<const Key, Value>> iterator;
    typedef base_iterator<const_list_iterator, const std::pair<Key, Value>> const_iterator;

    const_iterator cbegin() const {
        return const_iterator(root);
    }

    const_iterator begin() const {
        return const_iterator(root);
    }

    iterator begin() {
        return iterator(root);
    }

    const_iterator cend() const {
        auto last = find_rightmost();
        return const_iterator(last->right, last->container.cend());
    }

    const_iterator end() const {
        auto last = find_rightmost();
        return const_iterator(last->right, last->container.cend());
    }

    iterator end() {
        auto last = find_rightmost();
        return iterator(last->right, last->container.end());
    }

    typename avl_tree<Key, Value>::iterator find(const Key& key) const;
    void insert(const Key& key, const Value& value);
    void erase(const Key& key);
    std::vector<std::pair<Key, Value>> inorder_traversal() const;
    int size() const;

    typename avl_tree<Key, Value>::iterator upper_bound(const Key& key);
    ~avl_tree();

    /* friend class iterator;
    friend class const_iterator; */
private:

    std::vector<std::pair<Key, Value>> inorder_traversal(node* node) const;

    void rebalance(node*& startingNode);

    node* find(node* current, const Key& key) const;

    node* insert(node* current, const Key& key, const Value& value);

    node* erase(node* current, const Key& key);

    node* find_minimal_node(node* startingNode, node* parent);

    node* find_rightmost() const;

    node* right_rotation(node* n);

    node* left_rotation(node* n);

    void destroy(node* n);
};

#include "avl.inl"

#endif