#include <vector>

import cwlib;
import std;

import <windows.h>;

constexpr int foo() {
    return 0;
}


// 性能计时工具
class Timer {
private:
    std::chrono::high_resolution_clock::time_point start_time;
    std::string name;

public:
    Timer(const std::string& n) : name(n) {
        start_time = std::chrono::high_resolution_clock::now();
    }

    ~Timer() {
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
        std::cout << std::left << std::setw(40) << name
            << ": " << std::right << std::setw(10) << duration.count()
            << " μs" << std::endl;
    }
};

// 生成随机字符串
std::string generate_random_string(size_t length) {
    static const char charset[] =
        "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(0, sizeof(charset) - 2);

    std::string result;
    result.reserve(length);
    for (size_t i = 0; i < length; ++i) {
        result.push_back(charset[dis(gen)]);
    }
    return result;
}

int main() {
    std::cout << "========== 性能测试报告 ==========\n" << std::endl;

    const int TEST_COUNT = 100000;
    const int STRING_LENGTH = 12;
    const int APPEND_COUNT = 1000;

    // ========== 1. 构造性能测试 ==========
    std::cout << "【1. 构造性能测试】" << std::endl;
    {
        for (size_t _ = 0; _ < 3; ++_) {
            std::string test_str = generate_random_string(STRING_LENGTH);

            // std::string 构造
            {
                Timer t("std::string 拷贝构造 (x" + std::to_string(TEST_COUNT) + ")");
                for (int i = 0; i < TEST_COUNT; ++i) {
                    std::string s(test_str);
                    volatile auto dummy = s.data();
                }
            }

            // dast::cstring 构造
            {
                Timer t("dast::cstring 拷贝构造 (x" + std::to_string(TEST_COUNT) + ")");
                for (int i = 0; i < TEST_COUNT; ++i) {
                    dast::cstring s(test_str.c_str());
                    volatile auto dummy = s.data();
                }
            }
        }
    }
    std::cout << std::endl;

    // ========== 2. 赋值性能测试 ==========
    std::cout << "【2. 赋值性能测试】" << std::endl;
    {
        std::string src_str = generate_random_string(STRING_LENGTH);
        std::string dest_str;

        // std::string 赋值
        {
            Timer t("std::string 赋值 (x" + std::to_string(TEST_COUNT) + ")");
            for (int i = 0; i < TEST_COUNT; ++i) {
                dest_str = src_str;
                volatile auto dummy = dest_str.size();
            }
        }

        dast::cstring src_cstr(src_str.c_str());
        dast::cstring dest_cstr;

        // dast::cstring 赋值
        {
            Timer t("dast::cstring 赋值 (x" + std::to_string(TEST_COUNT) + ")");
            for (int i = 0; i < TEST_COUNT; ++i) {
                dest_cstr = src_cstr;
                volatile auto dummy = dest_cstr.size();
            }
        }
    }
    std::cout << std::endl;

    // ========== 3. push_back 性能测试 ==========
    std::cout << "【3. push_back 性能测试】" << std::endl;
    {
        // dast::cstring push_back
        {
            Timer t("dast::cstring push_back (x" + std::to_string(TEST_COUNT) + ")");
            dast::cstring s;
            for (int i = 0; i < TEST_COUNT; ++i) {
                s.append('A');
            }
        }
        // std::string push_back
        {
            Timer t("std::string push_back (x" + std::to_string(TEST_COUNT) + ")");
            std::string s;
            for (int i = 0; i < TEST_COUNT; ++i) {
                s.push_back('A');
            }
        }
    }
    std::cout << std::endl;

    // ========== 4. append 性能测试 ==========
    std::cout << "【4. append 性能测试】" << std::endl;
    {
        std::string append_str = generate_random_string(10);

        // std::string append
        {
            Timer t("std::string append (x" + std::to_string(APPEND_COUNT) + ")");
            std::string s;
            for (int i = 0; i < APPEND_COUNT; ++i) {
                s.append(append_str);
            }
        }

        // dast::cstring append
        {
            Timer t("dast::cstring append (x" + std::to_string(APPEND_COUNT) + ")");
            dast::cstring s;
            for (int i = 0; i < APPEND_COUNT; ++i) {
                s.append(append_str.c_str());
            }
        }
    }
    std::cout << std::endl;

    // ========== 5. 下标访问性能测试 ==========
    std::cout << "【5. 下标访问性能测试】" << std::endl;
    {
        std::string test_str = generate_random_string(STRING_LENGTH);
        dast::cstring test_cstr(test_str.c_str());

        // std::string 下标读取
        {
            Timer t("std::string operator[] 读取 (x" + std::to_string(TEST_COUNT) + ")");
            char c = 0;
            for (int i = 0; i < TEST_COUNT; ++i) {
                c = test_str[i % STRING_LENGTH];
            }
            volatile auto dummy = c;
        }

        // dast::cstring 下标读取
        {
            Timer t("dast::cstring operator[] 读取 (x" + std::to_string(TEST_COUNT) + ")");
            char c = 0;
            for (int i = 0; i < TEST_COUNT; ++i) {
                c = test_cstr[i % STRING_LENGTH];
            }
            volatile auto dummy = c;
        }

        // std::string 下标写入
        {
            Timer t("std::string operator[] 写入 (x" + std::to_string(TEST_COUNT) + ")");
            for (int i = 0; i < TEST_COUNT; ++i) {
                test_str[i % STRING_LENGTH] = 'Z';
            }
        }

        // dast::cstring 下标写入
        {
            Timer t("dast::cstring operator[] 写入 (x" + std::to_string(TEST_COUNT) + ")");
            for (int i = 0; i < TEST_COUNT; ++i) {
                test_cstr[i % STRING_LENGTH] = 'Z';
            }
        }
    }
    std::cout << std::endl;

    // ========== 6. 比较性能测试 ==========
    std::cout << "【6. 比较性能测试】" << std::endl;
    {
        std::string str1 = generate_random_string(STRING_LENGTH);
        std::string str2 = str1;
        dast::cstring cstr1(str1.c_str());
        dast::cstring cstr2(cstr1);

        // std::string 比较（相等）
        {
            Timer t("std::string operator== (相等, x" + std::to_string(TEST_COUNT) + ")");
            bool result = false;
            for (int i = 0; i < TEST_COUNT; ++i) {
                result = (str1 == str2);
            }
            volatile auto dummy = result;
        }

        // dast::cstring 比较（相等）
        {
            Timer t("dast::cstring operator== (相等, x" + std::to_string(TEST_COUNT) + ")");
            bool result = false;
            for (int i = 0; i < TEST_COUNT; ++i) {
                result = (cstr1 == cstr2);
            }
            volatile auto dummy = result;
        }
    }
    std::cout << std::endl;

    // ========== 7. 混合操作性能测试 ==========
    std::cout << "【7. 混合操作性能测试】" << std::endl;
    {
        // std::string 混合操作
        {
            Timer t("std::string 混合操作 (构造+追加+访问)");
            std::string s;
            for (int i = 0; i < 1000; ++i) {
                std::string temp = generate_random_string(10);
                s.append(temp);
                if (i % 2 == 0) {
                    s.push_back('-');
                }
                volatile auto c = s[i % s.size()];
            }
        }

        // dast::cstring 混合操作
        {
            Timer t("dast::cstring 混合操作 (构造+追加+访问)");
            dast::cstring s;
            for (int i = 0; i < 1000; ++i) {
                std::string temp = generate_random_string(10);
                s.append(temp.c_str());
                if (i % 2 == 0) {
                    s.append('-');
                }
                volatile auto c = s[i % s.size()];
            }
        }
    }
    std::cout << std::endl;

    // ========== 9. 大规模数据测试 ==========
    std::cout << "【9. 大规模数据测试】" << std::endl;
    {
        const int BIG_TEST = 1000000;

        // std::string
        {
            Timer t("std::string 构建 " + std::to_string(BIG_TEST) + " 字符");
            std::string s;
            for (int i = 0; i < BIG_TEST; ++i) {
                s.push_back('A' + (i % 26));
            }
        }

        // dast::cstring
        {
            Timer t("dast::cstring 构建 " + std::to_string(BIG_TEST) + " 字符");
            dast::cstring s;
            for (int i = 0; i < BIG_TEST; ++i) {
                s.append('A' + (i % 26));
            }
        }
    }
    std::cout << std::endl;

    std::cout << "========== 测试完成 ==========" << std::endl;

    return 0;
}