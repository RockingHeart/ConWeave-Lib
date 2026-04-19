# 此为介绍类与其接口的文档

## 字符串接口快照表

| Interfaces | Shoots |
| :---: | :---: |
| construct | [construct] |
| at | [at] |
| element | [element] |
| swap | [swap] |
| move | [move] |
| begin and end | [begin_and_end] |
| cut | [cut] |
| reverse | [reverse] |
| trimmed | [trimmed] |
| lower | [lower] |
| upper | [upper] |
| index | [index] |
| clear residue | [clear_residue] |
| leave residue | [leave_residue] |
| const string | [const_string] |
| truncate | [truncate] |
| append | [append] |
| center | [center] |
| left | [left] |
| right | [right] |
| expand prefix | [expand_prefix] |
| empty | [empty] |
| is blank | [is_blank] |
| is digit | [is_digit] |
| is lower | [is_lower] |
| is upper | [is_upper] |
| is enstr | [is_enstr] |
| is ascii | [is_ascii] |
| max size | [max_size] |
| info | [info] |
| operator | [operator] |
| replace | [replace] |
| insert | [insert] |
| exclusive or | [exclusive_or] |
| residue | [residue] |
| resize | [resize] |
| reserve | [reserve] |
| restore cache mode | [restore_cache_mode] |
| toggle large mode | [toggle_large_mode] |
| size | [size] |
| tick | [tick] |
| to | [to] |
| to lower | [to_lower] |
| swapcase | [swapcase] |
| to upper | [to_upper] |

