#ifndef AVL_IMPLEMENTATION
#define AVL_IMPLEMENTATION

#include "avl.hpp"

using namespace std;

template <typename Key, typename Value, typename Compare>
avl_tree<Key, Value, Compare>::avl_tree() {}

template <typename Key, typename Value, typename Compare>
typename avl_tree<Key, Value, Compare>::node* 
avl_tree<Key, Value, Compare>::mask(const node* n, short balance) {
    uintptr_t nn = reinterpret_cast<uintptr_t>(n);
    nn |= (balance + 1);
    return reinterpret_cast<node*>(nn);
}

template <typename Key, typename Value, typename Compare>
typename avl_tree<Key, Value, Compare>::node* 
avl_tree<Key, Value, Compare>::unmask(const node* n) {
    uintptr_t nn = reinterpret_cast<uintptr_t>(n);
    nn &= ~3;
    return reinterpret_cast<node*>(nn);
}

template <typename Key, typename Value, typename Compare>
bool avl_tree<Key, Value, Compare>::is_left_heavy(node* n) {
    return (reinterpret_cast<uintptr_t>(n) & 3) == 2;
}

template <typename Key, typename Value, typename Compare>
bool avl_tree<Key, Value, Compare>::is_right_heavy(node* n) {
    return (reinterpret_cast<uintptr_t>(n) & 3) == 0;
}

template <typename Key, typename Value, typename Compare>
int avl_tree<Key, Value, Compare>::height() const {
    return rec_height(root);
}

template <typename Key, typename Value, typename Compare>
size_t avl_tree<Key, Value, Compare>::rec_height(const node* n) const {

    return (n) ? std::max(rec_height(n->left), rec_height(n->right)) + 1 : -1;
}

template <typename Key, typename Value, typename Compare>
int avl_tree<Key, Value, Compare>::balance_factor(const node* n) const {
    //return (n) ? height(n->left) - height(n->right) : 0;
    return (reinterpret_cast<uintptr_t>(n) & 3) - 1;
}

template <typename Key, typename Value, typename Compare>
void avl_tree<Key, Value, Compare>::insert(const Key& key, const Value& val) {
    bool updateBalanceFactors = true;
    root = insert(root, key, val, updateBalanceFactors);

    //root->height = std::max(height(root->left), height(root->right)) + 1;
}

template <typename Key, typename Value, typename Compare>
void avl_tree<Key, Value, Compare>::erase(const Key& key) {
    bool updateBalanceFactors = true;
    root = erase(root, key, updateBalanceFactors);
    
    //root->height = std::max(height(root->left), height(root->right)) + 1;
}

template <typename Key, typename Value, typename Compare>
std::vector<pair<Key, Value>> avl_tree<Key, Value, Compare>::inorder_traversal() const {

    return inorder_traversal(unmask(root));
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

    if (node && node->left) vec = inorder_traversal(unmask(node->left));

    vec.insert(vec.end(), node->container.cbegin(), node->container.cend());

    if (node->right) {
        std::vector<pair<Key, Value>> right_inorder = inorder_traversal(unmask(node->right));

        vec.insert(vec.end(), right_inorder.begin(), right_inorder.end());
    }

    return vec;
}

/* template <typename Key, typename Value, typename Compare>
const typename avl_tree<Key, Value, Compare>::node* const
avl_tree<Key, Value, Compare>::find(const Key& key) const {
    return find(root, key);
} */

template <typename Key, typename Value, typename Compare>
typename avl_tree<Key, Value, Compare>::const_iterator 
avl_tree<Key, Value, Compare>::find(const Key& key) const {
    node* n = find(root, key);

    if (!n) return cend();

    return const_iterator(n, n->container.begin());
}

template <typename Key, typename Value, typename Compare>
typename avl_tree<Key, Value, Compare>::iterator
avl_tree<Key, Value, Compare>::find(const Key& key) {
    std::stack<node*> stack_;
    node* n = find(root, key, stack_);

    if (!n) return end();

    return iterator(n, n->container.begin(), std::move(stack_));
}

