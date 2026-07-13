#include <vector>

import cwlib;
import std;

import <windows.h>;

int main() {
    {
        dast::vector<int> vec;
        for (std::size_t i = 0; i < 10000; i++) {
            vec.push_back(i);
        }
    }
    {
        dast::vector<int, 5> vec;
        for (std::size_t i = 0; i < 10000; i++) {
            vec.push_back(i);
        }
    }
    {
        dast::vector<std::string> vec;
        for (std::size_t i = 0; i < 10000; i++) {
            vec.push_back("Hello");
        }
    }
    {
        dast::vector<std::string, 5> vec;
        for (std::size_t i = 0; i < 10000; i++) {
            vec.push_back("Hello");
        }
    }
    return 0;
}