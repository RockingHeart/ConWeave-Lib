#include <vector>

import cwlib;
import std;

import <windows.h>;

constexpr int foo() {
    return 0;
}

int main() {
    sys::adapt_filer<char> filer("A:\\Tests\\Test.txt");
    filer.read();
    std::cout << sys::adapt_loader<char>::can_open(nullptr) << '\n';
    std::cout << filer.data() << '\n';
}