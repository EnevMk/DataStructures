#include <iostream>
#include <cmath>

double absDiff(double n, double mid) {

    if (mid > n) return mid - n;

    return n - mid;
}

double cubicRoot(double n) {

    double start = 0, end = n;

    double precision = 0.0001;

    while (true) {

        double mid = (start + end) / 2;

        double diff = absDiff(n, mid * mid * mid);
        
        if (diff <= precision) {
            return mid;
        }

        else if (mid*mid*mid > n) {
            end = mid;
        }

        else {
            start = mid;
        }
    }
}

int main() {

    long long pts, gifts = 0;

    std::cin >> pts;

    long long perfectCube = 0, i = 1;

    while (pts > 0) {
        
        long long newPerfectCube = i * i * i;
        
        if (newPerfectCube == pts) {
            //std::cout << pts << " - " << newPerfectCube << '\n';
            pts -= newPerfectCube; //
            gifts++;
            break;
        }

        if (newPerfectCube > pts) {
            //std::cout << pts << " - " << perfectCube << '\n';
            pts -= perfectCube;
            i = 1;
            gifts++;
            perfectCube = 1;
            continue;
        }

        perfectCube = newPerfectCube;
        i++;
    }

    std::cout << gifts;

}