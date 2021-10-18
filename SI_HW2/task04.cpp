#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

struct BodyBuilderStrength {
    double diameter;
    double time;
    int index;

    double totalMass;
};

bool compareStrength(BodyBuilderStrength a, BodyBuilderStrength b) {

    /* return (a.diameter / a.time < b.diameter / b.time) || ((a.diameter / a.time == b.diameter / b.time)
            && a.diameter < b.diameter); */

    return a.totalMass < b.totalMass || (a.totalMass == b.totalMass && a.diameter < b.diameter);
}

int main() {

    int ahtletesCount;
    std::cin >> ahtletesCount;

    std::vector<BodyBuilderStrength> myVec;
    myVec.reserve(ahtletesCount);

    int timeUnit = 1;

    for (int i = 0; i < ahtletesCount; ++i) {

        BodyBuilderStrength b;
        std::cin >> b.diameter >> b.time;

        timeUnit *= b.time;

        b.index = i + 1;
        myVec[i] = b;
    }

    for (int i = 0; i < ahtletesCount; ++i) {
        double radius = myVec[i].diameter / 2; 
        double area = radius * radius;
        myVec[i].totalMass = area / myVec[i].time;
    }

    std::sort(myVec.begin(), myVec.begin() + ahtletesCount, compareStrength);
        
    for (int i = ahtletesCount - 1; i >= 0; --i) {

        std::cout << myVec[i].index << ' ';
    }
    
    return 0;
}