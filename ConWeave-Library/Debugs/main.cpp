#include <vector>

import cwlib;
import std;

import <windows.h>;

constexpr int foo() {
    return 0;
}

int main() {
    sys::adapt_filer<char> loader("A:\\Tests\\Test.txt");
    loader.read();
    std::cout << loader.data() << '\n';
}