import std;
import cwlib;

import <windows.h>;

int main() {
    dast::symbol_table<char> symtab;
    symtab.add("Var1");
    symtab.add("Var2");
    symtab.for_each (
        [&](auto symbol) {
            std::cout << symbol << '\n';
        }
    );
    return 0;
}