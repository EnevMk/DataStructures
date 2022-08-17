#ifndef AVL_TREE_HPP
#define AVL_TREE_HPP

#include <vector>
#include <exception>

template <typename Key, typename Value>
class avl_tree {

private:

    struct node {
        typedef std::pair<Key, Value> value_type;
        
        value_type pair;

        /* Key key;
        Value data; */
        
        node *left{};
        node *right{};
        
        int height{};
    } *root = nullptr;

    int nodesCount = 0;
public:

    avl_tree();
    int height() const;

private:
    int height(const node* n) const;
    int balance_factor(const node* n) const;

public:
    

    class iterator {

    private:

        node* toShow;

        node* current;
        node* next;

        int pointersToRemove = 0;

        iterator(node* n): current(n), toShow(n) {
            //toShow = findNext();
        }

        friend class avl_tree;

        node* findNext() {

            
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

                return findNext();
            }
        }
    public:
        std::pair<const Key, Value>& operator*() {
            return reinterpret_cast<std::pair<const Key, Value>&>(toShow->pair);
        }

        const std::pair<Key, Value>& operator*() const {
            return toShow->pair;
        }

        std::pair<const Key, Value>* operator->() {
            return &reinterpret_cast<std::pair<const Key, Value>&>(toShow->pair);
        }

        const std::pair<Key, Value>* operator->() const {
            return &(toShow->pair);
        }

        bool operator==(const iterator& other) {
            return (this->current == other.current && this->toShow == other.toShow);
        }

        bool operator!=(const iterator& other) {
            return !(*this == other);
        }

        iterator& operator++() {

            toShow = findNext();

            return *this;
        }

        ~iterator() {
            while (pointersToRemove) {
                toShow = findNext();
            }
        }
    };

    iterator begin() {
        iterator toReturn(root);
        toReturn.toShow = toReturn.findNext();

        return toReturn;
    }

    iterator end() const {
        return iterator(findRightMost()->right);
    }

    typename avl_tree<Key, Value>::iterator find(const Key& key) const;
    void insert(const Key& key, const Value& value);
    void erase(const Key& key);
    std::vector<Value> inorder_traversal() const;
    int size() const;
    ~avl_tree();

    friend class iterator;
private:

    std::vector<Value> inorder_traversal(node* node) const;

    void rebalance(node*& startingNode);

    node* find(node* current, const Key& key) const;

    node* insert(node* current, const Key& key, const Value& value);

    node* erase(node* current, const Key& key);

    node* find_minimal_node(node* startingNode, node* parent);

    node* findRightMost() const;

    node* right_rotation(node* n);

    node* left_rotation(node* n);

    void destroy(node* n);
};

#include "avl.inl"

#endif