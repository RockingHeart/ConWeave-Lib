import std;
import cwlib;

import <windows.h>;

int main() {
    dast::symbol_table<char> symtab;
    symtab.add("Var1");
    symtab.add("Var2");
    std::cout << symtab["Var2"] << '\n';
    return 0;
}