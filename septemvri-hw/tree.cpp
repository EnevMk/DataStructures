#include "tree.hpp"

TreeNode::TreeNode() {}

TreeNode::TreeNode(int val) : value(val) {}

TreeNode::TreeNode(int val, TreeNode* const parent) : value(val), parent(parent) {}

int getNextNode(std::ifstream& is) {

    string strValue;

    int whitespaces = 0;
    while (is.peek() == ' ') {
        if (whitespaces > WHITESPACES_EXCESS) throw invalid_argument("tree in file is too messed up!");
        is.get();
    }

    while (is.peek() != ' ' && is.peek() != '|') {
        strValue += is.get();
    }
    
    if (is.peek() == ' ') is.get();
    
    if (strValue.empty()) return NO_CHILDREN_NODES;
    
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

    if (is.peek() == '|') is.get();
    
    while (is.peek() != '|' && is.peek() != '\n') {
        int nodeValue;
        try {
            nodeValue = getNextNode(is);
        }
        catch (...) {
            for (int i = 0; i < childs.size();++i) {
                delete childs[i];
            }
            throw;
        }
        
        if (nodeValue == NO_CHILDREN_NODES) break;

        TreeNode* node = new TreeNode{nodeValue, parent};
        q.push(node);
        childs.add(node);
        //node->parent = parent;
    }
    is.get(); // getting the closing '|'
    if (is.peek() == '\n') is.get();
    
    return childs;
}

void Tree::saveToStream(std::ofstream& os) const {

    if (!os.is_open()) throw invalid_argument("file state not valid");

    std::queue<const TreeNode*> q;

    os << "| " << root.value << " |\n";
    q.push(&root);

    int brotherhoods = 1;
    while (!q.empty()) {
        
        const TreeNode* node = q.front();
        q.pop();

        int size = node->subordinates.size(), i = 0;
        os << '|' << ' ';
    
        while (i < size) {
            os << node->subordinates[i]->value << ' ';

            q.push(node->subordinates[i]);
            i++;
        }
        brotherhoods--;
        if(brotherhoods == 0)  {
            brotherhoods = q.size();
            os << "|\n";
        }    
    }
}

Tree::Tree() {}

Tree::Tree(ifstream& is) {

    loadFromStream(is);
}

Tree::Tree(const Tree& obj) {
    std::cout << "copy cons\n";
    this->root.value = obj.root.value;

    for (int i = 0; i < obj.root.subordinates.size(); ++i) {
        this->root.subordinates.add(obj.root.subordinates[i]->clone(&root));
    }
}

TreeNode* TreeNode::clone(TreeNode* parent) const {

    TreeNode *toReturn = new TreeNode{this->value};

    toReturn->parent = parent;

    for (int i = 0; i < this->subordinates.size(); ++i) {
        toReturn->subordinates.add(this->subordinates[i]->clone(toReturn));
    }

    return toReturn;
}

Tree::Tree(Tree&& obj) {
    this->root.value = obj.root.value;

    for (int i = 0; i < obj.root.subordinates.size(); ++i) {
        ((TreeNode*)obj.root.subordinates[i])->parent = &root;

        root.subordinates.add(obj.root.subordinates[i]);
    }

    for (int i = obj.root.subordinates.size() - 1; i >= 0; --i) {
        obj.root.subordinates.remove(i);
    }


}

void Tree::loadFromStream(std::ifstream& is) {

    if (!is.is_open()) throw invalid_argument("invalid file state");

    try {
        this->root.loadNode(is);
    } catch (exception& obj) {
        destroy(&root);
        throw;
    }
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

std::vector<const TreeNode*> Tree::getMatchingRoots(int value) const {
    
    std::vector<const TreeNode*> matchingNodes;

    if (root.value == value) matchingNodes.push_back(&root);

    matchingRootsHelper(value, &root, matchingNodes);

    return matchingNodes;
}

std::vector<const TreeNode*> Tree::matchingRootsHelper(int val, const TreeNode* toCheck, std::vector<const TreeNode*>& matching) const {


    TreeNode* node = toCheck->findChildNode(val);

    if (node) matching.push_back(node);

    for (int i = 0; i < toCheck->subordinates.size(); ++i) {

        matchingRootsHelper(val, toCheck->subordinates[i], matching);
    }

    return matching;
}

bool Tree::contains(const Tree& obj) const {
    auto roots = getMatchingRoots(obj.root.value);
    
    bool contains = false;
    for (int i = 0; i < roots.size() && !contains; ++i) {
        
        if (this->contains(roots[i], &obj.root)) contains = true;
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
    }

    return foundAllChilds;
}

bool Tree::remove(const Tree& obj) {

    TreeNode* sub = nullptr;
    int lev = 0;

    getDeepestSubtree(&root, &obj.root, sub, lev, 0);

    if (sub) {
        TreeNode* parent = sub->getParentNode();
        
        auto sum = getSumOfRemainingNodes(sub, &obj.root);
        int j = parent->findChildNodeIndex(sub->value);
        
        if (sum) {
            const TreeNode* childAlreadyExists = parent->findChildNode(sum.value());
            
            if (!childAlreadyExists || childAlreadyExists == sub) parent->subordinates[j] = new TreeNode{sum.value(), parent};
            else { parent->subordinates.remove(j); }
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

std::vector<const TreeNode*> Tree::getAllSubtrees(const Tree& obj) {
    auto potentialRoots = getMatchingRoots(obj.root.value);
    
    std::vector<const TreeNode*> roots;

    copy_if(potentialRoots.cbegin(), potentialRoots.cend(), std::back_inserter(roots), 
    [&obj](const TreeNode* t) { return contains(t, &obj.root); });

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