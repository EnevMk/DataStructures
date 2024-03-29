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
#include <optional>

#define NO_CHILDREN_NODES INT32_MIN
#define WHITESPACES_EXCESS 30

using namespace std;

#include "sortedVector.hpp"

template <typename T>
struct ptr_less {

    bool operator() (const T a, const T b) {
        return a->value < b->value;
    }
};

struct TreeNode {

private:
    
    TreeNode* parent;
    int value;

    TreeNode();
    TreeNode(int val);
    TreeNode(int val, TreeNode* const parent);
    SortedVector<TreeNode*, ptr_less<const TreeNode*>> subordinates;

    void loadNode(std::ifstream& is);
    SortedVector<TreeNode*, ptr_less<const TreeNode*>> loadChildren(std::ifstream& is, std::queue<TreeNode*>& q, TreeNode* parent);

    TreeNode* getParentNode();

    /// binary search
    TreeNode* findChildNode(int value) const;
    int findChildNodeIndex(int value) const;

    TreeNode* clone(TreeNode* parent) const;
    
    friend class Tree;
    friend struct ptr_less<const TreeNode*>;
    friend class SortedVector<TreeNode*, ptr_less<const TreeNode*>>;
};

struct Tree {

private:
    TreeNode root;
    
    /* const  */TreeNode* find(int value, const TreeNode* startingNode) const;
    //TreeNode* find(int value, const TreeNode* startingNode);

    Tree clone() const;

    void destroy(TreeNode* node);
    static bool contains(const TreeNode* container, const TreeNode* obj);

    std::vector<const TreeNode*> getMatchingRoots(int value) const; // returns the root of obj, if obj is contained in container
    std::vector<const TreeNode*> matchingRootsHelper(int val, const TreeNode* toCheck, std::vector<const TreeNode*>& matching) const;

    void getDeepestSubtree(TreeNode* container, const TreeNode *obj, TreeNode*& current, int &level, int currentLevel);

    /// returns vector of roots of all subtrees equivalent to obj;
    std::vector<const TreeNode*> getAllSubtrees(const Tree& obj);

    void getSumOfRemainingNodes(const TreeNode* container, const TreeNode* obj, std::optional<int> &sumOfRemaining, bool &flag);
    
    std::optional<int> getSumOfRemainingNodes(const TreeNode* container, const TreeNode* obj);
    


    TreeNode* getParentNode(int childNodeValue, TreeNode* startingNode);
public:
    Tree();
    Tree(const Tree& obj);
    Tree(Tree&& obj);

    void loadFromStream(std::ifstream& is);
    Tree(std::ifstream& is);
    
    ~Tree();

    string toString() const;

    bool contains(const Tree& obj) const;

    bool remove(const Tree& obj);

    void saveToStream(std::ofstream& os) const;

};


#endif