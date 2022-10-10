#ifndef AVL_IMPLEMENTATION
#define AVL_IMPLEMENTATION

#include "avl.hpp"

using namespace std;

template <typename Key, typename Value, typename Compare>
avl_tree<Key, Value, Compare>::avl_tree() {}

template <typename Key, typename Value, typename Compare>
typename avl_tree<Key, Value, Compare>::node* 
avl_tree<Key, Value, Compare>::mask(node* n, short balance) {
    uintptr_t nn = reinterpret_cast<uintptr_t>(n);
    nn |= (balance + 1);
    return reinterpret_cast<node*>(nn);
}

template <typename Key, typename Value, typename Compare>
typename avl_tree<Key, Value, Compare>::node* 
avl_tree<Key, Value, Compare>::unmask(node* n) const {
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
    return height(root);
}

template <typename Key, typename Value, typename Compare>
int avl_tree<Key, Value, Compare>::height(const node* n) const {
    return (n) ? n->height : -1;
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
const typename avl_tree<Key, Value, Compare>::node* const
avl_tree<Key, Value, Compare>::find(const Key& key) const {
    return reinterpret_cast<const node* const>(find(root, key));
}

template <typename Key, typename Value, typename Compare>
typename avl_tree<Key, Value, Compare>::node* 
avl_tree<Key, Value, Compare>::find(node* current, const Key& key) const {
    node* unmasked = unmask(current);
    //std::cout << unmasked
    if (!current) return nullptr;
    
    else if (Compare()(key, unmasked->getKey()))/* key < current->getKey() */ return find(unmasked->left, key);
    else if (Compare()(unmasked->getKey(), key)) return find(unmasked->right, key);

    else { return /* unmasked */current; }
}
template <typename Key, typename Value, typename Compare>
void avl_tree<Key, Value, Compare>::fix_left_heavy(node*& startingNode) {
    node* start = unmask(startingNode);
    node* left = unmask(start->left);
    int balance_left = balance_factor(start->left);

    if (balance_left >= 0) {        
        startingNode = right_rotation(start);
        size_t right_child_balance = (startingNode->right->left) ? 1 : 0;
        startingNode->right = mask(startingNode->right, right_child_balance);
        startingNode = mask(startingNode, 0);
    } else {
        size_t oldRootBalanceFactor = 0, pivotBalanceFactor = 0;
        //checking if the lastly inserted node (that caused disbalance) is left or right child - needed for updating balance factors correctly
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

    if (balance_right > 0) {
        size_t oldRootBalanceFactor = 0, pivotBalanceFactor = 0;
        std::cout << "arajano\n";
        if ((unmask(right->left))->right) oldRootBalanceFactor = 1; 
        else if ((unmask(right->left))->left) pivotBalanceFactor = -1;

        start->right = right_rotation(right);
        startingNode = left_rotation(start);

        startingNode->left = mask(startingNode->left, oldRootBalanceFactor);
        startingNode->right = mask(startingNode->right, pivotBalanceFactor);
        startingNode = mask(startingNode, 0);
    } else {
        startingNode = left_rotation(start);
        short left_child_balance = (startingNode->left->right) ? -1 : 0; // only needed after erasing a node
        startingNode->left = mask(startingNode->left, left_child_balance);        
        startingNode = mask(startingNode, -1 * left_child_balance);
    }
}

template <typename Key, typename Value, typename Compare>
void avl_tree<Key, Value, Compare>::ensureLeftBalance(node*& current, bool &shouldRebalance) {
    node* unmasked = unmask(current); // current ptr holds the balance factor, unmask to get ptr to actual node

    //if (!shouldRebalance) return current;

    if (shouldRebalance && is_left_heavy(current)) {
        fix_left_heavy(current); // rebalance
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
typename avl_tree<Key, Value, Compare>::node* 
avl_tree<Key, Value, Compare>::erase(node* current, const Key& key, bool &shouldRebalance) {
    node* unmasked = unmask(current);
    if (!current) return nullptr;

    if (Compare()(key, unmasked->getKey())) {
        unmasked->left = erase(unmasked->left, key, shouldRebalance);

        if (shouldRebalance && is_right_heavy(current)) fix_right_heavy(current); // update balance factor
        else if (shouldRebalance && is_left_heavy(current)) current = mask(unmasked, 0);
        else if (shouldRebalance) { current = mask(unmasked, -1); }
    }
    else if (Compare()(unmasked->getKey(), key)) {
        unmasked->right = erase(unmasked->right, key, shouldRebalance);
        //removed from shorter subtree, should rebalance
        if (shouldRebalance && is_left_heavy(current)) fix_left_heavy(current);
        // removed from taller subtree, balance is now even
        else if (shouldRebalance && is_right_heavy(current)) current = mask(unmasked, 0); 
        else if (shouldRebalance) { current = mask(unmasked, 1); } // update balance factor, left subtree is now taller
    }

    else {
        current = erase_node(unmasked);
    }
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
        delete current;
    }
    else if (!current->right) {
        substitute = current->left;
        delete current;
    }
    else {
        node* unmasked = unmask(current);

        bool shouldUpdateBalanceFactors = 1;
        substitute = extract_minimal_node(unmasked, unmasked->right, unmasked, shouldUpdateBalanceFactors);
    }
    return substitute;
}

template <typename Key, typename Value, typename Compare>
typename avl_tree<Key, Value, Compare>::node* 
avl_tree<Key, Value, Compare>::extract_minimal_node(node* parent, node* childNode, node* startingNode, bool &updateBalance) {

    node* sub;
    node* unmasked = unmask(childNode);

    bool shouldExtract = !childNode->left;

    if (!shouldExtract) {
        unmasked->left = extract_minimal_node(unmasked, unmasked->left, startingNode, updateBalance);

        ensureRightBalance(childNode, updateBalance);
    }

    else if (shouldExtract && parent->left == childNode) {
        parent->left = unmasked->right;
        unmasked->right = startingNode->right;
    }
    else if (shouldExtract) unmasked->left = startingNode->left;

    return childNode;
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
bool avl_tree<Key, Value, Compare>::equal(const Key& a, const Key& b) const {
    return !Compare()(a, b) && !Compare()(b, a);
}

#endif

