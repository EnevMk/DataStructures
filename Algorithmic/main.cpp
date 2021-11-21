#include <iostream>
#include <vector>
#include <string>
#include <list>


template<typename T>
class special_list {

private:

    int size_;

    struct Node {
        T data;
        Node* prev;
        Node* next;
        
    };

    Node* first;
    Node* last;
    Node* middle;

public:

    special_list() : first(nullptr), last(nullptr), middle(nullptr), size_(0) {}

    ~special_list() {

        Node* ptr = first;

        while (ptr != nullptr) {
            Node* temp = ptr;
            ptr = ptr->next;
            delete temp;
        }
    }

    void push_back(const T& data_) {

        last = new Node{data_, last, nullptr};

        if (size_ == 0) {
            first = last;
            middle = last;
        }
        else {
            Node *ptr = last->prev;
            ptr->next = last;

            middle = (size_ % 2) ? middle->next : middle;
        }

        size_++;
    }

    void pop_back() {

        Node* ptr = last;
        last = last->prev;
        last->next = nullptr;

        delete ptr;

        size_--;
        middle = (size_ % 2) ? middle - 1 : middle;
    }

    void rearrange_milen() {
        Node* ptr = middle->prev;
        ptr->next = nullptr;

        last->next = first;
    }

    int size() const {
        return this->size_;
    }

    void print() const {
        Node *ptr = first;

        int len = size();
        for (int i = 0; i < len; ++i) {
            std::cout << ptr->data << ' ';
            ptr = ptr->next;
        }
    }

};

int main_() {
    special_list<int> d;

    d.push_back(1);
    d.push_back(2);
    d.push_back(3);
    d.push_back(4);
    d.push_back(5);
    d.pop_back();
    d.print();

    return 0;
}

int main() {
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */
    //std::string cmd;
    
    int n;
    std::cin >> n;
    
    special_list<int> sheets;
    
    for (int i = 0; i < n; ++i) {
        std::string cmd;
        std::cin >> cmd;
        
        if (cmd == "add") {
            int num;
            std::cin >> num;
                
            sheets.push_back(num);
        }
        else if (cmd == "gun") {
            sheets.pop_back();
            
        }
        
        else if (cmd == "milen") {
            
            //sheets.rearrange_milen();
            sheets.print();
        }
        
    }
    
    /* int s = sheets.size();
    std::cout << s << '\n';
    
    sheets.print(); */
    return 0;
}