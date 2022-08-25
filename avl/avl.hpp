#ifndef AVL_TREE_HPP
#define AVL_TREE_HPP

#include <vector>
#include <exception>
#include <list>

#include <vector>

#include "type_traits.hpp"

template <typename Key, typename Value, typename Compare = std::less<Key>>
class avl_tree {

private:

    struct node {
        typedef std::list<std::pair<const Key, Value>> value_type;
        
        value_type container;

        node *parent;

        node *left{};
        node *right{};
        
        int height{};

        Key getKey() const {
            return container.front().first;
        }
    } *root = nullptr;

    int elems_count = 0;
    int nodes = 0;
public:

    avl_tree();
    //avl_tree(const std::list<std::pair<const Key, Value>> val);
    int height() const;
    
public:
    
    typedef typename std::list<std::pair<const Key, Value>>::iterator list_iterator;
    typedef typename std::list<std::pair<const Key, Value>>::const_iterator const_list_iterator;

    #include "avl_iterator.inl"

    typedef base_iterator<list_iterator, std::pair<const Key, Value>> iterator;
    typedef base_iterator<const_list_iterator, const std::pair<const Key, Value>> const_iterator;

    const_iterator cbegin() const;
    const_iterator begin() const;
    iterator begin();

    const_iterator cend() const;
    const_iterator end() const;
    iterator end();

    const_iterator find(const Key& key) const;
    void insert(const Key& key, const Value& val);
    void erase(const Key& key);
    std::vector<std::pair<Key, Value>> inorder_traversal() const;

    int size() const;
    int count(const Key& k) const;
    int count(const_iterator it) const;
    int unique_keys() const;

    iterator upper_bound(const Key& key);
    const_iterator upper_bound(const Key& key) const;

    iterator lower_bound(const Key& key);
    const_iterator const_lower_bound() const;
    const_iterator lower_bound(const Key& key) const;

    std::pair<iterator, iterator> equal_range(const Key& key);
    std::pair<const_iterator, const_iterator> equal_range(const Key& key) const;


    ~avl_tree();

    
private:

    int height(const node* n) const;

    int balance_factor(const node* n) const;

    std::vector<std::pair<Key, Value>> inorder_traversal(node* node) const;

    void rebalance(node*& startingNode);

    node* find(node* current, const Key& key) const;

    node* insert(node* current, const Key& key, const Value& val, node* parent);

    node* erase(node* current, const Key& key);

    node* extract_minimal_node(node* startingNode, node* parent);

    node* find_minimal_node(node* startingNode) const;

    node* right_rotation(node* n);

    node* left_rotation(node* n);

    node* find_eq_or_greater(node* n, const Key& key) const;

    void destroy(node* n);

    bool equal(const Key& a, const Key& b) const;

    //template <typename T, typename V>
    friend node* load_tree_helper(const std::vector<int>& v, int begin, int end, const std::string& path);
    
    friend void load_tree_binary(const std::string& path, avl_tree<int, std::string>& tr);
};

#include "avl.inl"

#endif