- String

    > basic_string是模板类
    >    - 它需要填入string_traits
    >        - string_traits需要满足string_traits_type约束

    > string_trait是模板类
    >    - 它需要填入char_t与value_traits

    > value_traits是类枚举
    >    - no_residue
    >    - remain

    - 作用：动态存储字符串
        - 类型描述：若string_trait的模板参数为char，则char_t为char，reference为char&，pointer_t为char*，const_pointer_t为const char*，以此类推
        - 以下为接口：
            ## basic_string
            - 返回值类型：basic_string 构造函数名：basic_string
                - 参数列表：(void)
                - basic_string(void)
                    - 用于构造空状态动态字符串 <br>
                - 参数列表：(const_pointer_t)
                - basic_string(const_pointer_t str)
                    - 用于构造具有str的动态字符串 <br>
                        **参数str用于传入需存储的字符串，自动获取其字符串大小**
                - 参数列表：(const_pointer_t, size_t)
                - basic_string(const_pointer_t str, size_t size)
                    - 用于构造具有size偏移str的动态字符串 <br>
                        **参数str用于传入需存储的字符串 <br> 参数size用于告知构造函数需存储的字符串大小**
                - 模板：template<size_type SizeType> 返回值类型：basic_string basic_string 参数列表：(SizeType, char_t)
                - template<size_type SizeType> basic_string(char_t char_value, SizeType size)
                    - 用于构造多个单个字符的动态字符串 <br>
                        **参数size用于传入此方法构造需要的长度**
                - 参数列表：(const basic_string&)
                - basic_string(const basic_string& object)
                    - 用于构造与object相同的动态字符串副本 <br>
                - 参数列表：(basic_string&&)
                - basic_string(basic_string&& object)
                    - 用于构造转移从object的所有权 <br>
                - 参数列表：(char_t)
                - basic_string(char_t char_value)
                    - 用于构造以单个字符串char_value为内容的动态字符串 <br>
                - 参数列表：(wrap::char_wrap)
                - basic_string(const wrap::char_wrap& wrap)
                    - 用于编译期构造 <br>
                
            ## at
            - 返回值类型：reference_t 非静态成员函数名：at
                - 参数列表：(size_t)
                - reference_t at(size_t position)
                    - 用于获取指定位置的字符
                        - 若position超出正常范围，则抛出错误。若抛出错误，其需捕获类型为const char*
                    ``` C++
                    import string;
                    import std;
                    
                    int main() {
                        dast::cstring str = { "Hello", 5 };
                        std::cout << str.at(0) << '\n';
                        return 0;
                    }
                    ```
                    #### 以上代码输出：'H'
                    该函数强调的返回描述：无
            
            ## element
            - 返回值类型：char_t 非静态成员函数名：element
                - 参数列表：(size_t)
                - char_t element(size_t position)
                    - 用于获取指定位置的字符
                        - 若position超出正常范围，则返回空（Null）字符。
                    ``` C++
                    import string;
                    import std;

                    int main() {
                        dast::cstring str = { "Hello", 5 };
                        std::cout << str.const_string() << '\n';
                        return 0;
                    }
                    ```
                    #### 以上代码输出：'H'
                    该函数强调的返回描述：无

            ## swap
            - 返回值类型：void 非静态成员函数名：swap
                - 参数列表：(basic_string&)
                - void swap(basic_string& object)
                    - 用于交换动态字符串
                    ``` C++
                    import string;
                    import std;

                    int main() {
                        dast::cstring str1 = { "Hello", 5 };
                        dast::cstring str2 = { "World", 5 };
                        str1.swap(str2);
                        std::cout << str.const_string() << '\n';
                        return 0;
                    }
                    ```
                    #### 以上代码输出："World"
                    该函数强调的返回描述：无

            ## move
            - 返回值类型：void 非静态成员函数名：move
                - 参数列表：(basic_string&)
                - void move(basic_string& object)
                    - 用于交换动态字符串
                    ``` C++
                    import string;
                    import std;

                    int main() {
                        dast::cstring str1 = { "Hello", 5 };
                        dast::cstring str2 = { "World", 5 };
                        str1.move(str2);
                        std::cout << str.const_string() << '\n';
                        return 0;
                    }
                    ```
                    #### 以上代码输出："World"
                    该函数强调的返回描述：无

            
            ## begin and end
            - 返回值类型：pointer_t 非静态成员函数名：begin 参数列表：(void) 与 返回值类型：pointer_t 非静态成员函数名：end 参数列表：（void）
            - pointer_t begin(void) 与 pointer_t end(void)
                - begin与end用于支持范围for
                ``` C++
                import string;
                import std;

                int main() {
                    dast::cstring str = { "Hello", 5 };
                    for (auto& c : str) {
                        std::cout << c << ' ';
                    } std::cout << '\n';
                    return 0;
                }
                ```
                #### 以上代码输出："H e l l o"
                该函数强调的返回描述：无

            ## cut
            - 返回值类型：size_t 非静态成员函数名：cut 参数列表：(char_t)
            - size_t cut(char_t char_value)
                - 用于获取头尾需裁剪的字符计数
                ``` C++
                import string;
                import std;

                int main() {
                    dast::cstring str = { " Hello ", 7 };
                    std::cout << str.cut(' ');
                    return 0;
                }
                ```
                #### 以上代码输出：2
                该函数强调的返回描述：无

            ## reverse
            - 返回值类型：basic_string::reverse_iterator 非静态成员函数名：reverse 参数列表：(void)
            - basic_string::reverse_iterator reverse(void)
                - 用于新建并返回反转后的动态字符串
                ``` C++
                import string;
                import std;

                int main() {
                    dast::cstring str = { "Hello", 5 };
                    for (auto& v : str.reverse()) {
                        std::cout << v << ' ';
                    }
                    return 0;
                }
                ```
                #### 以上代码输出："o l l e H"
                该函数强调的返回描述：无

            ## trimmed
            - 返回值类型：size_t 非静态成员函数名：trimmed 参数列表：(char_t)
            - size_t trimmed(char_t char_value = ' ')
                - 用于裁剪头尾的char_value
                ``` C++
                import string;
                import std;

                int main() {
                    dast::cstring str = { " Hello ", 5 };
                    std::cout << str.trimmed() << '\n';
                    std::cout << str.const_string() << '\n';
                    return 0;
                }
                ```
                #### 以上代码第一次输出：2
                #### 以上代码第二次输出："Hello"
                该函数强调的返回描述：返回裁剪数目

            ## lower
            - 返回值类型：basic_string 非静态成员函数名：lower 参数列表：(void)
            - basic_string lower()
                - 用于对一个指定动态字符串对象创建小写的副本
                ``` C++
                import string;
                import std;

                int main() {
                    dast::cstring str = { "HELLO", 5 };
                    auto lower = str.lower();
                    std::cout << lower.const_string() << '\n';
                    return 0;
                }
                ```
                #### 以上代码输出："hello"
                该函数强调的返回描述：无

            ## upper
            - 返回值类型：basic_string 非静态成员函数名：upper 参数列表：(void)
            - basic_string upper()
                - 用于对一个指定动态字符串对象创建大写的副本
                ``` C++
                import string;
                import std;

                int main() {
                    dast::cstring str = { "hello", 5 };
                    auto upper = str.upper();
                    std::cout << upper.const_string() << '\n';
                    return 0;
                }
                ```
                #### 以上代码输出："HELLO"
                该函数强调的返回描述：无

            ## index
            - 非静态成员函数名：index
                - 返回值类型：match<size_t> 
                - 参数列表：(char_t, size_t, size_t)
                - match<size_t> index(char_t target, size_t point, size_t end)
                    - 用于查找指定目标的索引
                        - 当参数point大于end时，反向查找
                        - 当参数point小于end时，正向查找
                    ``` C++
                    import string;
                    import std;
                    
                    int main() {
                        dast::cstring str = { "Hello", 5 };
                        std::cout << str.index('e', 0, str.size()) << '\n';
                        std::cout << str.index('l', str.size(), 0) << '\n';
                        return 0;
                    }
                    ```
                    #### 以上代码第一次输出：1
                    #### 以上代码第二次输出：3
                    函数返回值描述：其返回值类型为模板结构体：template <ResultType> match，有成员为bool类型的found与Result类型的result，found存储查询成功结果，result存储查询结果。
                
                - 返回值类型：bool
                - 参数列表：(size_t)
                - bool index(size_t position)
                - 用于判断position处于合法下标范围
                    ``` C++
                    import string;
                    import std;

                    int main() {
                        dast::cstring str = { "Hello", 5 };
                        std::cout << str.index(4) << '\n'
                        return 0;
                    }
                    ```
                    #### 以上代码输出： true
                    函数返回值描述：合法则返回true，否则返回false

                    
            
            ## clear_residue
            - 返回值类型：bool 非静态成员函数名：clear_residue 参数列表：(void)
            - bool clear_residue(void)
                - 用于清除保留数据
                    - 它会在清除之前预先检查是否存有保留数据
                    - 检查未存有保留数据则该函数调用失败
                    - 检查存有保留数据并清除保留数据之后返回true
                ``` C++
                int main() {
                    dast::cstring str = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";
                    str.resize(60);
                    std::cout << str.clear_residue() << '\n';
                    return 0;
                }
                ```
                #### 以上代码输出：'1'

            ## leave_residue
            - 返回值类型：bool 非静态成员函数名：leave_residue 参数列表：(void)
            - bool leave_residue(void)
                - 用于检查是否存有保留数据
                    - 以下条件均满足则存有保留数据，返回true
                        - 是大模式
                        - 记录保留数据的成员数据非空
                ``` C++
                import string;
                import std;

                int main() {
                    dast::cstring str = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";
                    str.resize(60);
                    std::cout << str.leave_residue() << '\n';
                    return 0;
                }
                ```
                #### 以上代码输出：'1'

            ## const_string
            - 返回值类型：const_pointer_t 非静态成员函数名：const_string 参数列表：(void)
            - const_pointer_t const_string(void)
                - 用于获取动态字符串
                    ``` C++
                    import string;
                    import std;

                    int main() {
                        dast::cstring str = { "Hello", 5 };
                        std::cout << str.const_string() << '\n'
                        return 0;
                    }
                    ```
                    #### 以上代码输出："Hello"
                    该函数强调的返回描述：无

            ## truncate
            - 返回值类型：basic_string 非静态成员函数名：truncate
                - 参数列表：(size_t, size_t)
                - basic_string truncate(size_t point, size_t end)
                    - 用于截取字符串区间的字符串 <br>
                    **参数point为截取的起始位置 <br> 参数end为截取的终止位置**
                    ``` C++
                    import string;
                    import std;

                    int main() {
                        dast::cstring str = { "Hello", 5 };
                        std::cout << str.truncate(1, 4).const_string() << '\n'
                        return 0;
                    }
                    ```
                    #### 以上代码输出："ell"
                    函数返回值描述：返回截取后的字符串
            
            ## center
            - 返回值类型：void 非静态成员函数名：center
                - 参数列表：(char_t, size_t) 
                    - void center(char_t fill, size_t size = 1)
                    - 用于居中对齐
                        ``` C++
                        import string;
                        import std;

                        int main() {
                            dast::cstring str = { "Hello", 5 };
                            str.center('*', 5)
                            std::cout << str.const_string() << '\n'
                            return 0;
                        }
                        ```
                    #### 以上代码输出："\*\*\*\*\*Hello\*\*\*\*\*"
                    该函数强调的返回描述：无

                - 参数列表：(const_pointer_t) 
                    - void center(const_pointer_t fill)
                    - 用于居中对齐
                        ``` C++
                        import string;
                        import std;

                        int main() {
                            dast::cstring str = { "Hello", 5 };
                            str.center("*****")
                            std::cout << str.const_string() << '\n'
                            return 0;
                        }
                        ```
                    #### 以上代码输出："\*\*\*\*\*Hello\*\*\*\*\*"
                    该函数强调的返回描述：无

            ## append
            - 返回值类型：void 非静态成员函数名：append
                - 参数为模板形参包
                    - void append(ArgsType...)
                    - 用于在末尾添加多个参数的子字符的串
                        ``` C++
                        import string;
                        import std;

                        int main() {
                            dast::cstring str = { "Hello", 5 };
                            str.append('.', "Hello", '.', str)
                            std::cout << str.const_string() << '\n'
                            return 0;
                        }
                        ```
                    #### 以上代码输出："Hello.Hello.Hello.Hello"
                    该函数强调的返回描述：无

            ## left
            - 返回值类型：void 非静态成员函数名：left
                - 参数列表：(char_t, size_t) 
                    - void left(char_t fill, size_t size = 1)
                    - 用于左对齐
                        ``` C++
                        import string;
                        import std;

                        int main() {
                            dast::cstring str = { "Hello", 5 };
                            str.left('*', 5)
                            std::cout << str.const_string() << '\n'
                            return 0;
                        }
                        ```
                    #### 以上代码输出："\*\*\*\*\*Hello"
                    该函数强调的返回描述：无

                - 参数列表：(const_pointer_t) 
                    - void left(const_pointer_t fill)
                    - 用于左对齐
                        ``` C++
                        import string;
                        import std;

                        int main() {
                            dast::cstring str = { "Hello", 5 };
                            str.left("*****")
                            std::cout << str.const_string() << '\n'
                            return 0;
                        }
                        ```
                    #### 以上代码输出："\*\*\*\*\*Hello"
                    该函数强调的返回描述：无

            ## right
            - 返回值类型：void 非静态成员函数名：right
                - 参数列表：(char_t, size_t) 
                    - void right(char_t fill, size_t size = 1)
                    - 用于右对齐
                        ``` C++
                        import string;
                        import std;

                        int main() {
                            dast::cstring str = { "Hello", 5 };
                            str.right('*', 5)
                            std::cout << str.const_string() << '\n'
                            return 0;
                        }
                        ```
                    #### 以上代码输出："Hello\*\*\*\*\*"
                    该函数强调的返回描述：无

                - 参数列表：(const_pointer_t) 
                    - void right(const_pointer_t fill)
                    - 用于右对齐
                        ``` C++
                        import string;
                        import std;

                        int main() {
                            dast::cstring str = { "Hello", 5 };
                            str.right("*****")
                            std::cout << str.const_string() << '\n'
                            return 0;
                        }
                        ```
                    #### 以上代码输出："Hello\*\*\*\*\*"
                    该函数强调的返回描述：无
            
            ## expand_prefix
            - 返回值类型：void 非静态成员函数名：expand_prefix
                - 参数列表：(const_pointer_t)
                    - void expand_prefix(const_pointer_t str)
                    - 用于在前缀位置使动态字符串扩展子字符串
                        ``` C++
                        import string;
                        import std;

                        int main() {
                            dast::cstring str = { " World", 5 };
                            str.expand_prefix("Hello")
                            std::cout << str.const_string() << '\n'
                            return 0;
                        }
                        ```
                    #### 以上代码输出："Hello World"
                    该函数强调的返回描述：无


            
            ## empty
            - 返回值类型：bool 非静态成员函数名：empty
                - 参数列表：(void)
                - bool empty(void)
                    - 用于检测动态字符串是否为空
                    ``` C++
                    import string;
                    import std;

                    int main() {
                        dast::cstring str = { "Hello", 5 };
                        std::cout << str.empty() << '\n'
                        return 0;
                    }
                    ```
                    #### 以上代码输出：false
                    该函数强调的返回描述：无
            
            ## is_blank
            - 返回值类型：bool 非静态成员函数名：is_blank
                - 参数列表：(void)
                - bool is_blank(void)
                    - 用于检测动态字符串中各元素是否为空格字符
                    ``` C++
                    import string;
                    import std;

                    int main() {
                        dast::cstring str = { "     ", 5 };
                        std::cout << str.is_blank() << '\n'
                        return 0;
                    }
                    ```
                    #### 以上代码输出：true
                    该函数强调的返回描述：无

            ## is_digit
            - 返回值类型：bool 非静态成员函数名：is_digit
                - 参数列表：(void)
                - bool is_digit(void)
                    - 用于检测动态字符串中各元素是否为数字
                    ``` C++
                    import string;
                    import std;

                    int main() {
                        dast::cstring str = { "12345", 5 };
                        std::cout << str.is_digit() << '\n'
                        return 0;
                    }
                    ```
                    #### 以上代码输出：true
                    该函数强调的返回描述：无

            ## is_lower
            - 返回值类型：bool 非静态成员函数名：is_lower
                - 参数列表：(void)
                - bool is_lower(void)
                    - 用于检测动态字符串中各元素是否为小写的英文单词
                    ``` C++
                    import string;
                    import std;

                    int main() {
                        dast::cstring str = { "abcde", 5 };
                        std::cout << str.is_lower() << '\n'
                        return 0;
                    }
                    ```
                    #### 以上代码输出：true
                    该函数强调的返回描述：无

            ## is_upper
            - 返回值类型：bool 非静态成员函数名：is_upper
                - 参数列表：(void)
                - bool is_upper(void)
                    - 用于检测动态字符串中各元素是否为大写的英文单词
                    ``` C++
                    import string;
                    import std;

                    int main() {
                        dast::cstring str = { "ABCDE", 5 };
                        std::cout << str.is_upper() << '\n'
                        return 0;
                    }
                    ```
                    #### 以上代码输出：true
                    该函数强调的返回描述：无

            ## is_enstr
            - 返回值类型：bool 非静态成员函数名：is_enstr
                - 参数列表：(void)
                - bool is_enstr(void)
                    - 用于检测动态字符串中各元素是否为英文单词
                    ``` C++
                    import string;
                    import std;

                    int main() {
                        dast::cstring str = { "ABCDEfg", 7 };
                        std::cout << str.is_enstr() << '\n'
                        return 0;
                    }
                    ```
                    #### 以上代码输出：true
                    该函数强调的返回描述：无

            ## is_ascii
            - 返回值类型：bool 非静态成员函数名：is_ascii
                - 参数列表：(void)
                - bool is_ascii(void)
                    - 用于检测动态字符串中各元素是否为ascii码
                    ``` C++
                    import string;
                    import std;

                    int main() {
                        dast::cstring str = { "abc", 3 };
                        std::cout << str.is_ascii() << '\n'
                        return 0;
                    }
                    ```
                    #### 以上代码输出：true
                    该函数强调的返回描述：无

            
            ## max_size
            - 返回值类型：size_t 非静态成员函数名：max_size 参数列表：(void)
            - size_t max_size(void)
                - 用于获取当前动态字符串最大存储大小
                    ``` C++
                    import string;
                    import std;

                    int main() {
                        dast::cstring str1 = { "Hello", 5 };
                        std::cout << str.max_size() << '\n'

                        dast::cstring str2 = { "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAA", 30 };
                        std::cout << str.max_size() << '\n'
                        return 0;
                    }
                    ```
                    #### 以上代码str1输出24（缓存阈值大小）<br> 以上代码str2输出60（成员堆大小阈值。请注意：当前该阈值使用构造的字符串大小乘以2以计算，此算法待更改）
                    该函数强调的返回描述：无
            
            ## info
            - 返回值类型：string_info 非静态成员函数名：info 参数列表：(void)
            - string_info info(void)
                - 用于获取字符串的信息
                    ``` C++
                    import string;
                    import std;

                    int main() {
                        dast::cstring str = "Hello";
                        str.exclusive_or('k');
                        string_info info = str.info();
                        string_mode mode = info.modes;
                        bool is_xored = info.is_xored;
                        std::cout << (mode == string_mode::cache) << '\n';
                        std::cout << is_xored << '\n';
                        return 0;
                    }
                    ```
                    #### 以上代码依次输出：1 -> 1
                    函数返回值类型描述：string_info内部的modes是mode_status枚举类型，其内部有cache与storage两种值，对应缓存模式与大模式。

            ## replace
            - 返回值类型：bool 非静态成员函数名：replace
            - 用于重置指定位置的动态字符串
                - 参数列表：(const_pointer_t, size_t, size_t)
                - bool replace(const_pointer_t str, size_t point, size_t end) <br>
                    **参数str为待替换的字符串 <br> 参数point为指定位置的起始位置 <br> 参数end为指定位置的终止位置**
                    ``` C++
                    import string;
                    import std;

                    int main() {
                        dast::cstring str = { "Hello", 5 };
                        str.replace("World", 0, 5);
                        std::cout << str.const_string() << '\n'
                        return 0;
                    }
                    ```
                    #### 以上代码输出："World"
                    函数返回值描述：此函数返回bool值，指告用户replace成功与否

                - 参数列表：(reference, size_t, size_t)
                - bool replace(reference char_value, size_t point, size_t end)<br>
                    **参数char_value为待替换的单个字符 <br> 参数point为指定位置的起始位置 <br> 参数end为指定位置的终止位置**
                    ``` C++
                    import string;
                    import std;

                    int main() {
                        dast::cstring str = { "Hello", 5 };
                        str.replace('A', 0, 5);
                        std::cout << str.const_string() << '\n'
                        return 0;
                    }
                    ```
                    #### 以上代码输出：49
                    注：保留数据仅当remain特性实例化的basic_string变量满足以下条件时保留
                        - 变量是大模式
                        - 调用resize更新变量为更大的动态字符串
                    函数返回值描述：若存有保留数据，则返回其保留数据

            ## insert
            - 返回值类型：bool 非静态成员函数名：insert
            - 用于在指定位置插入字符串
                - 参数列表：(const_pointer_t, size_t)
                - bool insert(const_pointer_t str, size_t position) <br>
                    **参数str为插入的字符串 <br> 参数position为插入位置位置**
                    ``` C++
                    import string;
                    import std;

                    int main() {
                        dast::cstring str = "Hd";
                        str.insert("ello Worl", 0);
                        std::cout << str.const_string();
                        return 0;
                    }
                    ```
                    #### 以上代码输出："Hello World"
                    函数返回值描述：此函数返回bool值，指告用户insert成功与否

            ## exclusive_or
            - 返回值类型：basic_string 非静态成员函数名：exclusive_or
                - 用于异或动态字符串对象
                    - 参数列表：(char_t)
                    - bool exclusive_or(char_t key) <br>
                    **参数key为异或的值**
                    ``` C++
                    import string;
                    import std;

                    int main() {
                        dast::cstring str = "Hello";
                        str.exclusive_or('.');
                        std::cout << str.const_string();
                        return 0;
                    }
                    ```
                    #### 以上代码输出："fKBBA"
                    函数返回值描述：const下复制一份动态字符串对象自身，并异或其复制体，非cosnt下异或动态字符串对象自身
            
            ## residue
            - 返回值类型：const residue_info 非静态成员函数名：residue 参数列表：(void)
            - const residue_info residue(void)
                - 用于获取保留数据
                ``` C++
                    import string;
                    import std;

                    int main() {
                        dast::cstring str = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";
                        str.resize(60);
                        if (str.leave_residue()) {
	                        std::cout << str.residue().size << '\n';
                        }
                        return 0;
                    }
                ```
                #### 以上代码输出："AAAAA"
                函数返回值描述：此函数返回bool值，指告用户replace成功与否

            ## resize
            - 返回值类型：bool 非静态成员函数名：resize 参数列表：(size_t, char_t)
            - bool resize(size_t size, char_t fill)
                - 用于扩展与扩展动态字符串<br>
                    **参数size用于控制重置的大小**
                    **参数fill用于填充扩展**
                    ``` C++
                    import string;
                    import std;

                    int main() {
                        dast::cstring str1 = { "Hello", 5 };

                        auto print_str = [&]() {
                            std::cout << str.const_string() << ' '
                            std::cout << static_cast<size_t>(str.mode()) << ' '
                            std::cout << str.max_size() << '\n'
                        };

                        str.resize(12);
                        print_str();

                        str.resize(30);
                        print_str();
                        return 0;
                    }
                    ```
                    #### 以上代码第一次输出："Hello" 0 24<br>以上代码第二次输出："Hello" 1 30
                    函数返回值描述：此函数返回bool值，指告用户resize成功与否

            ## reserve
            - 返回值类型：bool 非静态成员函数名：reserve 参数列表：(size_t)
            - bool reserve(size_t size)
                - 用于保留与扩展动态字符串<br>
                    **参数size用于控制重置的大小**
                    - 若size小于当前存储大小，则函数返回false（视为失败）
                        - 否则进行扩展，函数返回true（视为成功）
                    ``` C++
                    import string;
                    import std;

                    int main() {
                        dast::cstring str1 = { "Hello", 5 };
                        str1.reserve(50);
                        std::cout << str1.max_size() << ' ' << str1.size() << '\n';
                        return 0;
                    }
                    ```
                    #### 以上代码输出："50 5"
                    该函数强调的返回描述：若扩展成功，它并不会更改原有存储大小与元素

            ## restore_cache_mode
            - 返回值类型：bool 非静态成员函数名：restore_cache_mode
                - 参数列表：(void)
                - bool restore_cache_mode(void)
                    - 用于恢复至缓存模式
                        - 此无参重载将自动计算恢复长度
                            - 若是大模式
                                - 返回计算的长度
                                    - 长度按照剩余空间计算
                                        - 剩余空间越大，结果越小，但最小不小于0
                                        - 剩余空间越小，结果越大，但最大不大于缓存最大空间
                            - 若是小模式
                                - 不做任何改变
                        - 自动计算长度保证动态修改大小符合空间所需
                    ``` C++
                    import string;
                    import std;

                    int main() {
                        dast::cstring str = "Hello AAAAAAAAAAA";
                        str.restore_cache_mode();
                        std::cout << str.size() << '\n';
                        return 0;
                    }
                    ```
                    #### 以上代码输出：":AAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
                    函数返回值描述：恢复成功则返回true，反之回false
                    
                - 参数列表：(size_t)
                - bool restore_cache_mode(size_t size)
                    - 用于恢复至缓存模式
                        - 调用此size_t重载，若成功，缓存大小设定为size
                    ``` C++
                    import string;
                    import std;

                    int main() {
                        dast::cstring str1 = { 'A', 50 };
                        str.restore_cache_mode(12);
                        std::cout << str.const_string() << '\n';
                        return 0;
                    }
                    ```
                     #### 以上代码输出："AAAAAAAAAAAA"
                    函数返回值描述：恢复成功则返回true，反之回false

            ## toggle_large_mode
            - 返回值类型：bool 非静态成员函数名：toggle_large_mode
                - 参数列表：(void)
                - bool toggle_large_mode(void)
                    - 用于手动切换至大模式
                    ``` C++
                    import string;
                    import std;

                    int main() {
                        dast::cstring str = "Hello";
                        str.toggle_large_mode();
                        std::cout << (str.mode() == mode_status::storage) << '\n';
                        return 0;
                    }
                    ```
                    #### 以上代码输出：1
                    函数返回值描述：若动态字符串对象不处于缓存模式，则该函数返回false，否则为true

            ## size
            - 返回值类型：size_t 非静态成员函数名：size 参数列表：(void)
            - size_t size(void)
                - 用于获取当前动态字符串存储大小
                    ``` C++
                    import string;
                    import std;

                    int main() {
                        dast::cstring str = { "Hello", 5 };
                        std::cout << str.size() << '\n'
                        return 0;
                    }
                    ```
                    #### 以上代码输出： 5
                    该函数强调的返回描述：无

            ## tick
            - 返回值类型：size_t 非静态成员函数名：tick
                - 参数列表: (char_t, size_t, size_t)
                - size_t tick(char_t char_value, size_t point, size_t end)
                    - 用于计数字符串内的char_value <br>
                    **参数char_value为查找的目标 <br> 参数point为查找的起始位置 <br> 参数end为查找的终止位置**
                    ``` C++
                    import string;
                    import std;

                    int main() {
                        dast::cstring str = { "Hello", 5 };
                        std::cout << str.tick('l', 0, str.size()) << '\n'
                        return 0;
                    }
                    ```
                    #### 以上代码输出：2
                    函数返回值描述：返回查找的计数结果

            ## to
            - 模板参数：<CastType> 返回值类型：CastType 非静态模板成员函数名：to
                - 参数列表: (void)
                - CastType to(void)
                    - 用于将动态字符串对象转换为CastType类型对象
                    - 需要填入模板参数
                    ``` C++
                    import string;
                    import std;

                    int main() {
                        dast::cstring str = { "Hello", 5 };
                        std::string std_str = str.to<std::string>();
                        std::cout << std_str << '\n'
                        return 0;
                    }
                    ```
                    #### 以上代码输出： "Hello"
                    函数返回值描述：它由CastType所构造

            ## to_lower
            - 返回值类型：basic_string& 非静态模板成员函数名：to_lower
                - 参数列表: (void)
                - basic_string& to_lower(void)
                    - 用于将自身动态字符串对象的大写英文单词元素转换为小写
                    ``` C++
                    import string;
                    import std;

                    int main() {
                        dast::cstring str = { "HELLO", 5 };
                        std::cout << std_str.to_lower().const_string() << '\n'
                        return 0;
                    }
                    ```
                    #### 以上代码输出： "hello"
                    函数返回值描述：返回修改后的自身对象
            
            ## swapcase
            - 返回值类型：basic_string& 非静态模板成员函数名：swapcase
                - 参数列表: (void)
                - basic_string& swapcase(void)
                    - 用于将自身动态字符串对象大小写英文单词元素转换为与之大小写相反的英文单词
                    ``` C++
                    import string;
                    import std;

                    int main() {
                        dast::cstring str = { "hELLO", 5 };
                        std::cout << std_str.swapcase().const_string() << '\n'
                        return 0;
                    }
                    ```
                    #### 以上代码输出： "Hello"
                    函数返回值描述：返回修改后的自身对象

            ## to_upper
            - 返回值类型：basic_string& 非静态模板成员函数名：to_upper
                - 参数列表: (void)
                - basic_string& to_upper(void)
                    - 用于将自身动态字符串对象的小写英文单词元素转换为大写
                    ``` C++
                    import string;
                    import std;

                    int main() {
                        dast::cstring str = { "hello", 5 };
                        std::cout << std_str.to_upper().const_string() << '\n'
                        return 0;
                    }
                    ```
                    #### 以上代码输出： "HELLO"
                    函数返回值描述：返回修改后的自身对象
                
                - 参数列表：(size_t)
                - CastType to(size_t offset)
                    - 用于将动态字符串对象偏移offset数目的字符串转换为CastType类型对象
                    - 需要填入模板参数
                    ``` C++
                    import string;
                    import std;

                    int main() {
                        dast::cstring str = { "Hello", 5 };
                        std::string std_str = str.to<std::string>(1);
                        std::cout << std_str << '\n'
                        return 0;
                    }
                    ```
                    #### 以上代码输出： "ello"
                    函数返回值描述：它由CastType所构造

            ## operator
            - operator+
            - 返回值类型：basic_string 非静态成员运算符重载函数符号：+
                - 参数列表：(char_t)
                - basic_string operator+(char_t char_value)
                    - 用于组合新建一个动态字符串对象
                        ``` C++
                        import string;
                        import std;

                        int main() {
                            dast::cstring str = { "Hello", 5 };
                            dast::cstring str2 = str + 'A';
                            std::cout << str2.const_string() << '\n'
                            return 0;
                        }
                        ```
                        #### 以上代码输出："HelloA"
                        函数返回值描述：返回调用operator+=的对象

                - 参数列表：(const_pointer)
                - basic_string operator+(const_pointer pointer)
                    - 用于组合新建一个动态字符串对象
                        ``` C++
                        import string;
                        import std;

                        int main() {
                            dast::cstring str = { "Hello", 5 };
                            dast::cstring str2 = str + " World";
                            std::cout << str2.const_string() << '\n'
                            return 0;
                        }
                        ```
                        #### 以上代码输出："Hello World"
                        函数返回值描述：返回调用operator+=的对象

                - 参数列表：(basic_string&)
                - basic_string operator+(basic_string& object)
                    - 用于组合新建一个动态字符串对象
                        ``` C++
                        import string;
                        import std;

                        int main() {
                            dast::cstring str1 = { "Hello", 5 };
                            dast::cstring str2 = { " Wello", 5 };
                            dast::cstring str3 = str1 + str2;
                            std::cout << str2.const_string() << '\n'
                            return 0;
                        }
                        ```
                        #### 以上代码输出："Hello World"
                        函数返回值描述：返回调用operator+=的对象

            - operator+=
            - 返回值类型：basic_string& 非静态成员运算符重载函数符号：+=
            - 注：于enhance模式中，其返回值类型为adder
                - 参数列表：(char_t)
                - basic_string& operator+=(char_t char_value)
                    - 用于扩展动态字符串
                        ``` C++
                        import string;
                        import std;

                        int main() {
                            dast::cstring str = { "Hello", 5 };
                            str += 'A';
                            std::cout << str.const_string() << '\n'
                            return 0;
                        }
                        ```
                        #### 以上代码输出："HelloA"
                        函数返回值描述：返回调用operator+=的对象
                
                - 参数列表：(const_pointer)
                - basic_string& operator+=(const_pointer pointer)
                    - 用于扩展动态字符串
                        ``` C++
                        import string;
                        import std;

                        int main() {
                            dast::cstring str = { "Hello", 5 };
                            str += " World";
                            std::cout << str.const_string() << '\n'
                            return 0;
                        }
                        ```
                        #### 以上代码输出："Hello World"
                        函数返回值描述：返回调用operator+=的对象

                - 参数列表：(basic_string&)
                - basic_string& operator+=(basic_string& pointer)
                    - 用于扩展动态字符串
                        ``` C++
                        import string;
                        import std;

                        int main() {
                            dast::cstring str1 = { "Hello", 5 };
                            dast::cstring str2 = { "World", 5 };
                            str1 += str2;
                            std::cout << str.const_string() << '\n'
                            return 0;
                        }
                        ```
                        #### 以上代码输出："Hello World"
                        函数返回值描述：返回调用operator+=的对象
            
            - operator[]
            - 返回值类型：reference 非静态成员运算符重载函数符号：[]
                - 参数列表：(size_t)
                - reference operator[](size_t position)
                    - 用于获取指定位置的字符，而非字符串
                        ``` C++
                        import string;
                        import std;

                        int main() {
                            dast::cstring str = { "Hello", 5 };
                            std::cout << str[0] << '\n'
                            return 0;
                        }
                        ```
                        #### 以上代码输出：'H'
                        该函数强调的返回描述：无
            
            - operator==
            - 返回值类型：bool 非静态成员运算符重载函数符号：==
                - 参数列表：(char_t)
                - bool operator==(char_t char_value)
                    - 用于比较存储的内容是否等于char_value
                        ``` C++
                        import string;
                        import std;

                        int main() {
                            dast::cstring str = { "A", 1 };
                            std::cout << (str == 'A') << '\n'
                            return 0;
                        }
                        ```
                        #### 以上代码输出：1
                        该函数强调的返回描述：无
                
                - 参数列表：(const_pointer_t)
                - bool operator==(const_pointer_t str)
                    - 用于比较存储的内容是否等于str
                         ``` C++
                        import string;
                        import std;

                        int main() {
                            dast::cstring str = { "Hello", 1 };
                            std::cout << (str == "Hello") << '\n'
                            return 0;
                        }
                        ```
                        #### 以上代码输出：1
                        该函数强调的返回描述：无
                
            - operator !
            - 返回值类型：bool 非静态成员运算符重载函数符号：!
                - 参数列表：(void)
                - bool operator !(void)
                    - 用于检测动态字符串是否不为空
                         ``` C++
                        import string;
                        import std;

                        int main() {
                            dast::cstring str = { "Hello", 5 };
                            std::cout << (!str) << '\n'
                            return 0;
                        }
                        ```
                        #### 以上代码输出：true
                        该函数强调的返回描述：无
                        
            - operator const_pointer_t
            - 返回值类型与非静态成员运算符重载函数类型：const_pointer_t
                - 参数列表：(void)
                - operator const_pointer_t(void)
                    - 用于在合适的场景下转换为常量字符串指针
                         ``` C++
                        import string;
                        import std;

                        int main() {
                            dast::cstring str = { "Hello", 5 };
                            std::cout << str << '\n'
                            return 0;
                        }
                        ```
                        #### 以上代码输出："Hello"
                        该函数强调的返回描述：无
            
            - operator =
            - 返回值类型：basic_string& 非静态成员运算符重载函数符号：=
                - 参数列表：(char_t)
                    - basic_string& operator =(char_t char_value)
                        - 用于赋值
                            ``` C++
                            import string;
                            import std;

                            int main() {
                                dast::cstring str1;
                                str1 = 'A';
                                std::cout << str1 << '\n'
                                return 0;
                            }
                            ```
                            #### 以上代码第一次输出："A"
                            该函数强调的返回描述：无

                    - 参数列表：(basic_string&)
                    - basic_string& operator =(basic_string& object)
                        - 用于赋值
                            ``` C++
                            import string;
                            import std;

                            int main() {
                                dast::cstring str1;
                                dast::cstring str2 = { "Hello", 5 };
                                str1 = str2;
                                std::cout << str1 << '\n'
                                return 0;
                            }
                            ```
                            #### 以上代码第一次输出："Hello"
                            该函数强调的返回描述：无
                    
                    - 参数列表：(basic_string&&)
                    - basic_string& operator =(basic_string&& object)
                        - 用于赋值移动运算
                            - 非加强模式
                                - 无条件地将object移动到self
                            - 是加强模式
                                - 若self与object皆不为空
                                    - self与object做交换
                                - 否则
                                    - 无条件地将object移动到self
                            - 返回移动后的self
                            ``` C++
                            import string;
                            import std;

                            int main() {
                                dast::cstring str1;
                                dast::cstring str2 = { "Hello", 5 };
                                str1 = std::move(str2);
                                std::cout << str1 << '\n'
                                std::cout << str2 << '\n'
                                return 0;
                            }
                            ```
                            #### 以上代码第一次输出："Hello"
                            #### 以上代码第二次输出：""
                            该函数强调的返回描述：无