template <typename Key, typename Value, typename Compare>
typename avl_tree<Key, Value, Compare>::node* 
avl_tree<Key, Value, Compare>:: find(node* current, const Key& key, std::stack<node*>& parent_stack) const {
    node* unmasked = unmask(current);
    //std::cout << unmasked
    if (!current) return nullptr;
    
    else if (Compare()(key, unmasked->getKey()))/* key < current->getKey() */ {
        parent_stack.push(current);
        return find(unmasked->left, key, parent_stack);
    }
    else if (Compare()(unmasked->getKey(), key)) return find(unmasked->right, key, parent_stack);

    else { return unmasked/* current */; }
}

template <typename Key, typename Value, typename Compare>
typename avl_tree<Key, Value, Compare>::node* 
avl_tree<Key, Value, Compare>::find(node* current, const Key& key) const {
    node* unmasked = unmask(current);
    //std::cout << unmasked
    if (!current) return nullptr;
    
    else if (Compare()(key, unmasked->getKey()))/* key < current->getKey() */ return find(unmasked->left, key);
    else if (Compare()(unmasked->getKey(), key)) return find(unmasked->right, key);

    else { return unmasked/* current */; }
}
template <typename Key, typename Value, typename Compare>
void avl_tree<Key, Value, Compare>::fix_left_heavy(node*& startingNode) {
    node* start = unmask(startingNode);
    node* left = unmask(start->left);
    int balance_left = balance_factor(start->left);

    if (balance_left >= 0) {     // right rotation    
        startingNode = right_rotation(start);
        size_t right_child_balance = (startingNode->right->left) ? 1 : 0; // only needed after erasing a node
        startingNode->right = mask(startingNode->right, right_child_balance);
        startingNode = mask(startingNode, 0);
    } else { // left-right rotation
        size_t oldRootBalanceFactor = 0, pivotBalanceFactor = 0;
        //checking if the lastly inserted node (that causes disbalance) is left or right child - needed for updating balance factors correctly
        if ((unmask(left->right))->left) oldRootBalanceFactor = -1; 
        else if (unmask(left->right)->right) pivotBalanceFactor = 1;

        start->left = left_rotation(left);
        startingNode = right_rotation(start);

        startingNode->right = mask(startingNode->right, oldRootBalanceFactor);
        startingNode->left = mask(startingNode->left, pivotBalanceFactor);
        startingNode = mask(startingNode, 0);
    }
}

template <typename Key, typename Value, typename Compare>
void avl_tree<Key, Value, Compare>::fix_right_heavy(node*& startingNode) {
    node* start = unmask(startingNode);
    node* right = unmask(start->right);
    int balance_right = balance_factor(start->right);

    if (balance_right > 0) { // right-left rot
        size_t oldRootBalanceFactor = 0, pivotBalanceFactor = 0;
        if ((unmask(right->left))->right) oldRootBalanceFactor = 1; 
        else if ((unmask(right->left))->left) pivotBalanceFactor = -1;

        start->right = right_rotation(right);
        startingNode = left_rotation(start);

        std::cout << oldRootBalanceFactor << " " << pivotBalanceFactor << '\n';

        startingNode->left = mask(startingNode->left, oldRootBalanceFactor);
        startingNode->right = mask(startingNode->right, pivotBalanceFactor);
        startingNode = mask(startingNode, 0);
    } else { // left rot
        startingNode = left_rotation(start);
        short left_child_balance = (startingNode->left->right) ? -1 : 0; // only needed after erasing a node
        startingNode->left = mask(startingNode->left, left_child_balance);        
        startingNode = mask(startingNode, -1 * left_child_balance);
    }
}

template <typename Key, typename Value, typename Compare>
void avl_tree<Key, Value, Compare>::ensureLeftBalance(node*& current, bool &shouldRebalance) {
    node* unmasked = unmask(current); // current ptr holds the balance factor, unmask to get ptr to actual node

    if (shouldRebalance && is_left_heavy(current)) {
        fix_left_heavy(current); // rebalance
        shouldRebalance = 0;
    }

    else if (shouldRebalance && is_right_heavy(current)) {
        shouldRebalance = 0; // insert was in the shorter subtree of the two, no more updating of balance factors needed
        current = mask(unmasked, 0); // balance is now even
    }
    else if (shouldRebalance) { current = mask(unmasked, 1); } // update balance factor, left subtree is now taller
}

