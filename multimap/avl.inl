#ifndef AVL_IMPLEMENTATION
#define AVL_IMPLEMENTATION

#include "avl.hpp"

using namespace std;

template <typename Key, typename Value, typename Compare>
avl_tree<Key, Value, Compare>::avl_tree() {}

/* template <typename Key, typename Value>
avl_tree<Key, Value>::avl_tree(const std::list<std::pair<const Key, Value>> val) {
    this->root->container = val;
} */

template <typename Key, typename Value, typename Compare>
int avl_tree<Key, Value, Compare>::height() const {
    return height(root);
}

template <typename Key, typename Value, typename Compare>
int avl_tree<Key, Value, Compare>::height(const node* n) const {
    return (n) ? n->height : -1;
}

template <typename Key, typename Value, typename Compare>
int avl_tree<Key, Value, Compare>::balance_factor(const node* n) const {
    return (n) ? height(n->left) - height(n->right) : 0;
}

template <typename Key, typename Value, typename Compare>
int avl_tree<Key, Value, Compare>::balance_factor(iterator it) const {
    return balance_factor(it.current);
}

template <typename Key, typename Value, typename Compare>
typename avl_tree<Key, Value, Compare>::const_iterator avl_tree<Key, Value, Compare>::find(const Key& key) const {

    auto node = find(root, key);

    if (node) return const_iterator(node, node->container.begin());

    else { return end(); }
}

template <typename Key, typename Value, typename Compare>
typename avl_tree<Key, Value, Compare>::iterator avl_tree<Key, Value, Compare>::find(const Key& key) {

    auto node = find(root, key);

    if (node) return iterator(node, node->container.begin());

    else { return end(); }
}

template <typename Key, typename Value, typename Compare>
void avl_tree<Key, Value, Compare>::insert(const Key& key, const Value& val) {
    root = insert(root, key, val, nullptr);

    root->height = std::max(height(root->left), height(root->right)) + 1;
}

template <typename Key, typename Value, typename Compare>
void avl_tree<Key, Value, Compare>::erase(const Key& key) {
    root = erase(root, key);
    
    root->height = std::max(height(root->left), height(root->right)) + 1;
}

template <typename Key, typename Value, typename Compare>
void avl_tree<Key, Value, Compare>::erase(iterator position) {

    position.current->container.erase(position.element);
    this->elems_count--;

    if (position.current->container.size() == 0) {
        node* p = position.current->parent;
        node** to_rebalance;
        
        if (p && p->right == position.current) {
            p->right = erase_node(p->right);
            to_rebalance = (p->right) ? &p->right : &p;
        }
        else if (p && p->left == position.current) {
            p->left = erase_node(p->left);
            to_rebalance = (p->left) ? &p->left : &p;
        }
        else { 
            position.current = erase_node(position.current);
            to_rebalance = &root;
        }

        while (*to_rebalance) {

            (*to_rebalance)->height = std::max(height((*to_rebalance)->left), height((*to_rebalance)->right)) + 1;
            node **par = &(*to_rebalance)->parent;
            rebalance(*to_rebalance);
            to_rebalance = par;
        }
    }
}

template <typename Key, typename Value, typename Compare>
std::vector<pair<Key, Value>> avl_tree<Key, Value, Compare>::inorder_traversal() const {

    return inorder_traversal(root);
}

template <typename Key, typename Value, typename Compare>
int avl_tree<Key, Value, Compare>::size() const {
    return this->elems_count;
}

template <typename Key, typename Value, typename Compare>
int avl_tree<Key, Value, Compare>::count(const Key& k) const {

    auto node = find(root, k);

    if (node) return node->container.size();

    return 0;
}

template <typename Key, typename Value, typename Compare>
int avl_tree<Key, Value, Compare>::count(const_iterator it) const {

    return it.current->container.size();
}

template <typename Key, typename Value, typename Compare>
int avl_tree<Key, Value, Compare>::unique_keys() const {
    return this->nodes;
}

template <typename Key, typename Value, typename Compare>
avl_tree<Key, Value, Compare>::~avl_tree() {
    destroy(root);
}

template <typename Key, typename Value, typename Compare>
std::vector<pair<Key, Value>> avl_tree<Key, Value, Compare>::inorder_traversal(node* node) const {

    std::vector<pair<Key, Value>> vec;

    if (node && node->left) vec = inorder_traversal(node->left);

    vec.insert(vec.end(), node->container.cbegin(), node->container.cend());

    if (node->right) {
        std::vector<pair<Key, Value>> right_inorder = inorder_traversal(node->right);

        vec.insert(vec.end(), right_inorder.begin(), right_inorder.end());
    }

    return vec;
}

