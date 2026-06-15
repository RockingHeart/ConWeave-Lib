#include <vector>

import cwlib;
import std;

import <windows.h>;

constexpr int foo() {
    return 0;
}

int main() {
    sys::adapt_loader<char> loader("A:\\Tests\\Test.txt");
    sys::adapt_reader<char> reader(loader);
    std::cout << reader.data() << '\n';
}