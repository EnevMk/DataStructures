#ifndef AVL_IMPLEMENTATION
#define AVL_IMPLEMENTATION

#include "avl.hpp"

template <typename Key, typename Value>
avl_tree<Key, Value>::avl_tree() {}

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

    if (node) return iterator(node);

    else { return end(); }
}

template <typename Key, typename Value>
void avl_tree<Key, Value>::insert(const Key& key, const Value& value) {
    root = insert(root, key, value);

    root->height = std::max(height(root->left), height(root->right)) + 1;
}

template <typename Key, typename Value>
void avl_tree<Key, Value>::erase(const Key& key) {
    root = erase(root, key);
    
    root->height = std::max(height(root->left), height(root->right)) + 1;
}

template <typename Key, typename Value>
std::vector<Value> avl_tree<Key, Value>::inorder_traversal() const {

    return inorder_traversal(root);
}

template <typename Key, typename Value>
int avl_tree<Key, Value>::size() const {
    return this->nodesCount;
}

template <typename Key, typename Value>

avl_tree<Key, Value>::~avl_tree() {
    destroy(root);
}

template <typename Key, typename Value>
std::vector<Value> avl_tree<Key, Value>::inorder_traversal(node* node) const {

    std::vector<Value> vec;

    if (node && node->left) vec = inorder_traversal(node->left);

    vec.push_back(node->pair.second);

    if (node->right) {
        std::vector<Value> right_inorder = inorder_traversal(node->right);

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

    else if (key < current->pair.first) return find(current->left, key);
    else if (key > current->pair.first) return find(current->right, key);

    else { return current; }
}

template <typename Key, typename Value>
typename avl_tree<Key, Value>::node* avl_tree<Key, Value>::insert(node* current, const Key& key, const Value& value) {

    
    if (!current) {
        this->nodesCount++;
        return new node{std::make_pair(key, value)};
    }
    else if (key < current->pair.first) current->left = insert(current->left, key, value);
    else if (key > current->pair.first) current->right = insert(current->right, key, value);

    else { return current; }

    current->height = std::max(height(current->left), height(current->right)) + 1;
    
    rebalance(current);
    
    return current;
}

template <typename Key, typename Value>
typename avl_tree<Key, Value>::node* avl_tree<Key, Value>::erase(node* current, const Key& key) {

    if (!current) return nullptr;

    if (key < current->key) current->left = erase(current->left, key);

    else if (key > current->key) current->right = erase(current->right, key);

    else {
        this->nodesCount--;
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
            auto min = find_minimal_node(current->right, current);
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
typename avl_tree<Key, Value>::node* avl_tree<Key, Value>::find_minimal_node(node* startingNode, node* parent) {

    node *current = startingNode;
    
    while (current && current->left) {
        parent = current;
        current = current->left;
    }
    parent->left = current->right;
    return current;
}

template <typename Key, typename Value>
typename avl_tree<Key, Value>::node* avl_tree<Key, Value>::findRightMost() const {
    node* temp = root;

    while (temp && temp->right) {
        temp = temp->right;
    }

    return temp;
}

template <typename Key, typename Value>
typename avl_tree<Key, Value>::node* avl_tree<Key, Value>::right_rotation(node* n) {
    auto new_parent = n->left;
    auto rigth_subtree = new_parent->right;

    new_parent->right = n;
    n->left = rigth_subtree;

    n->height = std::max(height(n->left), height(n->right)) + 1;

    new_parent->height = std::max(height(new_parent->left), height(new_parent->right)) + 1;

    return new_parent;
}

template <typename Key, typename Value>
typename avl_tree<Key, Value>::node* avl_tree<Key, Value>::left_rotation(node* n) {
    auto new_parent = n->right;
    auto left_subtree = new_parent->left;

    new_parent->left = n;
    n->right = left_subtree;

    n->height = std::max(height(n->left), height(n->right)) + 1;
    new_parent->height = std::max(height(new_parent->left), height(new_parent->right)) + 1;

    return new_parent;
}

template <typename Key, typename Value>
void avl_tree<Key, Value>::destroy(node* n) {

    if (n->left) destroy(n->left);

    if (n->right) destroy(n->right);

    delete n;
}

#endif

