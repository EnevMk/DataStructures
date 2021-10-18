#include <iostream>
#include <cstring>

int abs(int n) {
    if (n < 0) return -n;
    return n;
}

int main() {

    char str[64];
    char ch;

    std::cin >> str;
    std::cin >> ch;

    int lastIndex, strLen = strlen(str);
    int distances[strLen];

    for (int i = 0; i < strLen; ++i) {

        if (str[i] == ch) {
            lastIndex = i;
            break;
        }

    }

    for (int i = 0; i < strLen; ++i) {

        if (str[i] == ch) {
            lastIndex = i;
            distances[i] = 0;
            continue;
        }

        distances[i] = abs(i - lastIndex);
    }

    for (int i = strLen - 1; i >= 0; --i) {
        if (lastIndex <= i) continue;

        if (str[i] == ch) {
            lastIndex = i;
            //distances[i] = 0;
            continue;
        }

        distances[i] = (distances[i] > lastIndex - i) ? lastIndex - i : distances[i];
    }

    for (int i = 0; i < strLen; ++i) {

        std::cout << distances[i] << ' ';
    }
}