[construct]: https://github.com/RockingHeart/ConWeave-Lib/blob/main/Describes/String/ClassInterface-String.md#basic_string
[at]: https://github.com/RockingHeart/ConWeave-Lib/blob/main/Describes/String/ClassInterface-String.md#at
[element]: https://github.com/RockingHeart/ConWeave-Lib/blob/main/Describes/String/ClassInterface-String.md#element
[swap]: https://github.com/RockingHeart/ConWeave-Lib/blob/main/Describes/String/ClassInterface-String.md#swap
[move]: https://github.com/RockingHeart/ConWeave-Lib/blob/main/Describes/String/ClassInterface-String.md#move
[begin_and_end]: https://github.com/RockingHeart/ConWeave-Lib/blob/main/Describes/String/ClassInterface-String.md#begin-and-end
[cut]: https://github.com/RockingHeart/ConWeave-Lib/blob/main/Describes/String/ClassInterface-String.md#cut
[reverse]: https://github.com/RockingHeart/ConWeave-Lib/blob/main/Describes/String/ClassInterface-String.md#reverse
[trimmed]: https://github.com/RockingHeart/ConWeave-Lib/blob/main/Describes/String/ClassInterface-String.md#trimmed
[lower]: https://github.com/RockingHeart/ConWeave-Lib/blob/main/Describes/String/ClassInterface-String.md#lower
[upper]: https://github.com/RockingHeart/ConWeave-Lib/blob/main/Describes/String/ClassInterface-String.md#upper
[index]: https://github.com/RockingHeart/ConWeave-Lib/blob/main/Describes/String/ClassInterface-String.md#index
[clear_residue]: https://github.com/RockingHeart/ConWeave-Lib/blob/main/Describes/String/ClassInterface-String.md#clear_residue
[leave_residue]: https://github.com/RockingHeart/ConWeave-Lib/blob/main/Describes/String/ClassInterface-String.md#leave_residue
[const_string]: https://github.com/RockingHeart/ConWeave-Lib/blob/main/Describes/String/ClassInterface-String.md#const_string
[truncate]: https://github.com/RockingHeart/ConWeave-Lib/blob/main/Describes/String/ClassInterface-String.md#truncate
[center]: https://github.com/RockingHeart/ConWeave-Lib/blob/main/Describes/String/ClassInterface-String.md#center
[append]: https://github.com/RockingHeart/ConWeave-Lib/blob/main/Describes/String/ClassInterface-String.md#append
[left]: https://github.com/RockingHeart/ConWeave-Lib/blob/main/Describes/String/ClassInterface-String.md#left
[right]: https://github.com/RockingHeart/ConWeave-Lib/blob/main/Describes/String/ClassInterface-String.md#right
[expand_prefix]: https://github.com/RockingHeart/ConWeave-Lib/blob/main/Describes/String/ClassInterface-String.md#expand_prefix
[empty]: https://github.com/RockingHeart/ConWeave-Lib/blob/main/Describes/String/ClassInterface-String.md#empty
[is_blank]: https://github.com/RockingHeart/ConWeave-Lib/blob/main/Describes/String/ClassInterface-String.md#is_blank
[is_digit]: https://github.com/RockingHeart/ConWeave-Lib/blob/main/Describes/String/ClassInterface-String.md#is_digit
[is_lower]: https://github.com/RockingHeart/ConWeave-Lib/blob/main/Describes/String/ClassInterface-String.md#is_lower
[is_upper]: https://github.com/RockingHeart/ConWeave-Lib/blob/main/Describes/String/ClassInterface-String.md#is_upper
[is_enstr]: https://github.com/RockingHeart/ConWeave-Lib/blob/main/Describes/String/ClassInterface-String.md#is_enstr
[max_size]: https://github.com/RockingHeart/ConWeave-Lib/blob/main/Describes/String/ClassInterface-String.md#max_size
[info]: https://github.com/RockingHeart/ConWeave-Lib/blob/main/Describes/String/ClassInterface-String.md#info
[operator]: https://github.com/RockingHeart/ConWeave-Lib/blob/main/Describes/String/ClassInterface-String.md#operator
[replace]: https://github.com/RockingHeart/ConWeave-Lib/blob/main/Describes/String/ClassInterface-String.md#replace
[insert]: https://github.com/RockingHeart/ConWeave-Lib/blob/main/Describes/String/ClassInterface-String.md#insert
[exclusive_or]: https://github.com/RockingHeart/ConWeave-Lib/blob/main/Describes/String/ClassInterface-String.md#exclusive_or
[residue]: https://github.com/RockingHeart/ConWeave-Lib/blob/main/Describes/String/ClassInterface-String.md#residue
[resize]: https://github.com/RockingHeart/ConWeave-Lib/blob/main/Describes/String/ClassInterface-String.md#resize
[reserve]: https://github.com/RockingHeart/ConWeave-Lib/blob/main/Describes/String/ClassInterface-String.md#reserve
[restore_cache_mode]: https://github.com/RockingHeart/ConWeave-Lib/blob/main/Describes/String/ClassInterface-String.md#restore_cache_mode
[toggle_large_mode]: https://github.com/RockingHeart/ConWeave-Lib/blob/main/Describes/String/ClassInterface-String.md#toggle_large_mode
[size]: https://github.com/RockingHeart/ConWeave-Lib/blob/main/Describes/String/ClassInterface-String.md#size
[tick]: https://github.com/RockingHeart/ConWeave-Lib/blob/main/Describes/String/ClassInterface-String.md#tick
[to]: https://github.com/RockingHeart/ConWeave-Lib/blob/main/Describes/String/ClassInterface-String.md#to
[to_lower]: https://github.com/RockingHeart/ConWeave-Lib/blob/main/Describes/String/ClassInterface-String.md#to_lower
[swapcase]: https://github.com/RockingHeart/ConWeave-Lib/blob/main/Describes/String/ClassInterface-String.md#swapcase
[to_upper]: https://github.com/RockingHeart/ConWeave-Lib/blob/main/Describes/String/ClassInterface-String.md#to_upper