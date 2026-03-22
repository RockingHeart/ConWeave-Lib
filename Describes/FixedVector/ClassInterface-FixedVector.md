# 此为介绍类与其接口的文档

## 定长向量接口快照表

| Interfaces | Shoots |
| :---: | :---: |
| construct | [construct] |
| push back | [push_back] |
| pop back | [pop_back] |
| begin and end | [begin_and_end] |
| size | [size] |
| capacity | [capacity] |
| max size | [max_size] |
| at | [at] |
| operator | [operator] |

- FixedVector

    > fixed_vector是模板类
        > 它需要填入值类型与长度

    - 作用：静态存储元素
        - 类型描述：若值类型为int，则reference为int&，以此类推
        - 以下为接口：
            ## construct
            - 返回值类型：fixed_vector 构造函数名：fixed_vector
                - 参数列表：(void)
                - fixed_vector(void)
                    - 用于构造空状态定长向量 <br>
                - 参数列表：(size_t)
                - fixed_vector(size_t size)
                    - 以默认值类型的构造方法构造定长度的元素 <br>
                - 参数列表：(initlist_t)
                - fixed_vector(const initlist_t& list)
                    - 以列表中的全部元素构造定长向量
            
            ## push_back
            - 返回值类型：bool
                - 参数列表：(const value_t&)
                - push_back(const value_t& value)
                    - 用于推入最后一个位置的元素
                    ``` C++
                    import fixed_vector;
                    import std;
                    
                    int main() {
                        dast::fixed_vector<int, 5> vec;
                        std::cout << vec.push_back(1);
                        return 0;
                    }
                    ```
                    #### 以上代码输出：true (1)
                    函数返回值描述：若无剩余空间，则返回false，否则返回true

             ## pop_back
            - 返回值类型：bool
                - 参数列表：(void)
                - pop_back()
                    - 用于弹出最后一个位置的元素
                    ``` C++
                    import fixed_vector;
                    import std;

                    int main() {
                        dast::fixed_vector<int, 5> vec;
                        for (size_t i = 0; i < 3; i++) {
                            vec.push_back(i);
                        }
                        std::cout << vec.pop_back();
                        return 0;
                    }
                    ```
                    #### 以上代码输出：true (1)
                    #### 弹出前有：0 1 2 弹出后：0 1
                    函数返回值描述：若无剩余空间，则返回false，否则返回true

            ## begin and end
            - 返回值类型：pointer_t 非静态成员函数名：begin 参数列表：(void) 与 返回值类型：pointer_t 非静态成员函数名：end 参数列表：（void）
            - pointer_t begin(void) 与 pointer_t end(void)
                - begin与end用于支持范围for
                ``` C++
                import fixed_vector;
                import std;

                int main() {
                    dast::fixed_vector<int, 5> vec;
                    size_t count = 0;
                    for (auto& v : vec) {
                        v = count;
                        count += 1;
                        std::cout << v << ' ';
                    } std::cout << '\n';
                    return 0;
                }
                ```
                #### 以上代码输出：0 1 2 3 4
                函数返回描述：无

            ## size
            - 返回值类型：size_t 非静态成员函数名：size 参数列表：(void)
            - size_t size(void)
                - 用于获取当前存储大小
                    ``` C++
                    import fixed_vector;
                    import std;

                    int main() {
                        dast::fixed_vector<int, 5> vec;
                        std::cout << vec.size() << '\n'
                        return 0;
                    }
                    ```
                    #### 以上代码输出： 0
                    函数返回描述：无

            ## capacity
            - 返回值类型：size_t 非静态成员函数名：capacity 参数列表：(void)
            - size_t capacity(void)
                - 用于获取剩余未使用的大小
                    ``` C++
                    import fixed_vector;
                    import std;

                    int main() {
                        dast::fixed_vector<int, 5> vec(1);
                        std::cout << vec.capacity() << '\n'
                        return 0;
                    }
                    ```
                    #### 以上代码输出： 4
                    函数返回描述：无

            ## max_size
            - 返回值类型：size_t 非静态成员函数名：size 参数列表：(void)
            - size_t max_size(void)
                - 用于获取当前最大存储大小
                    ``` C++
                    import fixed_vector;
                    import std;

                    int main() {
                        dast::fixed_vector<int, 5> vec;
                        std::cout << vec.max_size() << '\n'
                        return 0;
                    }
                    ```
                    #### 以上代码输出： 5
                    函数返回描述：无

            ## at
            - 返回值类型：reference_t 非静态成员函数名：at 参数列表：(size_t)
            - reference_t at(size_t size)
                - 用于获取指定位置的元素
                - 当position大于最大容量时，则抛出const char*类型的异常
                    ``` C++
                    import fixed_vector;
                    import std;

                    int main() {
                        dast::fixed_vector<int, 5> vec;
                        vec[0] = 1;
                        std::cout << vec.at(0) << '\n' // out: 1
                        vec.at(6); // error: out of range
                        return 0;
                    }
                    ```
                    #### 由于抛出异常，以上代码执行至vec.at(6)时终止
                    函数返回描述：无

            ## operator
            - operator[]
                - 返回值类型：reference_t 非静态运算符重载符：[] 参数列表：(size_t)
                - reference_t operator[](size_t size)
                    - 用于获取指定位置的元素
                        ``` C++
                        import fixed_vector;
                        import std;

                        int main() {
                            dast::fixed_vector<int, 5> vec;
                            std::cout << (vec[0] = 1, vec) << '\n'
                            return 0;
                        }
                        ```
                        #### 以上代码输出： 1
                        函数返回描述：无

[construct]: https://github.com/RockingHeart/Modern-Librarys/blob/main/Describes/FixedVector/ClassInterface-FixedVector.md#construct
[push_back]: https://github.com/RockingHeart/Modern-Librarys/blob/main/Describes/FixedVector/ClassInterface-FixedVector.md#push_back
[pop_back]: https://github.com/RockingHeart/Modern-Librarys/blob/main/Describes/FixedVector/ClassInterface-FixedVector.md#pop_back
[begin_and_end]: https://github.com/RockingHeart/Modern-Librarys/blob/main/Describes/FixedVector/ClassInterface-FixedVector.md#begin-and-end
[size]: https://github.com/RockingHeart/Modern-Librarys/blob/main/Describes/FixedVector/ClassInterface-FixedVector.md#size
[capacity]: https://github.com/RockingHeart/Modern-Librarys/blob/main/Describes/FixedVector/ClassInterface-FixedVector.md#capacity
[max_size]: https://github.com/RockingHeart/Modern-Librarys/blob/main/Describes/FixedVector/ClassInterface-FixedVector.md#max_size
[at]: https://github.com/RockingHeart/Modern-Librarys/blob/main/Describes/FixedVector/ClassInterface-FixedVector.md#at
[operator]: https://github.com/RockingHeart/Modern-Librarys/blob/main/Describes/FixedVector/ClassInterface-FixedVector.md#operator