template <typename Key, typename Value, typename Compare>
void avl_tree<Key, Value, Compare>::rebalance(node*& startingNode) {

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

template <typename Key, typename Value, typename Compare>
typename avl_tree<Key, Value, Compare>::node* 
avl_tree<Key, Value, Compare>::find(node* current, const Key& key) const {

    if (!current) return nullptr;

    else if (Compare()(key, current->getKey()))/* key < current->getKey() */ return find(current->left, key);
    else if (Compare()(current->getKey(), key)) return find(current->right, key);

    else { return current; }
}

template <typename Key, typename Value, typename Compare>
typename avl_tree<Key, Value, Compare>::node* 
avl_tree<Key, Value, Compare>::insert(node* current, const Key& key, const Value& val, node* parent) {

    
    if (!current) {
        this->elems_count++;
        this->nodes++;
        return new node{std::list<pair<const Key, Value>>(1, std::make_pair(key, val)), parent};
    }
    else if (Compare()(key, current->getKey())) current->left = insert(current->left, key, val, current);
    else if (Compare()(current->getKey(), key)) current->right = insert(current->right, key, val, current);

    else { 
        current->container.push_front(std::make_pair(key, val));
        this->elems_count++;
        return current; 
    }

    current->height = std::max(height(current->left), height(current->right)) + 1;
    
    rebalance(current);
    
    return current;
}

template <typename Key, typename Value, typename Compare>
typename avl_tree<Key, Value, Compare>::node* avl_tree<Key, Value, Compare>::erase(node* current, const Key& key) {

    if (!current) return nullptr;

    if (Compare()(key, current->getKey())) current->left = erase(current->left, key);

    else if (Compare()(current->getKey(), key)) current->right = erase(current->right, key);

    else {
        current = erase_node(current);
    }
    // if ??
    if (current) current->height = std::max(height(current->left), height(current->right)) + 1;
    rebalance(current);
    return current;
}

template <typename Key, typename Value, typename Compare>
typename avl_tree<Key, Value, Compare>::node* avl_tree<Key, Value, Compare>::erase_node(node* current) {

    this->elems_count -= current->container.size();
    this->nodes -= 1;

    node* substitute;

    if (!current->left && !current->right) {
        delete current;
        substitute = nullptr;
    }
    else if (!current->left) {
        substitute = current->right;
        current->right->parent = current->parent;
        delete current;
    }
    else if (!current->right) {
        substitute = current->left;
        current->left->parent = current->parent;
        delete current;
    }
    else {
        substitute = extract_minimal_node(current, current);
        /* substitute->left = current->left;
        substitute->right = current->right;

        substitute->left->parent = substitute;
        substitute->right->parent = substitute;
        
        substitute->parent = current->parent; */
        delete current;
    }
    return substitute;
}

template <typename Key, typename Value, typename Compare>
typename avl_tree<Key, Value, Compare>::node* avl_tree<Key, Value, Compare>::extract_minimal_node(node* startingNode, node* parent) {

    node *current = startingNode->right;
    
    while (current && current->left) {
        //parent = current;
        current = current->left;
    }
    if (current->parent->left == current) {
        current->parent->left = current->right;
        
        current->right = startingNode->right;       
        current->right->parent = current;        
    }
    
    current->left = startingNode->left;
    current->left->parent = current;

    current->parent = startingNode->parent;
    //current->parent = parent;
    return current;
}

template <typename Key, typename Value, typename Compare>
typename avl_tree<Key, Value, Compare>::node* avl_tree<Key, Value, Compare>::find_minimal_node(node* startingNode) const {
    node* current = startingNode;

    while (current && current->left) {
        current = current->left;
    }
    return current;
}

template <typename Key, typename Value, typename Compare>
typename avl_tree<Key, Value, Compare>::node* avl_tree<Key, Value, Compare>::right_rotation(node* n) {
    std::cout << "right rot\n";
    auto new_root = n->left;
    auto rigth_subtree = new_root->right;

    new_root->right = n;
    n->left = rigth_subtree;

    auto parent = n->parent;
    n->parent = new_root;
    if (rigth_subtree) rigth_subtree->parent = n;
    new_root->parent = parent;

    if (parent && parent->right == n) parent->right = new_root;
    else if (parent && parent->left == n) parent->left = new_root;

    n->height = std::max(height(n->left), height(n->right)) + 1;

    new_root->height = std::max(height(new_root->left), height(new_root->right)) + 1;

    return new_root;
}

template <typename Key, typename Value, typename Compare>
typename avl_tree<Key, Value, Compare>::node* avl_tree<Key, Value, Compare>::left_rotation(node* n) {
    std::cout << "left rot\n";
    auto new_root = n->right;
    auto left_subtree = new_root->left;

    new_root->left = n;
    n->right = left_subtree;

    auto parent = n->parent;
    n->parent = new_root;
    if (left_subtree) left_subtree->parent = n;
    new_root->parent = parent;
    if (parent && parent->right == n) parent->right = new_root;
    else if (parent && parent->left == n) parent->left = new_root;

    n->height = std::max(height(n->left), height(n->right)) + 1;
    new_root->height = std::max(height(new_root->left), height(new_root->right)) + 1;
    
    //if (left_subtree) left_subtree->height = std::max(height(left_subtree->left), height(left_subtree->right)) + 1;
    return new_root;
}

template <typename Key, typename Value, typename Compare>
void avl_tree<Key, Value, Compare>::destroy(node* n) {

    if (!n) return;

    if (n->left) destroy(n->left);

    if (n->right) destroy(n->right);

    delete n;
}

template <typename Key, typename Value, typename Compare>
typename avl_tree<Key, Value, Compare>::iterator avl_tree<Key, Value, Compare>::upper_bound(const Key& key) {

    auto node = find_eq_or_greater(root, key);

    if (node && equal(key, node->getKey())) return ++iterator(node);

    return iterator(node);
}

template <typename Key, typename Value, typename Compare>
typename avl_tree<Key, Value, Compare>::const_iterator avl_tree<Key, Value, Compare>::upper_bound(const Key& key) const {
    auto node = find_eq_or_greater(root, key);

    if (node && equal(key, node->getKey())) return ++const_iterator(node);

    return const_iterator(node);
}

template <typename Key, typename Value, typename Compare>
typename avl_tree<Key, Value, Compare>::iterator avl_tree<Key, Value, Compare>::lower_bound(const Key& key) {
    
    return iterator(find_eq_or_greater(root, key));
}

template <typename Key, typename Value, typename Compare>
typename avl_tree<Key, Value, Compare>::const_iterator avl_tree<Key, Value, Compare>::lower_bound(const Key& key) const {
    return const_iterator(find_eq_or_greater(root, key));
}

template <typename Key, typename Value, typename Compare>
std::pair<typename avl_tree<Key, Value, Compare>::iterator, typename avl_tree<Key, Value, Compare>::iterator>
avl_tree<Key, Value, Compare>::equal_range(const Key& key) {

    auto node = find_eq_or_greater(root, key);

    if (node && equal(node->getKey(), key)) {
        auto first = iterator(node, node->container.begin());
        auto second = iterator(node, (--node->container.end()));

        ++second;
        return std::make_pair(first, second);
    }

    else { return std::make_pair(iterator(node), iterator(node)); }
}

template <typename Key, typename Value, typename Compare>
std::pair<typename avl_tree<Key, Value, Compare>::const_iterator, typename avl_tree<Key, Value, Compare>::const_iterator>
avl_tree<Key, Value, Compare>::equal_range(const Key& key) const {
    auto node = find_eq_or_greater(root, key);

    if (node && equal(node->getKey(), key)) {
        auto first = const_iterator(node, node->container.begin());
        auto second = const_iterator(node, (--node->container.end()));

        ++second;
        return std::make_pair(first, second);
    }

    else { return std::make_pair(const_iterator(node), const_iterator(node)); }
}

template <typename Key, typename Value, typename Compare>
typename avl_tree<Key, Value, Compare>::node* avl_tree<Key, Value, Compare>::find_eq_or_greater(node* n, const Key& key) const {

    if (!n) return nullptr;

    else if (/* n->getKey() > key */Compare()(key, n->getKey())) {
        auto node = find_eq_or_greater(n->left, key);

        return (node) ? node : n;
    }
    else if (equal(key, n->getKey())) {
        return n;
    }

    else { return find_eq_or_greater(n->right, key); }
}

template <typename Key, typename Value, typename Compare>
typename avl_tree<Key, Value, Compare>::const_iterator 

avl_tree<Key, Value, Compare>::cbegin() const {
    return const_iterator(find_minimal_node(root));
}

template <typename Key, typename Value, typename Compare>
typename avl_tree<Key, Value, Compare>::const_iterator
avl_tree<Key, Value, Compare>::begin() const {
    return const_iterator(find_minimal_node(root));
}

template <typename Key, typename Value, typename Compare>
typename avl_tree<Key, Value, Compare>::iterator

avl_tree<Key, Value, Compare>::begin() {
    auto it = iterator(find_minimal_node(root));
    return it;
}

template <typename Key, typename Value, typename Compare>
typename avl_tree<Key, Value, Compare>::const_iterator

avl_tree<Key, Value, Compare>::cend() const {
    return const_iterator(nullptr);
}

template <typename Key, typename Value, typename Compare>
typename avl_tree<Key, Value, Compare>::const_iterator

avl_tree<Key, Value, Compare>::end() const {
    return const_iterator(nullptr);
}

template <typename Key, typename Value, typename Compare>
typename avl_tree<Key, Value, Compare>::iterator

avl_tree<Key, Value, Compare>::end() {
    return iterator(nullptr);
}

template <typename Key, typename Value, typename Compare>
bool avl_tree<Key, Value, Compare>::equal(const Key& a, const Key& b) const {
    return !Compare()(a, b) && !Compare()(b, a);
}

#endif

