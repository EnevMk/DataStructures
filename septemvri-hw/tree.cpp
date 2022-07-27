#include "tree.hpp"

TreeNode::TreeNode() {}

TreeNode::TreeNode(int val) : value(val) {}

int getNextNode(std::ifstream& is) {

    string strValue;

    while (is.peek() == ' ') is.get();

    while (is.peek() != ' ' && is.peek() != '|') {
        strValue += is.get();
    }
    
    if (is.peek() == ' ') is.get();
    
    if (strValue.empty()) {
        
        return NO_CHILDREN_NODES;
    }
    
    return std::stoi(strValue);
}

void TreeNode::loadNode(std::ifstream& is) {
    
    std::queue<TreeNode*> elems;

    if (is.peek() == '|') is.get();

    int root = getNextNode(is);
    while (is.peek() == '|' || is.peek() == '\n') is.get();
    
    
    this->value = root;
    this->subordinates = loadChildren(is, elems);

    while (!elems.empty()) {

        int size = elems.size();
        
        while (size > 0) {

            TreeNode* node = elems.front();
            elems.pop();
            
            node->subordinates = loadChildren(is, elems);
            
            size--;
        }
    }
}

SortedVector<TreeNode*, ptr_less<const TreeNode*>> TreeNode::loadChildren(std::ifstream& is, std::queue<TreeNode*> &q) {

    SortedVector<TreeNode*, ptr_less<const TreeNode*>> childs;
    string strValue;

    if (is.peek() == '|') is.get();
    
    while (is.peek() != '|' && is.peek() != '\n') {
        int nodeValue = getNextNode(is);
        if (nodeValue == NO_CHILDREN_NODES) break;

        TreeNode* node = new TreeNode{nodeValue};
        q.push(node);
        childs.add(node);
    }
    is.get(); // getting the closing '|'
    if (is.peek() == '\n') is.get();
    
    return childs;
}

Tree::Tree(ifstream& is) {

    if (!is.is_open()) throw invalid_argument("invalid file state");

    //root = new TreeNode
    this->root.loadNode(is);
}

void Tree::destroy(TreeNode* node) {

    for (int i = 0; i < node->subordinates.size(); ++i) {

        destroy(node->subordinates[i]);
    }

    if (node != &this->root) {
        delete node;
    }
}

Tree::~Tree() {


    destroy(&root);
}

string Tree::toString() const {
    
    std::string res;

    std::queue<const TreeNode*> q;

    q.push(&this->root);
    
    while (!q.empty()) {

        int size = q.size();
        
        while(size > 0) {

            const TreeNode* n = q.front();
            q.pop();

            for (int i = 0; i < n->subordinates.size(); ++i) {
                q.push(n->subordinates[i]);
            }

            for (int i = 0; i < n->subordinates.size(); ++i) {
                
                res += std::to_string(n->value) + '-' + std::to_string(n->subordinates[i]->value) + '\n';
            }
           
            size--;
        }
    }

    return res;
}

TreeNode* Tree::getParentNode(int childNodeValue, TreeNode* startingNode) {

    TreeNode* ptr = startingNode->findChildNode(childNodeValue);

    if (ptr) return startingNode;

    else {

        for (int i = 0; i < startingNode->subordinates.size(); ++i) {

            TreeNode* parent = getParentNode(childNodeValue, startingNode->subordinates[i]);

            if (parent) return parent;
        }
    }
    return nullptr;
}

std::vector<const TreeNode*> Tree::getMatchingRoots(int value) {
    
    std::vector<const TreeNode*> matchingNodes;

    if (root.value == value) matchingNodes.push_back(&root);

    matchingRootsHelper(value, &root, matchingNodes);

    return matchingNodes;
}

std::vector<const TreeNode*> Tree::matchingRootsHelper(int val, const TreeNode* toCheck, std::vector<const TreeNode*>& matching) {


    const TreeNode* node = toCheck->findChildNode(val);

    if (node) matching.push_back(node);

    for (int i = 0; i < toCheck->subordinates.size(); ++i) {

        matchingRootsHelper(val, toCheck->subordinates[i], matching);
    }

    return matching;
}

bool Tree::contains(const Tree& obj) {
    auto roots = getMatchingRoots(obj.root.value);
    
    bool contains = false;
    for (int i = 0; i < roots.size() && !contains; ++i) {
        
        if (!this->contains(roots[i], &obj.root)) contains = true;
    }
    return contains;
}

