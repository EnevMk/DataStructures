#include "UI.hpp"

void UI::run() {

    std::string str;
    
    std::getline(std::cin, str);

    CommandLine cmd;
    try {
        cmd.parseCommand(str);
    }
    catch (length_error &obj) {
        std::cout << obj.what() << '\n';
        run();
    }

    while (cmd[0] != "EXIT") {
        try {
            
            if (cmd[0] == "LOAD") {
                this->loadTree(cmd[1], cmd[2]);
            }
            else if (cmd[0] == "SAVE") {
                this->saveTree(cmd[1], cmd[2]);
            }
            else if (cmd[0] == "CONTAINS") {
                this->contains(cmd[1], cmd[2]);
            }
            else if (cmd[0] == "REMOVE") {
                this->remove(cmd[1], cmd[2]);
            }
            else if (cmd[0] == "EXIT") {
                break;
            }
            else {
                std::cout << cmd[0] << " is not recognized as a command.\n";
            }

            std::getline(std::cin, str);
        
            cmd.parseCommand(str);
        }
        catch (exception& obj) {
            std::cout << obj.what() << '\n';
        }
        
    }
}

void UI::loadTree(const std::string &name, const std::string &path) {

    std::ifstream is(path);
    if (!is) throw std::invalid_argument(path + " is not a valid path to a file with a tree");

    is.clear();
    is.ignore();

    Tree t;
    try {
        t.loadFromStream(is);
    }
    catch (std::exception& ex) {
        std::cout << ex.what() << "\n Loading tree " + name + " failed.\n";
        is.close();
        return;
    }
    is.close();

    if (container.find(name) != container.end()) container.erase(name);
    container.insert(make_pair(name, move(t)));
    
    std::cout << "tree " + name + " loaded successfully!\n";
}

void UI::saveTree(const std::string &name, const std::string &path) {
    std::ofstream os(path);

    os.clear();

    if (!os) throw invalid_argument("file at " + path + " does not exist.\n");

    auto iter = this->container.find(name);

    if (iter == container.cend()) throw invalid_argument("tree named " + name + " is not loaded in the current session.\n");

    iter->second.saveToStream(os);

    os.close();

    std::cout << "Tree " + name + " saved successfully at " + path + "!\n";
}

void UI::contains(const std::string &t1, const std::string &t2) const {
    auto iter1 = container.find(t1), iter2 = container.find(t2);

    if (iter1 == container.cend()) throw invalid_argument("tree named " + t1 + " is not loaded in the current session.\n");
    if (iter2 == container.cend()) throw invalid_argument("tree named " + t2 + " is not loaded in the current session.\n");
    std::cout << "heyaa\n";
    if (iter1->second.contains(iter2->second)) {
        std::cout << t2 << " is a subtree of " << t2 << '\n';
    }
    else {
        std::cout << t2 << " is NOT a subtree of " << t2 << '\n';
    }
}

void UI::remove(const std::string &t1, const std::string &t2) {
    auto iter1 = container.find(t1), iter2 = container.find(t2);

    if (iter1 == container.end()) throw invalid_argument("tree named " + t1 + " is not loaded in the current session.\n");
    if (iter2 == container.end()) throw invalid_argument("tree named " + t2 + " is not loaded in the current session.\n");

    iter1->second.remove(iter2->second);
    

    std::cout << "Removed all occurrences of " + t2 + " in " + t1 + ".\n";
}