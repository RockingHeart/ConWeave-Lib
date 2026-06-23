#include <vector>

import cwlib;
import std;

import <windows.h>;

constexpr int foo() {
    return 0;
}

int main() {
    dast::block_allocator<dast::plain_memory> allor;
    auto addr1 = allor.allocate("Hello World", 11);
    auto addr2 = allor.allocate(L"Hello World2", 12);
    std::cout << addr1 << '\n';
    std::wcout << addr2 << '\n';
}