#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include <vector>

#include "avl.hpp"

using namespace std;
/* void test_iter(const avl_tree<int, std::string>& t) {
    avl_tree<int, std::string>::const_iterator itup = t.upper_bound(17);
    auto itdown = t.lower_bound(3);

    for (auto iter = itdown; iter != itup; ++iter) {
        std::cout << iter->second << ':' << iter->first << '\n';
        
    }
} */

//template <typename Key, typename Value>
void save_tree_binary(const std::string& path, const avl_tree<int, std::string>& tr) {

    std::ofstream saveFileKeys(path + "/keys", ios::binary);

    int size = tr.unique_keys();
    saveFileKeys.write(reinterpret_cast<const char*>(&size), sizeof(int));

    for (auto it = tr.cbegin(); it != tr.cend(); ++it) {

        saveFileKeys.write(reinterpret_cast<const char*>(&it->first), sizeof(int));

        std::ofstream saveFileVals(path + '/' + std::to_string(it->first), ios::binary);
        
        int countElems = tr.count(it);
        saveFileVals.write(reinterpret_cast<const char*>(&countElems), sizeof(int));

        int len = it->second.size();
        saveFileVals.write(reinterpret_cast<const char*>(&len), sizeof(int));
        saveFileVals.write(reinterpret_cast<const char*>(it->second.c_str()), len);

        auto nextIterator = it;
        ++nextIterator;

        while(nextIterator != tr.cend() && it->first == nextIterator->first) {
            ++it;
            int len = it->second.size();
            saveFileVals.write(reinterpret_cast<const char*>(&len), sizeof(int));
            saveFileVals.write(reinterpret_cast<const char*>(it->second.c_str()), len);
            ++nextIterator;
        }

        saveFileVals.close();
    }

    saveFileKeys.close();
}

std::vector<int> load_sorted_arr(const string& path) {
    ifstream is(path + "/keys", ios::binary);

    int count;
    is.read(reinterpret_cast<char*>(&count), sizeof(int));

    vector<int> arr;
    for (int i = 0; i < count; ++i) {
        int num;
        is.read(reinterpret_cast<char*>(&num), sizeof(int));

        arr.push_back(num);
    }

    for (int i = 0 ; i < arr.size(); ++i) {
        std::cout << arr[i] << ' ';
    }
    std::cout << '\n';

    is.close();
    return arr;
}

avl_tree<int, std::string>::node* load_tree_helper(const vector<int>& v /* std::ifstream& is */, int begin, int end, const std::string& path) {

    if (begin > end) return nullptr;

    int mid = (begin + end) / 2;

    ifstream is(path + '/' + std::to_string(v[mid]));

    int countElems;
    is.read(reinterpret_cast<char*>(&countElems), sizeof(int));

    int size;
    is.read(reinterpret_cast<char*>(&size), sizeof(int));

    char* buf = new char[size+1];
    buf[size] = '\0';
    is.read(buf, size);

    avl_tree<int, std::string>::node* root = new avl_tree<int, std::string>::node;
    root->container = list<pair<const int, string>>(1, make_pair(v[mid], string(buf)));

    for (int i = 1; i < countElems; ++i) {
        int size;
        is.read(reinterpret_cast<char*>(&size), sizeof(int));

        char* buf = new char[size+1];
        buf[size] = '\0';
        is.read(buf, size);

        root->container.push_front(make_pair(v[mid], string(buf)));
        delete[] buf;
    }
    

    delete[] buf;
    is.close();

    root->left = load_tree_helper(v, begin, mid - 1, path);
    if (root->left) root->left->parent = root;

    root->right = load_tree_helper(v, mid + 1, end, path);
    if (root->right) root->right->parent = root;

    return root;
    
}

void load_tree_binary(const std::string& path, avl_tree<int, std::string>& tr) {
    
    auto keys = load_sorted_arr(path);
    
    tr.root = load_tree_helper(keys, 0, keys.size() - 1, path);

    tr.root->parent = nullptr;
}

int main() {


    avl_tree<int, std::string> tr;
    
    tr.insert(3, "Roma");
    tr.insert(18, "Inter");
    tr.insert(19, "Milan");
    tr.insert(36, "Juve");
    tr.insert(9, "Genoa");
    tr.insert(2, "Fiorentina");
    tr.insert(2, "Napoli");
    tr.insert(7, "Bologna");
    //tr.insert(28, "REal");



    /* avl_tree<int, std::string>::iterator it = tr.lower_bound(3);
    auto itup = tr.upper_bound(20);

    for (auto iter = itup; iter != it; iter--) {
        std::cout << iter->second << " : " << iter->first << '\n';
    } */

    //tr.erase(2);
    
    string path = "C:/Users/ildiavolo/Desktop/dev/SD/avl/data";
    save_tree_binary(path, tr);


    avl_tree<int, string> toLoad;

    load_tree_binary(path, toLoad);

    avl_tree<int, std::string>::iterator it = toLoad.lower_bound(3);
    auto itup = toLoad.upper_bound(20);

    for (auto iter = toLoad.begin(); iter != toLoad.end(); iter++) {
        std::cout << iter->second << " : " << iter->first << '\n';
    }

    //std::cout << '\n' << tr.height() << '\n' << tr.size() << '\n';

    /* for (auto it = toLoad.cbegin(); it != toLoad.cend(); ++it) {
        std::cout << it->second << '\n';
    } */

    /*
    auto itdown = tr.lower_bound(7), itup = tr.upper_bound(18);

    for (auto it = itdown; it != itup; ++it) {
        std::cout << it->second << " : " << it->first << '\n';
    }
        
    auto range = tr.equal_range(2);

    for (auto it = range.first; it != range.second; ++it) {
        std::cout << it->second << ' ';
    } */


    return 0;
}