import std;
import fixed_vector;
import string;
import utility;


import <windows.h>;

int main() {
    auto sstr = new char[] {"Hello"};
    auto stime = GetTickCount64();
    size_t max = 0;
    {
        for (size_t i = 0; i < 100000; i++) {
            std::string str = sstr;
            str.resize(100000);
            for (size_t j = 0; j < 100000; j++) {
                str.append(" World");
            }
            max = (str.capacity());
        }
    }

    auto etime = GetTickCount64();

    std::cout << "Std: " << (etime - stime) << '\n';

    std::cout << max << '\n';
    max = 0;

    stime = GetTickCount64();

    {
        for (size_t i = 0; i < 100000; i++) {
            dast::cstring str = sstr;
            str.resize(100000);
            for (size_t j = 0; j < 100000; j++) {
                str += " World";
            }
            max = str.max_size();
        }
    }

    etime = GetTickCount64();

    std::cout << "Mine: " << (etime - stime) << '\n';
    std::cout << max << '\n';
    return 0;
}