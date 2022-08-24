#ifndef AVL_IMPLEMENTATION
#define AVL_IMPLEMENTATION

#include "avl.hpp"

using namespace std;

template <typename Key, typename Value>
avl_tree<Key, Value>::avl_tree() {}

/* template <typename Key, typename Value>
avl_tree<Key, Value>::avl_tree(const std::list<std::pair<const Key, Value>> val) {
    this->root->container = val;
} */

template <typename Key, typename Value>
int avl_tree<Key, Value>::height() const {
    return height(root);
}

template <typename Key, typename Value>
int avl_tree<Key, Value>::height(const node* n) const {
    return (n) ? n->height : 0;
}

template <typename Key, typename Value>
int avl_tree<Key, Value>::balance_factor(const node* n) const {
    return (n) ? height(n->left) - height(n->right) : 0;
}

template <typename Key, typename Value>
typename avl_tree<Key, Value>::iterator avl_tree<Key, Value>::find(const Key& key) const {

    auto node = find(root, key);

    if (node) return iterator(node, node->container.begin());

    else { return end(); }
}

template <typename Key, typename Value>
void avl_tree<Key, Value>::insert(const Key& key, const Value& val) {
    root = insert(root, key, val, nullptr);

    root->height = std::max(height(root->left), height(root->right)) + 1;
}

template <typename Key, typename Value>
void avl_tree<Key, Value>::erase(const Key& key) {
    root = erase(root, key);
    
    root->height = std::max(height(root->left), height(root->right)) + 1;
}

template <typename Key, typename Value>
std::vector<pair<Key, Value>> avl_tree<Key, Value>::inorder_traversal() const {

    return inorder_traversal(root);
}

template <typename Key, typename Value>
int avl_tree<Key, Value>::size() const {
    return this->elems_count;
}

template <typename Key, typename Value>
int avl_tree<Key, Value>::count(const Key& k) const {

    auto node = find(root, k);

    if (node) return node->container.size();

    return 0;
}

template <typename Key, typename Value>
int avl_tree<Key, Value>::count(const_iterator it) const {

    return it.current->container.size();
}

template <typename Key, typename Value>
int avl_tree<Key, Value>::unique_keys() const {
    return this->nodes;
}

template <typename Key, typename Value>
avl_tree<Key, Value>::~avl_tree() {
    destroy(root);
}

template <typename Key, typename Value>
std::vector<pair<Key, Value>> avl_tree<Key, Value>::inorder_traversal(node* node) const {

    std::vector<pair<Key, Value>> vec;

    if (node && node->left) vec = inorder_traversal(node->left);

    //vec.push_back(node->container.second);
    vec.insert(vec.end(), node->container.cbegin(), node->container.cend());

    if (node->right) {
        std::vector<pair<Key, Value>> right_inorder = inorder_traversal(node->right);

        vec.insert(vec.end(), right_inorder.begin(), right_inorder.end());
    }

    return vec;
}

template <typename Key, typename Value>
void avl_tree<Key, Value>::rebalance(node*& startingNode) {

    int balance = balance_factor(startingNode);

    if (balance > 1) {
        int balance_left = balance_factor(startingNode->left);

        if (balance_left >= 0) {
            startingNode = right_rotation(startingNode);
        } else {
            startingNode->left = left_rotation(startingNode->left);
            startingNode = right_rotation(startingNode);
        }
    } else if (balance < -1) {
        int balance_right = balance_factor(startingNode->right);

        if (balance_right > 0) {
            startingNode->right = right_rotation(startingNode->right);
            startingNode = left_rotation(startingNode);
        } else {
            startingNode = left_rotation(startingNode);
        }
    }
}

template <typename Key, typename Value>
typename avl_tree<Key, Value>::node* avl_tree<Key, Value>::find(node* current, const Key& key) const {

    if (!current) return nullptr;

    else if (key < current->getKey()) return find(current->left, key);
    else if (key > current->getKey()) return find(current->right, key);

    else { return current; }
}

template <typename Key, typename Value>
typename avl_tree<Key, Value>::node* avl_tree<Key, Value>::insert(node* current, const Key& key, const Value& val, node* parent) {

    
    if (!current) {
        this->elems_count++;
        this->nodes++;
        return new node{std::list<pair<const Key, Value>>(1, std::make_pair(key, val)), parent};
    }
    else if (key < current->getKey()) current->left = insert(current->left, key, val, current);
    else if (key > current->getKey()) current->right = insert(current->right, key, val, current);

    else { 
        current->container.push_front(std::make_pair(key, val));
        this->elems_count++;
        return current; 
    }

    current->height = std::max(height(current->left), height(current->right)) + 1;
    
    rebalance(current);
    
    return current;
}

template <typename Key, typename Value>
typename avl_tree<Key, Value>::node* avl_tree<Key, Value>::erase(node* current, const Key& key) {

    if (!current) return nullptr;

    if (key < current->getKey()) current->left = erase(current->left, key);

    else if (key > current->getKey()) current->right = erase(current->right, key);

    else {
        this->elems_count -= current->container.size();
        this->nodes -= current->container.size();
        if (!current->left && !current->right) {
            delete current;
            return nullptr;
        }
        else if (!current->left) {
            node* temp = current->right;
            delete current;
            return temp;
        }
        else if (!current->right) {
            node* temp = current->left;
            delete current;
            return temp;
        }
        else {
            auto min = extract_minimal_node(current->right, current);
            min->left = current->left;
            min->right = current->right;
            delete current;
            return min;
        }
    }

    current->height = std::max(height(current->left), height(current->right)) + 1;
    rebalance(current);
    return current;
}