template <typename Key, typename Value, typename Compare>
void avl_tree<Key, Value, Compare>::ensureRightBalance(node*& current, bool &shouldRebalance) {
    node* unmasked = unmask(current); // current ptr holds the balance factor, unmask to get ptr to actual node
    
    //if (!shouldRebalance) return current;
    if (shouldRebalance && is_right_heavy(current)) {
        fix_right_heavy(current); // update balance factor
        shouldRebalance = 0;
    }
    
    else if (shouldRebalance && is_left_heavy(current)) {
        shouldRebalance = 0;
        current = mask(unmasked, 0);
    }
    else if (shouldRebalance) { current = mask(unmasked, -1); }
}

template <typename Key, typename Value, typename Compare>
typename avl_tree<Key, Value, Compare>::node*
//void
avl_tree<Key, Value, Compare>::insert(node* current, const Key& key, const Value& val, bool &shouldRebalance) {
    node* unmasked = unmask(current); // current ptr holds the balance factor, unmask to get ptr to actual node

    if (!current) {
        this->elems_count++;
        this->nodes++; // return new node with balance factor of 0
        return mask(new node{ std::list<pair<const Key, Value>>(1, std::make_pair(key, val)) }, 0);
    }
    else if (Compare()(key, unmasked->getKey())) {
        unmasked->left = insert(unmasked->left, key, val, shouldRebalance);
        ensureLeftBalance(current, shouldRebalance);
    }
    else if (Compare()(unmasked->getKey(), key)) {
        unmasked->right = insert(unmasked->right, key, val, shouldRebalance);
        ensureRightBalance(current, shouldRebalance);
    }
    else { 
        unmasked->container.push_front(std::make_pair(key, val));
        this->elems_count++;
    }    
    return current;
}

template <typename Key, typename Value, typename Compare>
void avl_tree<Key, Value, Compare>::ensureLeftBalanceErase(node*& current, bool &shouldRebalance) {

    node* unmasked = unmask(current);

    if (shouldRebalance && is_right_heavy(current)) {
        fix_right_heavy(current);
    }
    else if (shouldRebalance && is_left_heavy(current)) {
        current = mask(unmasked, 0);
    }
    else if (shouldRebalance) {
        current = mask(unmasked, -1);
        shouldRebalance = 0;
    }
}

template <typename Key, typename Value, typename Compare>
void avl_tree<Key, Value, Compare>::ensureRightBalanceErase(node*& current, bool &shouldRebalance) {
    node *unmasked = unmask(current);
    //removed from shorter subtree, should rebalance
    if (shouldRebalance && is_left_heavy(current)) {
        fix_left_heavy(current);
    }
    // removed from taller subtree, balance is now even
    else if (shouldRebalance && is_right_heavy(current)) {
        current = mask(unmasked, 0);
    }
    // update balance factor, left subtree is now taller
    // stop BF updating, as there are no height changes in upper nodes
    else if (shouldRebalance) {
        current = mask(unmasked, 1);
        shouldRebalance = 0;
    }
}

template <typename Key, typename Value, typename Compare>
typename avl_tree<Key, Value, Compare>::node* 
avl_tree<Key, Value, Compare>::erase(node* current, const Key& key, bool &shouldRebalance) {
    
    node* unmasked = unmask(current);
    if (!current) return nullptr;

    if (Compare()(key, unmasked->getKey())) {
        unmasked->left = erase(unmasked->left, key, shouldRebalance);
        ensureLeftBalanceErase(current, shouldRebalance);
    }
    else if (Compare()(unmasked->getKey(), key)) {
        unmasked->right = erase(unmasked->right, key, shouldRebalance);
        ensureRightBalanceErase(current, shouldRebalance);
    }

    else {
        current = erase_node(current);
    }
    return current;
}

