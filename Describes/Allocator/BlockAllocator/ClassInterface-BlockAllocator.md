# 此为介绍BlockAllocator类与其接口的文档

## 定长向量接口快照表

| Interfaces | Shoots |
| :---: | :---: |
| construct | [construct] |
| allocate | [allocate] |
| try allocate | [try_allocate] |
| begin | [begin] |
| end | [end] |

- BlockAllocator

    - 作用：申请线性内存并以块的形式管理
        - 以下为接口：
            > 页的大小由操作系统给出
            ## construct
            - 返回值类型：block_allocator 构造函数名：block_allocator
                - 参数列表：(void)
                - block_allocator(void)
                    - 用于构造一页内存块（为降低TLB Miss的机率，则以页对齐） <br>
                - 参数列表：(size_t)
                - block_allocator(size_t size)
                    - 用于构造size为页大小的页内存块 <br>
            
            ## allocate
            - 模板类型<class AllocType, class... ArgsType>
            - 返回值类型：AllocType*
                - 参数列表：(AllocType&&, ArgsType&&...)
                - allocate(AllocType&&, ArgsType&&... args)
                    - 此函数会自动申请新空间
                    - 核心
                        - 用于申请一块大小为AllocType长度的内存
                        - 以参数为转发的args调用移动构造函数进行place new构造
                    - 该函数强调的返回值：无

            - <rest::character CharType>
            - 返回值类型：CharType*
                - 参数列表：(CharType*, std::size_t)
                - allocate(CharType*, std::size_t size)
                    - 此函数会自动申请新空间
                    - 核心
                        - 用于申请以size为大小的字符串空间
                    - 该函数强调的返回值：无

            ## try_allocate
            - 此函数拥有与[allocate]的相同参数版本与核心行为
            - 该函数强调的返回值：不会申请新空间，若空间不足则返回false
                    
            ## begin
            - 注意：此为序列化表示，并非为字符串表示
            - 返回值类型：char*
                - 参数列表：begin(void)
                - begin(void)
                    - 用于返回块起始指针
                - 该函数强调的返回值：无

            ## end
            - 注意：此为序列化表示，并非为字符串表示
            - 返回值类型：char*
                - 参数列表：end(void)
                - end(void)
                    - 用于返回块末尾指针
                - 该函数强调的返回值：无




[construct]: https://github.com/RockingHeart/Modern-Librarys/blob/main/Describes/Allocator/BlockAllocator/ClassInterface-BlockAllocator.md#construct
[allocate]: https://github.com/RockingHeart/Modern-Librarys/blob/main/Describes/Allocator/BlockAllocator/ClassInterface-BlockAllocator.md#allocate
[try_allocate]: https://github.com/RockingHeart/Modern-Librarys/blob/main/Describes/Allocator/BlockAllocator/ClassInterface-BlockAllocator.md#try_allocate
[begin]: https://github.com/RockingHeart/Modern-Librarys/blob/main/Describes/Allocator/BlockAllocator/ClassInterface-BlockAllocator.md#begin
[end]: https://github.com/RockingHeart/Modern-Librarys/blob/main/Describes/Allocator/BlockAllocator/ClassInterface-BlockAllocator.md#end