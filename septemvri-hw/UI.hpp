#ifndef UI_HPP
#define UI_HPP

#include "tree.hpp"
//#include "Hashmap.hpp"
#include "CommandLine.hpp"

#include <unordered_map>

class UI {

private:
    std::unordered_map<std::string, Tree> container;

public:

    void loadTree(const std::string &name, const std::string &path);
    void saveTree(const std::string &name, const std::string &path);
    void contains(const std::string &tree1, const std::string &tree2) const;
    void remove(const std::string &tree1, const std::string &tree2);

    void run();

};

#endif