template <typename Key, typename Value, typename Compare>
typename avl_tree<Key, Value, Compare>::node* avl_tree<Key, Value, Compare>::erase_node(node* current) {
    
    node* substitute;
    node* unmasked = unmask(current);

    this->elems_count -= unmasked->container.size();
    this->nodes -= 1;
    
    if (!unmasked->left && !unmasked->right) {
        delete unmasked;
        substitute = nullptr;
    }
    else if (!unmasked->left) {
        substitute = unmasked->right;
        delete unmasked;
    }
    else if (!unmasked->right) {
        substitute = unmasked->left;
        delete unmasked;
    }
    else {
        
        bool shouldUpdateBalanceFactors = 1;
        node* min = find_minimal_node(unmask(unmasked->right));

        substitute = new node { min->container };
        root = erase(root, min->getKey(), shouldUpdateBalanceFactors);
        
        substitute->right = unmasked->right, substitute->left = unmasked->left;
        substitute = mask(substitute, balance_factor(current));
        delete unmasked;
    }
    return substitute;
}

template <typename Key, typename Value, typename Compare>
typename avl_tree<Key, Value, Compare>::node* avl_tree<Key, Value, Compare>::find_minimal_node(node* startingNode) const {
    node* current = startingNode;

    while (current && current->left) {
        current = unmask(current->left);

    }
    return current;
}

template <typename Key, typename Value, typename Compare>
typename avl_tree<Key, Value, Compare>::node* avl_tree<Key, Value, Compare>::right_rotation(node* n) {
    std::cout << "right rot\n";

    auto new_root = unmask(n->left);
    auto rigth_subtree = new_root->right;

    new_root->right = n, 0;
    n->left = rigth_subtree;

    return new_root;
}

template <typename Key, typename Value, typename Compare>
typename avl_tree<Key, Value, Compare>::node* avl_tree<Key, Value, Compare>::left_rotation(node* n) {
    std::cout << "left rot\n";

    auto new_root = unmask(n->right);
    auto left_subtree = new_root->left;

    new_root->left = n;
    n->right = left_subtree;

    return new_root;
}

template <typename Key, typename Value, typename Compare>
void avl_tree<Key, Value, Compare>::destroy(node* n) {
    node* unmasked = unmask(n);
    if (!n) return;

    if (unmasked->left) destroy(unmasked->left);

    if (unmasked->right) destroy(unmasked->right);

    delete unmasked;
}

template <typename Key, typename Value, typename Compare>
typename avl_tree<Key, Value, Compare>::node* 
avl_tree<Key, Value, Compare>::find_eq_or_greater(node* n, const Key& key, std::stack<node*>& parent_stack) const {
    //std::cout << "pr pr\n";
    n = unmask(n);
    if (!n) return nullptr;

    else if (/* n->getKey() > key */Compare()(key, n->getKey())) {
        
        auto node = find_eq_or_greater(n->left, key, parent_stack);

        if (node) {
            parent_stack.push(n);
            return node;
        } else {
            return n;
        }
        //return (node) ? node : n;
    }
    else if (equal(key, n->getKey())) {
        return n;
    }

    else { return find_eq_or_greater(n->right, key, parent_stack); }
}

template <typename Key, typename Value, typename Compare>
bool avl_tree<Key, Value, Compare>::equal(const Key& a, const Key& b) const {
    return !Compare()(a, b) && !Compare()(b, a);
}

template <typename Key, typename Value, typename Compare>
typename avl_tree<Key, Value, Compare>::node* 
avl_tree<Key, Value, Compare>::find_rightmost(node* n) const {
    
    while (n && n->right) {
        //n = unmask(n);
        n = unmask(n->right);
    }
    return unmask(n);
}


template <typename Key, typename Value, typename Compare>
typename avl_tree<Key, Value, Compare>::const_iterator 
avl_tree<Key, Value, Compare>::cbegin() const {
    return const_iterator(unmask(root));
}

template <typename Key, typename Value, typename Compare>
typename avl_tree<Key, Value, Compare>::const_iterator 
avl_tree<Key, Value, Compare>::begin() const {
    return cbegin();
}

template <typename Key, typename Value, typename Compare>
typename avl_tree<Key, Value, Compare>::iterator 
avl_tree<Key, Value, Compare>::begin() {

    return iterator(unmask(root));
}

