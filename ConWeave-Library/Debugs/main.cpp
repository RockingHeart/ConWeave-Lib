#include <vector>

import cwlib;
import std;

import <windows.h>;

constexpr int foo() {
    return 0;
}

int main() {
    dast::block_allocator<dast::plain_memory> allor;
    auto& value1 = *allor.allocate<int>(10);
    auto& value2 = *allor.allocate<int>(100);
    auto& value3 = *allor.allocate<int>(1000);
    std::cout << value1 << '\n';
    std::cout << value2 << '\n';
    std::cout << value3 << '\n';
}