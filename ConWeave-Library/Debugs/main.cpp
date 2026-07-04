#include <vector>

import cwlib;
import std;

import <windows.h>;

constexpr int foo() {
    return 0;
}

int main() {
    dast::cstring str = "Hello.............";
    str += ".............World.............";
    str.resize(60, ' ');
    std::cout << str.const_string() << '\n';
}