template <typename Key, typename Value, typename Compare>
typename avl_tree<Key, Value, Compare>::iterator 
avl_tree<Key, Value, Compare>::end() {
    node* r = find_rightmost(unmask(root));
    return iterator(r, r->container.end());
}


template <typename Key, typename Value, typename Compare>
typename avl_tree<Key, Value, Compare>::const_iterator 
avl_tree<Key, Value, Compare>::cend() const {
    node* r = find_rightmost(unmask(root));
    return const_iterator(r, r->container.end());
}

template <typename Key, typename Value, typename Compare>
typename avl_tree<Key, Value, Compare>::const_iterator 
avl_tree<Key, Value, Compare>::end() const {
    return cend();
}

template <typename Key, typename Value, typename Compare>
typename avl_tree<Key, Value, Compare>::const_iterator avl_tree<Key, Value, Compare>::lower_bound(const Key& key) const {
    std::stack<node*> parent_stack;
    auto node = find_eq_or_greater(root, key, parent_stack);

    if (!node) return end();
    
    return const_iterator(node, node->container.begin(), std::move(parent_stack));
}

template <typename Key, typename Value, typename Compare>
typename avl_tree<Key, Value, Compare>::const_iterator avl_tree<Key, Value, Compare>::upper_bound(const Key& key) const {
    std::stack<node*> parent_stack;
    auto node = find_eq_or_greater(root, key, parent_stack);

    if (!node) return end();

    if (node && equal(key, node->getKey())) {
        const_iterator toReturn = const_iterator(node, node->container.begin(), std::move(parent_stack));
        
        while (equal(toReturn->first, key)) ++toReturn;
        return toReturn;
    }

    return const_iterator(node, node->container.begin(), std::move(parent_stack));
}

template <typename Key, typename Value, typename Compare>
typename avl_tree<Key, Value, Compare>::iterator avl_tree<Key, Value, Compare>::lower_bound(const Key& key) {
    
    std::stack<node*> parent_stack;
    auto node = find_eq_or_greater(root, key, parent_stack);

    if (!node) return end();
        
    return iterator(node, node->container.begin(), std::move(parent_stack));
}

template <typename Key, typename Value, typename Compare>
typename avl_tree<Key, Value, Compare>::iterator avl_tree<Key, Value, Compare>::upper_bound(const Key& key) {
    std::stack<node*> parent_stack;
    auto node = find_eq_or_greater(root, key, parent_stack);

    if (!node) return end();

    if (node && equal(key, node->getKey())) {
        iterator toReturn = iterator(node, node->container.begin(), std::move(parent_stack));
        
        while (equal(toReturn->first, key)) ++toReturn;
        return toReturn;
    }

    return iterator(node, node->container.begin(), std::move(parent_stack));
}


template <typename Key, typename Value, typename Compare>
std::pair<typename avl_tree<Key, Value, Compare>::const_iterator, 
          typename avl_tree<Key, Value, Compare>::const_iterator> 
          
avl_tree<Key, Value, Compare>::equal_range(const Key& key) const {
    /* auto node = find_eq_or_greater(root, key);

    if (!node) return std::make_pair(end(), end());

    if (node && equal(node->getKey(), key)) {
        auto first = const_iterator(node, node->container.begin());
        auto second = const_iterator(node, (--node->container.end()));

        ++second;
        return std::make_pair(first, second);
    }

    else { return std::make_pair(const_iterator(node), const_iterator(node)); } */

    return std::make_pair(lower_bound(key), upper_bound(key));
}


template <typename Key, typename Value, typename Compare>
std::pair<typename avl_tree<Key, Value, Compare>::iterator, 
          typename avl_tree<Key, Value, Compare>::iterator>  

avl_tree<Key, Value, Compare>::equal_range(const Key& key) {
    /* auto node = find_eq_or_greater(root, key);

    if (!node) return std::make_pair(end(), end());

    if (node && equal(node->getKey(), key)) {
        auto first = iterator(node, node->container.begin());
        auto second = iterator(node, (--node->container.end()));

        ++second;
        return std::make_pair(first, second);
    }

    else { return std::make_pair(iterator(node), iterator(node)); } */

    return std::make_pair(lower_bound(key), upper_bound(key));
}

#endif