bool Tree::contains(const TreeNode* container, const TreeNode* obj) const {
    
    bool foundAllChilds = true;
    Vector<const TreeNode*> childsMap;
    for (int i = 0; i < obj->subordinates.size() && foundAllChilds; ++i) {
        
        const TreeNode* child = container->findChildNode(obj->subordinates[i]->value);
        
        (child) ? childsMap.push_back(child) : foundAllChilds = false;
    }

    if (foundAllChilds) {
        
        for(int i = 0; i < obj->subordinates.size(); ++i) {
            
            if (!contains(childsMap[i], obj->subordinates[i])) return false;
        }
        return true;
    }

    return false;
}

bool Tree::remove(const Tree& obj) {
    TreeNode* parent = getParentNode(obj.root.value, &root);

    int rootToRemoveIndex = -1;// getMatchingRoot(&this->root, &obj.root);

    for (int i = 0; i < parent->subordinates.size(); ++i) {

        const TreeNode* child = parent->subordinates[i]; // implicit cast from vector proxy to Treenode
        if (child->value == obj.root.value) {
            rootToRemoveIndex = i;
        }
    }

    int sum = 0;
    getSumOfRemainingNodes(parent->subordinates[rootToRemoveIndex], &obj.root, sum);
    
    TreeNode* toRemove = parent->subordinates[rootToRemoveIndex];

    parent->subordinates[rootToRemoveIndex] = new TreeNode(sum);
    destroy(toRemove);
    return true;
    /* if (rootToRemove) {
        std::cout << "remo " << rootToRemove->value << '\n';
        int sum = getSumOfRemainingNodes(rootToRemove, &obj.root, 0);
        destroy(rootToRemove);//delete rootToRemove;

        if (sum != 0) rootToRemove = new TreeNode(sum);

        return true;
    }
 */
    return false;
}

std::vector<const TreeNode*> Tree::getAllSubtrees(const Tree& obj) {
    auto matching = getMatchingRoots(obj.root.value);
    
    std::vector<const TreeNode*> roots;

    for (int i = 0; i < matching.size(); ++i) {
        
        if (this->contains(matching[i], &obj.root)) roots.push_back(matching[i]);
    }

    return roots;
}

void Tree::getSumOfRemainingNodes(const TreeNode* container, const TreeNode* obj, int& sumOfRemaining) {

    Vector<pair<TreeNode*, TreeNode*>> foundChilds;
    for (int i = 0; i < container->subordinates.size(); ++i) {

        TreeNode* child = obj->findChildNode((/* (TreeNode*) */container->subordinates[i])->value);

        if (child == nullptr) {
            std::cout << "added sum: " << container->subordinates[i]->value << '\n';
            sumOfRemaining += (/* (TreeNode*) */container->subordinates[i])->value;   
        }
        else {
            std::cout << "found " << child->value << '\n';
            foundChilds.push_back(make_pair(container->subordinates[i], child));
        }
    }

    for (int i = 0; i < foundChilds.size(); ++i) {
        getSumOfRemainingNodes(foundChilds[i].first, foundChilds[i].second, sumOfRemaining);
    }

    //return sumOfRemaining;
}

/* TreeNode* Tree::find(int value, const TreeNode* startingNode) {
    TreeNode* ptr = startingNode->findChildNode(value);

    if (ptr) return ptr;

    else {


        for (int i = 0; i < startingNode->subordinates.size(); ++i) {

            ptr = find(value, startingNode->subordinates[i]);

            if (ptr) return ptr;
        }
    }
} */

/* const  */TreeNode* Tree::find(int value, const TreeNode* startingNode) const {
    //if (startingNode->value == value) return startingNode;

    //else {
        /* const  */TreeNode* ptr = startingNode->findChildNode(value);

        if (ptr) return ptr;

        else {


            for (int i = 0; i < startingNode->subordinates.size(); ++i) {

                ptr = find(value, startingNode->subordinates[i]);

                if (ptr) return ptr;
            }
        }
    //}

    return nullptr;
}

/* TreeNode* TreeNode::findChildNode(int value) {

    int begin = 0, end = subordinates.size() - 1;

    begin = 0, end = subordinates.size() - 1;

    while (begin <= end) {

        int mid = (end + begin) / 2;

        if (((TreeNode*)this->subordinates[mid])->value == value) {
            return subordinates[mid];
        }

        else if (((TreeNode*)this->subordinates[mid])->value > value) {
            end = mid - 1;
        }

        else {
            begin = mid + 1;
        }
    }

    return nullptr;
} */

/* const  */TreeNode* TreeNode::findChildNode(int value) const {

    int begin = 0, end = subordinates.size() - 1;

    while (begin <= end) {

        int mid = (end + begin) / 2;

        if (this->subordinates[mid]->value == value) {
            return subordinates[mid];
        }

        else if (this->subordinates[mid]->value > value) {
            end = mid - 1;
        }

        else {
            begin = mid + 1;
        }
    }

    return nullptr;
}