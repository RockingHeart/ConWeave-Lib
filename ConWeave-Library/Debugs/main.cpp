#include <vector>

import cwlib;
import std;

import <windows.h>;

int main() {
    const dast::vector<int> vec(1);
    vec.begin()[0] = 0;
    return 0;
}