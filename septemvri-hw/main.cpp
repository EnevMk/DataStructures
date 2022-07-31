#include <iostream>
#include "tree.hpp"
#include "sortedVector.hpp"
#include "UI.hpp"



int main() {

    UI session;
    session.run();

    /* std::ifstream is;
    is.open("C:\\Users\\ildiavolo\\Desktop\\dev\\SD\\septemvri-hw\\tree5.txt");

    Tree t1;
    t1.loadFromStream(is);

    is.close();

    is.open("C:\\Users\\ildiavolo\\Desktop\\dev\\SD\\septemvri-hw\\tree4.txt");

    Tree t2(is);
    is.close();

    Tree t3(t1);

    t3.remove(t2);
    
    std::ofstream os("C:\\Users\\ildiavolo\\Desktop\\dev\\SD\\septemvri-hw\\milan.txt");
    t3.saveToStream(os);
    std::cout << t1.contains(t2);
    os.close(); */

    
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