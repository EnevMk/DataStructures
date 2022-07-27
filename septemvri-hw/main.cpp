#include <iostream>
#include "tree.hpp"
#include "sortedVector.hpp"


int main() {

    std::ifstream is;
    is.open("C:\\Users\\ildiavolo\\Desktop\\dev\\SD\\septemvri-hw\\tree3.txt");

    Tree t1(is);

    is.close();

    is.open("C:\\Users\\ildiavolo\\Desktop\\dev\\SD\\septemvri-hw\\tree4.txt");

    Tree t2(is);

    //t1.contains(t2);

    std::cout << t1.contains(t2);

    /* is.open("C:\\Users\\ildiavolo\\Desktop\\dev\\SD\\septemvri-hw\\tree2.txt");

    Tree sub(is);
    is.close();

    std::cout << "\n\n" << sub.toString() << "\n\n" << t.contains(sub) << "\n\n";

    t.remove(sub);

    std::cout << t.toString(); */

    /* SortedVector<int> s;
    s.add(14);
    s.add(2);
    s.add(7);
    s.add(3);

    for (int i = 0; i < s.size(); ++i) {
        std::cout << s[i] << ' ';
    }

    s[1] = 99; */


    return 0;
}