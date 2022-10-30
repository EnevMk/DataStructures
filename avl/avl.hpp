#ifndef AVL_TREE_HPP
#define AVL_TREE_HPP

#include <list>
#include <vector>
#include <stack>

//#include "type_traits.hpp"

#include <type_traits>
using std::enable_if;
using std::is_const;
using std::stack;

template <typename Key, typename Value, typename Compare = std::less<Key>>
class avl_tree {

private:
//public:

    typedef std::pair<const Key, Value> value_type;

    struct node {

        std::list<value_type> container;

        //node *parent;

        node *left{};
        node *right{};
        
        //int height{};

        Key getKey() const {
            return container.front().first;
        }
    } *root = nullptr;

    int elems_count = 0;
    int nodes = 0;
public:

    avl_tree();
    //avl_tree(const std::list<std::pair<const Key, Value>> val);

    avl_tree(const avl_tree&) = delete;
    avl_tree& operator=(const avl_tree&) = delete;
    int height() const;
    size_t rec_height(const node* n) const;
    
public:
    
    typedef typename std::list<value_type>::iterator list_iterator;
    typedef typename std::list<value_type>::const_iterator const_list_iterator;

    #include "avl_iterator.inl"

    typedef base_iterator<list_iterator, value_type> iterator;
    typedef base_iterator<const_list_iterator, const value_type> const_iterator;

    const_iterator cbegin() const;
    const_iterator begin() const;
    iterator begin();

    const_iterator cend() const;
    const_iterator end() const;
    iterator end();

    const_iterator lower_bound(const Key& key) const;
    const_iterator upper_bound(const Key& key) const;

    iterator lower_bound(const Key& key);
    iterator upper_bound(const Key& key);

    std::pair<const_iterator, const_iterator> equal_range(const Key& key) const;
    std::pair<iterator, iterator> equal_range(const Key& key);
    /* const_iterator find(const Key& key) const;
    iterator find(const Key& key); */
    
    const node* const find(const Key& key) const;

    void insert(const Key& key, const Value& val);

    void erase(const Key& key);
    //void erase(iterator position);

    std::vector<std::pair<Key, Value>> inorder_traversal() const;

    int size() const;
    int count(const Key& k) const;
    //int count(const_iterator it) const;
    int unique_keys() const;

    /* iterator upper_bound(const Key& key);
    const_iterator upper_bound(const Key& key) const;

    iterator lower_bound(const Key& key);
    //
    const_iterator const_lower_bound() const;
    const_iterator lower_bound(const Key& key) const;

    std::pair<iterator, iterator> equal_range(const Key& key);
    std::pair<const_iterator, const_iterator> equal_range(const Key& key) const; */


    ~avl_tree();

    //int balance_factor(iterator it) const;
private:
//public:
    int height(const node* n) const;

    int balance_factor(const node* n) const;

    std::vector<std::pair<Key, Value>> inorder_traversal(node* node) const;

    void rebalance(node*& startingNode);

    node* find(node* current, const Key& key) const;

    node* /* void  */insert(node* current, const Key& key, const Value& val, bool &shouldRebalance);

    void ensureLeftBalance(node*& current, bool &shouldRebalance);
    void ensureRightBalance(node*& current, bool &shouldRebalance);

    void ensureLeftBalanceErase(node*& current, bool &shouldRebalance);
    void ensureRightBalanceErase(node*& current, bool &shouldRebalance);
    void insert_node(node* toInsert, node** parent, bool left);

    node* erase(node* current, const Key& key, bool &shouldRebalance);

    node* extract_minimal_node(node* parent, node* childNode, node* startingNode, bool &updateBalance);

    node* find_minimal_node(node* startingNode) const;

    node* right_rotation(node* n);

    node* left_rotation(node* n);

    node* find_eq_or_greater(node* n, const Key& key) const;

    node* erase_node(node* current);

    void destroy(node* n);

    bool equal(const Key& a, const Key& b) const;

    node* find_rightmost(node* n) const;

    static node* mask(const node*, short balance);
    static node* unmask(const node*);
    bool is_left_heavy(node*);
    bool is_right_heavy(node*);

    void fix_left_heavy(node*&);
    void fix_right_heavy(node*&);


    //template <typename T, typename V>
    /* friend node* load_tree_helper(const std::vector<int>& v, int begin, int end, const std::string& path);
    
    friend void load_tree_binary(const std::string& path, avl_tree<int, std::string>& tr); */
};

#include "avl.inl"

#endif