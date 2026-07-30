import std;
import cwlib;

import <windows.h>;

int main() {
    dast::symbol_table<char, int, 1> symtab;
    symtab.add("Var1", 1);
    symtab.add("Var2", 2);
    if (auto result = symtab["Var2"]; result) {
        std::cout << result.value() << '\n';
    }
    dast::cstring str = "Hello";
    std::cout << (str + 'k').const_string();
    return 0;
}