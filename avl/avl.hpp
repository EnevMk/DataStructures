#ifndef AVL_TREE_HPP
#define AVL_TREE_HPP

#include <vector>
#include <exception>
#include <list>
#include <functional>

#include "type_traits.hpp"

template <typename Key, typename Value>
class avl_tree {

private:

    struct node {
        typedef std::list<std::pair<Key, Value>> value_type;
        
        value_type container;

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

public:
    
    typedef typename std::list<std::pair<Key, Value>>::iterator list_iterator;
    typedef typename std::list<std::pair<Key, Value>>::const_iterator const_list_iterator;

    #include "avl_iterator.inl"

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

    iterator upper_bound(const Key& key);
    const_iterator upper_bound(const Key& key) const;

    iterator lower_bound(const Key& key);
    const_iterator lower_bound(const Key& key) const;

    std::pair<iterator, iterator> equal_range(const Key& key);
    typename avl_tree<Key, Value>::const_iterator equal_range(const Key& key) const;




    ~avl_tree();

    
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