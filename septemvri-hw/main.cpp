#include <iostream>
#include "tree.hpp"
#include "sortedVector.hpp"


int main() {

    std::ifstream is;
    is.open("C:\\Users\\ildiavolo\\Desktop\\dev\\SD\\septemvri-hw\\tree5.txt");

    Tree t1(is);

    is.close();

    is.open("C:\\Users\\ildiavolo\\Desktop\\dev\\SD\\septemvri-hw\\tree6.txt");

    Tree t2(is);
    is.close();
    //t1.contains(t2);
    t1.remove(t2);

    //std::cout << t1.contains(t2);
    std::cout << t1.toString(); 


    /* SortedVector<int> s;
    s.add(14);
    s.add(2);
    s.add(7);
    s.add(3);

    s[2] = 99;

    for (int i = 0; i < s.size(); ++i) {
        std::cout << s[i] << ' ';
    } */


    return 0;
}