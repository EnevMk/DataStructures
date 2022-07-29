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
    this->subordinates = loadChildren(is, elems, this);

    while (!elems.empty()) {

        int size = elems.size();
        
        while (size > 0) {

            TreeNode* node = elems.front();
            elems.pop();
            
            node->subordinates = loadChildren(is, elems, node);
            
            size--;
        }
    }
}

SortedVector<TreeNode*, ptr_less<const TreeNode*>> TreeNode::loadChildren(std::ifstream& is, std::queue<TreeNode*> &q, TreeNode* parent) {

    SortedVector<TreeNode*, ptr_less<const TreeNode*>> childs;
    string strValue;

    if (is.peek() == '|') is.get();
    
    while (is.peek() != '|' && is.peek() != '\n') {
        int nodeValue = getNextNode(is);
        if (nodeValue == NO_CHILDREN_NODES) break;

        TreeNode* node = new TreeNode{nodeValue};
        q.push(node);
        childs.add(node);
        node->parent = parent;
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

TreeNode* TreeNode::getParentNode() {

    return this->parent;
}

std::vector<TreeNode*> Tree::getMatchingRoots(int value) {
    
    std::vector<TreeNode*> matchingNodes;

    if (root.value == value) matchingNodes.push_back(&root);

    matchingRootsHelper(value, &root, matchingNodes);

    return matchingNodes;
}

std::vector<TreeNode*> Tree::matchingRootsHelper(int val, const TreeNode* toCheck, std::vector<TreeNode*>& matching) {


    TreeNode* node = toCheck->findChildNode(val);

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

bool Tree::contains(const TreeNode* container, const TreeNode* obj) {
    
    bool foundAllChilds = true;
    Vector<const TreeNode*> childsMap;
    for (int i = 0; i < obj->subordinates.size() && foundAllChilds; ++i) {
        
        const TreeNode* child = container->findChildNode(obj->subordinates[i]->value);
        
        if (child) childsMap.push_back(child);
        else { foundAllChilds = false; }
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

    TreeNode* sub = nullptr;
    int lev = 0;
    getDeepestSubtree(&root, &obj.root, sub, lev, 0);

    if (sub) {
        
        TreeNode* parent = sub->getParentNode();
        auto sum = getSumOfRemainingNodes(sub, &obj.root);
        int j = parent->findChildNodeIndex(sub->value);

        if (sum && !parent->findChildNode(sum.value())) {
            parent->subordinates[j] = new TreeNode{sum.value()};
        } else {
            parent->subordinates.remove(j);
        }
        destroy(sub);
        remove(obj);
    }

    return true;
}

void Tree::getDeepestSubtree(TreeNode* container, const TreeNode *obj, TreeNode*& current, int &level, int currentLevel) {

    
    if (container->value == obj->value && currentLevel > level && contains(container, obj)) {
        current = container;
        level = currentLevel;
    }

    for (int i = 0; i < container->subordinates.size(); ++i) {

        getDeepestSubtree(container->subordinates[i], obj, current, level, currentLevel + 1);
    }
}

std::vector<TreeNode*> Tree::getAllSubtrees(const Tree& obj) {
    auto potentialRoots = getMatchingRoots(obj.root.value);
    
    std::vector<TreeNode*> roots;

    copy_if(potentialRoots.cbegin(), potentialRoots.cend(), std::back_inserter(roots), 
    [obj](TreeNode* t) { return contains(t, &obj.root); });

    return roots;
}

std::optional<int> Tree::getSumOfRemainingNodes(const TreeNode* container, const TreeNode* obj) {
    optional<int> sum = 0;
    bool remaining = false;

    getSumOfRemainingNodes(container, obj, sum, remaining);

    return (remaining) ? sum : std::nullopt;
}

void Tree::getSumOfRemainingNodes(const TreeNode* container, const TreeNode* obj, std::optional<int>& sumOfRemaining, bool &flag) {

    Vector<pair<TreeNode*, TreeNode*>> foundChilds;
    for (int i = 0; i < container->subordinates.size(); ++i) {

        TreeNode* child = obj->findChildNode(container->subordinates[i]->value);

        if (child == nullptr) {
            flag = true;
            *sumOfRemaining += container->subordinates[i]->value;   
        }
        else {
            foundChilds.push_back(make_pair(container->subordinates[i], child));
        }
    }

    for (int i = 0; i < foundChilds.size(); ++i) {
        getSumOfRemainingNodes(foundChilds[i].first, foundChilds[i].second, sumOfRemaining, flag);
    }
}

int TreeNode::findChildNodeIndex(int value) const {

    int begin = 0, end = subordinates.size() - 1;

    while (begin <= end) {

        int mid = (end + begin) / 2;

        if (this->subordinates[mid]->value == value) return mid;

        else if (this->subordinates[mid]->value > value) end = mid - 1;

        else { begin = mid + 1; }
    }
    return -1;
}

TreeNode* TreeNode::findChildNode(int value) const {

    int i = findChildNodeIndex(value);

    return (i != -1) ? subordinates[i] : nullptr;
}