#include <iostream>
#include <string>
#include <vector>

using namespace std;

std::string repeat(const std::string& s) {

    string res;

    for (int i = 0; i < s.size(); ++i) {

        res.push_back(s[i]);
        res.push_back(s[i]);
    }

    return res;
}

int find(int elem, const vector<pair<int, int>>& v) {

    for (int i = 0; i < v.size(); ++i) {

        if (v[i].first == elem) return i;
    }
    return -1;
}

std::vector<std::pair<int, int>> mult(const std::vector<int>& v) {

    vector<pair<int, int>> res;

    for (int i = 0; i < v.size(); ++i) {
        
        if (find(v[i], res) != -1) {
            
            res[i].second++;
        } else {

            res.push_back(make_pair(v[i], 1));
        }
    }

    return res;
}

// [5, 5, 1, 1, 5, 5, 3]
int main() {

    //std::cout << repeat("String");

    auto vec = mult(vector<int>({5, 5, 1, 1, 5, 5, 3}));

    for (int i = 0; i < vec.size(); ++i) {
        std::cout << vec[i].first << " : " << vec[i].second << '\n';
    }

    return 0;
}