template <typename Key, typename Value>
typename avl_tree<Key, Value>::node* avl_tree<Key, Value>::extract_minimal_node(node* startingNode, node* parent) {

    node *current = startingNode;
    
    while (current && current->left) {
        parent = current;
        current = current->left;
    }
    parent->left = current->right;
    return current;
}

template <typename Key, typename Value>
typename avl_tree<Key, Value>::node* avl_tree<Key, Value>::find_minimal_node(node* startingNode) const {
    node* current = startingNode;

    while (current && current->left) {
        current = current->left;
    }
    return current;
}

template <typename Key, typename Value>
typename avl_tree<Key, Value>::node* avl_tree<Key, Value>::right_rotation(node* n) {
    auto new_root = n->left;
    auto rigth_subtree = new_root->right;

    new_root->right = n;
    n->left = rigth_subtree;

    auto parent = n->parent;
    n->parent = new_root;
    if (rigth_subtree) rigth_subtree->parent = n;
    new_root->parent = parent;

    n->height = std::max(height(n->left), height(n->right)) + 1;

    new_root->height = std::max(height(new_root->left), height(new_root->right)) + 1;

    return new_root;
}

template <typename Key, typename Value>
typename avl_tree<Key, Value>::node* avl_tree<Key, Value>::left_rotation(node* n) {
    
    auto new_root = n->right;
    auto left_subtree = new_root->left;

    new_root->left = n;
    n->right = left_subtree;

    auto parent = n->parent;
    n->parent = new_root;
    if (left_subtree) left_subtree->parent = n;
    new_root->parent = parent;

    n->height = std::max(height(n->left), height(n->right)) + 1;
    new_root->height = std::max(height(new_root->left), height(new_root->right)) + 1;

    return new_root;
}

template <typename Key, typename Value>
void avl_tree<Key, Value>::destroy(node* n) {

    if (!n) return;

    if (n->left) destroy(n->left);

    if (n->right) destroy(n->right);

    delete n;
}

template <typename Key, typename Value>
typename avl_tree<Key, Value>::iterator avl_tree<Key, Value>::upper_bound(const Key& key) {

    auto node = find_eq_or_greater(root, key);

    if (node && node->getKey() == key) return ++iterator(node);

    return iterator(node);
}

template <typename Key, typename Value>
typename avl_tree<Key, Value>::const_iterator avl_tree<Key, Value>::upper_bound(const Key& key) const {
    auto node = find_eq_or_greater(root, key);

    if (node && node->getKey() == key) return ++const_iterator(node);

    return const_iterator(node);
}

template <typename Key, typename Value>
typename avl_tree<Key, Value>::iterator avl_tree<Key, Value>::lower_bound(const Key& key) {
    
    return iterator(find_eq_or_greater(root, key));
}

template <typename Key, typename Value>
typename avl_tree<Key, Value>::const_iterator avl_tree<Key, Value>::lower_bound(const Key& key) const {
    return const_iterator(find_eq_or_greater(root, key));
}

template <typename Key, typename Value>
std::pair<typename avl_tree<Key, Value>::iterator, typename avl_tree<Key, Value>::iterator>
avl_tree<Key, Value>::equal_range(const Key& key) {

    auto node = find(root, key);

    if (node) {
        auto first = iterator(node, node->container.begin());
        auto second = iterator(node, (--node->container.end()));

        ++second;
        return std::make_pair(first, second);
    }

    else { return std::make_pair(end(), end()); }
}

template <typename Key, typename Value>
typename avl_tree<Key, Value>::const_iterator avl_tree<Key, Value>::equal_range(const Key& key) const {
    auto node = find(root, key);

    if (node) {
        auto first = const_iterator(node, node->container.begin());
        auto second = const_iterator(node, (--node->container.end()));

        ++second;
        return std::make_pair(first, second);
    }

    else { return std::make_pair(end(), end()); }
}

template <typename Key, typename Value>
typename avl_tree<Key, Value>::node* avl_tree<Key, Value>::find_eq_or_greater(node* n, const Key& key) const {

    if (!n) return nullptr;

    else if (n->getKey() >= key) {
        auto node = find_eq_or_greater(n->left, key);

        return (node) ? node : n;
    }

    else { return find_eq_or_greater(n->right, key); }
}

template <typename Key, typename Value>
typename avl_tree<Key, Value>::const_iterator 

avl_tree<Key, Value>::cbegin() const {
    return const_iterator(find_minimal_node(root));
}

template <typename Key, typename Value>
typename avl_tree<Key, Value>::const_iterator
avl_tree<Key, Value>::begin() const {
    return const_iterator(find_minimal_node(root));
}

template <typename Key, typename Value>
typename avl_tree<Key, Value>::iterator

avl_tree<Key, Value>::begin() {
    auto it = iterator(find_minimal_node(root));
    return it;
}

template <typename Key, typename Value>
typename avl_tree<Key, Value>::const_iterator

avl_tree<Key, Value>::cend() const {
    return const_iterator(nullptr);
}

template <typename Key, typename Value>
typename avl_tree<Key, Value>::const_iterator

avl_tree<Key, Value>::end() const {
    return const_iterator(nullptr);
}

template <typename Key, typename Value>
typename avl_tree<Key, Value>::iterator

avl_tree<Key, Value>::end() {
    return iterator(nullptr);
}

#endif

