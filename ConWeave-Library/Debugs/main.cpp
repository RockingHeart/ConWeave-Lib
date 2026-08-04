#include <vector>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <random>
#include <string>
#include <cassert>

import cwlib;
import std;
import <windows.h>;

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
        std::cout << std::left << std::setw(55) << name
            << ": " << std::right << std::setw(10) << duration.count()
            << " μs" << std::endl;
    }
};

// 生成随机字符串
std::string generate_random_string(size_t length) {
    static const char charset[] =
        "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    static std::mt19937 gen(std::random_device{}());
    static std::uniform_int_distribution<> dis(0, sizeof(charset) - 2);

    std::string result;
    result.reserve(length);
    for (size_t i = 0; i < length; ++i) {
        result.push_back(charset[dis(gen)]);
    }
    return result;
}

int main() {
    std::cout << "dast::cstring 正确性测试..." << std::endl;

    // 1. 默认构造
    {
        dast::cstring s;
        assert(s.size() == 0);
        assert(s.empty());
    }

    // 2. C字符串构造 + operator[]
    {
        dast::cstring s("hello");
        assert(s.size() == 5);
        assert(s[0] == 'h');
        assert(s[4] == 'o');
    }

    // 3. 拷贝构造
    {
        dast::cstring s1("test");
        dast::cstring s2(s1);
        assert(s2.size() == 4);
        assert(strcmp(s2.data(), "test") == 0);
    }

    // 4. 拷贝赋值
    {
        dast::cstring s1("hello");
        dast::cstring s2;
        s2 = s1;
        assert(s2.size() == 5);
        assert(strcmp(s2.data(), "hello") == 0);
    }

    // 5. 自赋值
    {
        dast::cstring s("self");
        s = s;
        assert(s.size() == 4);
        assert(strcmp(s.data(), "self") == 0);
    }

    // 6. 写入
    {
        dast::cstring s("abc");
        s[0] = 'X';
        assert(s[0] == 'X');
        assert(strcmp(s.data(), "Xbc") == 0);
    }

    // 7. 拷贝独立性
    {
        dast::cstring s1("original");
        dast::cstring s2(s1);
        s2[0] = 'O';
        assert(s1[0] == 'o');
        assert(s2[0] == 'O');
    }

    // 8. 赋值独立性
    {
        dast::cstring s1("first");
        dast::cstring s2("second");
        s1 = s2;
        s1[0] = 'S';
        assert(s2[0] == 's');
        assert(s1[0] == 'S');
    }

    // 9. SSO 边界
    {
        dast::cstring s1("12345678901234567890123");   // 23 字节（SSO 内）
        assert(s1.size() == 23);

        dast::cstring s2("123456789012345678901234");  // 24 字节（堆分配）
        assert(s2.size() == 24);

        std::string long_str(50, 'A');
        dast::cstring s3(long_str.c_str());
        assert(s3.size() == 50);
        assert(s3[0] == 'A');
        assert(s3[49] == 'A');
    }

    // 10. SSO/堆混合赋值
    {
        // 堆 -> SSO
        dast::cstring s1(std::string(50, 'B').c_str());
        s1 = dast::cstring("short");
        assert(s1.size() == 5);
        assert(strcmp(s1.data(), "short") == 0);

        // SSO -> 堆
        dast::cstring s2("short");
        s2 = dast::cstring(std::string(50, 'C').c_str());
        assert(s2.size() == 50);
        assert(s2[0] == 'C');
    }

    // 11. append 单字符
    {
        dast::cstring s;
        s.append('A');
        s.append('B');
        s.append('C');
        assert(s.size() == 3);
        assert(strcmp(s.data(), "ABC") == 0);
    }

    // 12. append C字符串
    {
        dast::cstring s("Hello");
        s.append(" World");
        assert(s.size() == 11);
        assert(strcmp(s.data(), "Hello World") == 0);
    }

    // 13. 比较
    {
        dast::cstring s1("abc");
        dast::cstring s2("abc");
        dast::cstring s3("xyz");
        assert(s1 == s2);
        assert(!(s1 == s3));
    }

    // 14. operator+=
    {
        // 14a. += 单字符
        {
            dast::cstring s("Hello");
            s += '!';
            assert(s.size() == 6);
            assert(strcmp(s.data(), "Hello!") == 0);
        }

        // 14b. += C字符串
        {
            dast::cstring s("Hello");
            s += " World";
            assert(s.size() == 11);
            assert(strcmp(s.data(), "Hello World") == 0);
        }

        // 14c. += dast::cstring
        {
            dast::cstring s1("Hello");
            dast::cstring s2(" World");
            s1 += s2;
            assert(s1.size() == 11);
            assert(strcmp(s1.data(), "Hello World") == 0);
        }

        // 14d. += 自身
        {
            dast::cstring s("abc");
            s += s;
            assert(s.size() == 6);
            assert(strcmp(s.data(), "abcabc") == 0);
        }

        // 14e. += 跨 SSO 边界
        {
            dast::cstring s("12345678901234567890123");  // 23 字节
            s += "!";
            assert(s.size() == 24);
            assert(s[23] == '!');
        }

        // 14f. += 触发堆增长
        {
            dast::cstring s("short");
            s += dast::cstring(std::string(30, 'X').c_str());
            assert(s.size() == 35);
            assert(s[0] == 's');
            assert(s[4] == 't');
            assert(s[5] == 'X');
            assert(s[34] == 'X');
        }
    }

    // 15. 移动构造
    {
        dast::cstring s1("hello world");
        dast::cstring s2(std::move(s1));
        assert(s2.size() == 11);
        assert(strcmp(s2.data(), "hello world") == 0);
        assert(s1.empty() || s1.size() == 0);
    }

    // 16. 移动赋值
    {
        // 16a. 基本移动赋值
        {
            dast::cstring s1("hello");
            dast::cstring s2;
            s2 = std::move(s1);
            assert(s2.size() == 5);
            assert(strcmp(s2.data(), "hello") == 0);
        }

        // 16b. 移动赋值到非空对象（SSO -> SSO）
        {
            dast::cstring s1("world");
            dast::cstring s2("hello");
            s2 = std::move(s1);
            assert(s2.size() == 5);
            assert(strcmp(s2.data(), "world") == 0);
        }

        // 16c. 移动赋值跨模式（堆 -> SSO）
        {
            dast::cstring s1(std::string(50, 'A').c_str());
            dast::cstring s2("short");
            s2 = std::move(s1);
            assert(s2.size() == 50);
            assert(s2[0] == 'A');
            assert(s2[49] == 'A');
        }

        // 16d. 移动赋值跨模式（SSO -> 堆）
        {
            dast::cstring s1("short");
            dast::cstring s2(std::string(50, 'B').c_str());
            s2 = std::move(s1);
            assert(s2.size() == 5);
            assert(strcmp(s2.data(), "short") == 0);
        }

        // 16e. 自移动赋值
        {
            dast::cstring s("self");
            s = std::move(s);
            assert(s.size() == 4);
            assert(strcmp(s.data(), "self") == 0);
        }
    }

    // 17. 移动后独立性
    {
        dast::cstring s1("original");
        dast::cstring s2(std::move(s1));
        s2[0] = 'O';
        assert(s2[0] == 'O');
    }

    // 18. 链式追加
    {
        dast::cstring s;
        s += 'A';
        s += 'B';
        s += 'C';
        s += "DEF";
        s += dast::cstring("GHI");
        assert(s.size() == 9);
        assert(strcmp(s.data(), "ABCDEFGHI") == 0);
    }

    // 19. 堆模式 += 自身
    {
        dast::cstring s(std::string(30, 'A').c_str());
        size_t old_size = s.size();
        s += s;
        assert(s.size() == old_size * 2);
        for (size_t i = 0; i < s.size(); ++i) {
            assert(s[i] == 'A');
        }
    }

    // 20. 多次追加验证
    {
        dast::cstring s;
        for (int i = 0; i < 100; ++i) {
            s += 'X';
        }
        assert(s.size() == 100);
        for (size_t i = 0; i < 100; ++i) {
            assert(s[i] == 'X');
        }
    }

    // 21. 混合模式切换验证
    {
        dast::cstring s("short");
        s += dast::cstring(std::string(30, 'A').c_str());  // SSO -> 堆
        assert(s.size() == 35);
        for (size_t i = 0; i < 5; ++i) assert(s[i] == "short"[i]);
        for (size_t i = 5; i < 35; ++i) assert(s[i] == 'A');

        s += dast::cstring(std::string(30, 'B').c_str());  // 堆 -> 更大堆
        assert(s.size() == 65);
        for (size_t i = 0; i < 5; ++i) assert(s[i] == "short"[i]);
        for (size_t i = 5; i < 35; ++i) assert(s[i] == 'A');
        for (size_t i = 35; i < 65; ++i) assert(s[i] == 'B');
    }

    std::cout << "全部 " << 21 << " 项测试通过!" << std::endl;
    std::cout << "========== dast::cstring vs std::string 性能测试 ==========\n" << std::endl;

    const int TEST_COUNT = 10000;
    const int STRING_LENGTH = 100;
    const int APPEND_COUNT = 1000;

    // ========== 1. 构造性能测试 ==========
    std::cout << "【1. 构造性能测试】" << std::endl;
    {
        std::string test_str = generate_random_string(STRING_LENGTH);

        // std::string 拷贝构造
        {
            Timer t("std::string 拷贝构造 (x" + std::to_string(TEST_COUNT) + ")");
            for (int i = 0; i < TEST_COUNT; ++i) {
                std::string s(test_str);
                volatile auto dummy = s.data();
            }
        }

        // dast::cstring 拷贝构造
        {
            Timer t("dast::cstring 拷贝构造 (x" + std::to_string(TEST_COUNT) + ")");
            for (int i = 0; i < TEST_COUNT; ++i) {
                dast::cstring s(test_str.c_str());
                volatile auto dummy = s.data();
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

    }
    std::cout << std::endl;

    // ========== 3. push_back / append 字符性能测试 ==========
    std::cout << "【3. 追加单字符性能测试】" << std::endl;
    {
        // std::string push_back
        {
            Timer t("std::string push_back (x" + std::to_string(TEST_COUNT) + ")");
            std::string s;
            for (int i = 0; i < TEST_COUNT; ++i) {
                s.push_back('A');
            }
            volatile auto dummy = s.size();
        }

        // dast::cstring append 单字符
        {
            Timer t("dast::cstring append(char) (x" + std::to_string(TEST_COUNT) + ")");
            dast::cstring s;
            for (int i = 0; i < TEST_COUNT; ++i) {
                s.append('A');
            }
            volatile auto dummy = s.size();
        }
    }
    std::cout << std::endl;

    // ========== 4. append 字符串性能测试 ==========
    std::cout << "【4. append 字符串性能测试】" << std::endl;
    {
        std::string append_str = generate_random_string(10);

        // std::string append
        {
            Timer t("std::string append (x" + std::to_string(APPEND_COUNT) + ")");
            std::string s;
            for (int i = 0; i < APPEND_COUNT; ++i) {
                s.append(append_str);
            }
            volatile auto dummy = s.size();
        }

        // dast::cstring append
        {
            Timer t("dast::cstring append (x" + std::to_string(APPEND_COUNT) + ")");
            dast::cstring s;
            for (int i = 0; i < APPEND_COUNT; ++i) {
                s.append(append_str.c_str());
            }
            volatile auto dummy = s.size();
        }
    }
    std::cout << std::endl;

    // ========== 5. 下标访问性能测试 ==========
    std::cout << "【5. 下标访问性能测试】" << std::endl;
    {
        std::string test_str = generate_random_string(STRING_LENGTH);
        dast::cstring test_cstr(test_str.c_str());
        volatile char c;

        // std::string 下标读取
        {
            Timer t("std::string operator[] 读取 (x" + std::to_string(TEST_COUNT) + ")");
            for (int i = 0; i < TEST_COUNT; ++i) {
                c = test_str[i % STRING_LENGTH];
            }
        }

        // dast::cstring 下标读取
        {
            Timer t("dast::cstring operator[] 读取 (x" + std::to_string(TEST_COUNT) + ")");
            for (int i = 0; i < TEST_COUNT; ++i) {
                c = test_cstr[i % STRING_LENGTH];
            }
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

        // std::string 比较（不等）
        std::string str3 = generate_random_string(STRING_LENGTH);
        {
            Timer t("std::string operator== (不等, x" + std::to_string(TEST_COUNT) + ")");
            bool result = false;
            for (int i = 0; i < TEST_COUNT; ++i) {
                result = (str1 == str3);
            }
            volatile auto dummy = result;
        }

        // dast::cstring 比较（不等）
        dast::cstring cstr3(str3.c_str());
        {
            Timer t("dast::cstring operator== (不等, x" + std::to_string(TEST_COUNT) + ")");
            bool result = false;
            for (int i = 0; i < TEST_COUNT; ++i) {
                result = (cstr1 == cstr3);
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
            s.reserve(10000);
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

    // ========== 8. 大规模数据测试 ==========
    std::cout << "【8. 大规模数据测试】" << std::endl;
    {
        const int BIG_TEST = 1000000;

        // std::string
        {
            Timer t("std::string 构建 " + std::to_string(BIG_TEST) + " 字符");
            std::string s;
            s.reserve(BIG_TEST);
            for (int i = 0; i < BIG_TEST; ++i) {
                s.push_back('A' + (i % 26));
            }
            volatile auto dummy = s.size();
        }

        // dast::cstring
        {
            Timer t("dast::cstring 构建 " + std::to_string(BIG_TEST) + " 字符");
            dast::cstring s;
            for (int i = 0; i < BIG_TEST; ++i) {
                s.append('A' + (i % 26));
            }
            volatile auto dummy = s.size();
        }
    }
    std::cout << std::endl;

    // ========== 10. SSO 优势测试 ==========
    std::cout << "【10. SSO 优势测试（dast::cstring buffer=23, std::string buffer=15/22）】" << std::endl;
    {
        const int ITER = 100000;

        // --- 10a. SSO 内：10 字节（两者都在 SSO 内）---
        std::cout << "  --- 10a. 10 字节（两者都在 SSO 内）---" << std::endl;
        {
            std::string std_s = generate_random_string(10);
            dast::cstring dst_s(std_s.c_str());

            {
                Timer t("  std::string 构造");
                for (int i = 0; i < ITER; ++i) {
                    std::string s(std_s);
                    volatile auto d = s.data();
                }
            }
            {
                Timer t("  dast::cstring 构造");
                for (int i = 0; i < ITER; ++i) {
                    dast::cstring s(dst_s);
                    volatile auto d = s.data();
                }
            }

            {
                Timer t("  std::string 拷贝");
                for (int i = 0; i < ITER; ++i) {
                    std::string s;
                    s = std_s;
                    volatile auto d = s.size();
                }
            }
        }
        std::cout << std::endl;

        // --- 10b. SSO 边界：20 字节（std 堆分配，dast 仍在 SSO 内）---
        std::cout << "  --- 10b. 20 字节（关键优势区：std 堆分配，dast 仍在 SSO）---" << std::endl;
        {
            std::string std_s = generate_random_string(20);
            dast::cstring dst_s(std_s.c_str());

            {
                Timer t("  std::string 构造");
                for (int i = 0; i < ITER; ++i) {
                    std::string s(std_s);
                    volatile auto d = s.data();
                }
            }
            {
                Timer t("  dast::cstring 构造");
                for (int i = 0; i < ITER; ++i) {
                    dast::cstring s(dst_s);
                    volatile auto d = s.data();
                }
            }

            {
                Timer t("  std::string 拷贝");
                for (int i = 0; i < ITER; ++i) {
                    std::string s = std_s;
                    volatile auto d = s.size();
                }
            }
            {
                Timer t("  dast::cstring 拷贝");
                for (int i = 0; i < ITER; ++i) {
                    dast::cstring s = dst_s;
                    volatile auto d = s.size();
                }
            }

            {
                std::string dest;
                Timer t("  std::string 赋值");
                for (int i = 0; i < ITER; ++i) {
                    dest = std_s;
                    volatile auto d = dest.size();
                }
            }

            {
                dast::cstring dest;
                Timer t("  dast::string 赋值");
                for (int i = 0; i < ITER; ++i) {
                    dest = dst_s;
                    volatile auto d = dest.size();
                }
            }

        }
        std::cout << std::endl;

        // --- 10c. 超出 SSO：30 字节（两者都堆分配）---
        std::cout << "  --- 10c. 30 字节（两者都堆分配）---" << std::endl;
        {
            std::string std_s = generate_random_string(30);
            dast::cstring dst_s(std_s.c_str());

            {
                Timer t("  std::string 构造");
                for (int i = 0; i < ITER; ++i) {
                    std::string s(std_s);
                    volatile auto d = s.data();
                }
            }
            {
                Timer t("  dast::cstring 构造");
                for (int i = 0; i < ITER; ++i) {
                    dast::cstring s(dst_s);
                    volatile auto d = s.data();
                }
            }

            {
                Timer t("  std::string 拷贝");
                for (int i = 0; i < ITER; ++i) {
                    std::string s = std_s;
                    volatile auto d = s.size();
                }
            }
            {
                Timer t("  dast::cstring 拷贝");
                for (int i = 0; i < ITER; ++i) {
                    dast::cstring s = dst_s;
                    volatile auto d = s.size();
                }
            }
        }
        std::cout << std::endl;
    }

    std::cout << "========== 测试完成 ==========" << std::endl;
    return 0;
}