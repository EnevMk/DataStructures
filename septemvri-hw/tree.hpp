#ifndef TREE_HPP
#define TREE_HPP

#include <vector>
#include <queue>
#include <fstream>
#include <stdexcept>
#include <cstring>
#include <iostream>
#include <string>
#include <algorithm>

#define NO_CHILDREN_NODES INT32_MIN

using namespace std;

#include "sortedVector.hpp"

//struct TreeNode;

template <typename T>
struct ptr_less {

    bool operator() (const T a, const T b) {
        return a->value < b->value;
    }
};

struct TreeNode {

private:
    int value;

    TreeNode();
    TreeNode(int val);
    SortedVector<TreeNode*, ptr_less<const TreeNode*>> subordinates;

    void loadNode(std::ifstream& is);
    SortedVector<TreeNode*, ptr_less<const TreeNode*>> loadChildren(std::ifstream& is, std::queue<TreeNode*>& q);

    /// binary search
    TreeNode* findChildNode(int value) const;
    //TreeNode* findChildNode(int value);

    //~TreeNode();
    friend class Tree;
    friend struct ptr_less<const TreeNode*>;
};

struct Tree {

private:
    TreeNode root;
    
    /* const  */TreeNode* find(int value, const TreeNode* startingNode) const;
    //TreeNode* find(int value, const TreeNode* startingNode);

    void destroy(TreeNode* node);
    bool contains(const TreeNode* container, const TreeNode* obj) const;

    std::vector<const TreeNode*> getMatchingRoots(int value); // returns the root of obj, if obj is contained in container
    std::vector<const TreeNode*> matchingRootsHelper(int val, const TreeNode* toCheck, std::vector<const TreeNode*>& matching);

    /// returns vector of roots of all subtrees equivalent to obj;
    std::vector<const TreeNode*> getAllSubtrees(const Tree& obj);

    void getSumOfRemainingNodes(const TreeNode* container, const TreeNode* obj, int &sumOfRemaining);

    TreeNode* getParentNode(int childNodeValue, TreeNode* startingNode);
public:
    Tree(std::ifstream& is);
    
    
    ~Tree();

    string toString() const;

    bool contains(const Tree& obj);

    bool remove(const Tree& obj);

};


#endif