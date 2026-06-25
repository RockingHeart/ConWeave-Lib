# 此为介绍BlockAllocator类与其接口的文档

## 定长向量接口快照表

| Interfaces | Shoots |
| :---: | :---: |
| construct | [construct] |
| allocate | [allocate] |
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
            - 模板类型<class AllocType>
            - 返回值类型：AllocType*
                - 参数列表：(AllocType&&)
                - allocate(AllocType&&)
                    - 用于申请一块大小为AllocType长度的内存
                    - 以移动构造函数构造
                - 参数列表：(AllocType&)
                - allocate(AllocType&)
                    - 用于申请一块大小为AllocType长度的内存
                    - 以拷贝构造函数构造
            - <rest::character CharType>
            - 返回值类型：CharType*
                - 参数列表：(CharType*, std::size_t)
                - allocate(CharType*, std::size_t size)
                    - 用于申请以size为大小的字符串空间
                    
            ## begin
            - 注意：此为序列化表示，并非为字符串表示
            - 返回值类型：char*
                - 参数列表：begin(void)
                - begin(void)
                    - 用于返回块起始指针

            ## end
            - 注意：此为序列化表示，并非为字符串表示
            - 返回值类型：char*
                - 参数列表：end(void)
                - end(void)
                    - 用于返回块末尾指针




[construct]: https://github.com/RockingHeart/Modern-Librarys/blob/main/Describes/Allocator/BlockAllocator/ClassInterface-BlockAllocator.md#construct
[allocate]: https://github.com/RockingHeart/Modern-Librarys/blob/main/Describes/Allocator/BlockAllocator/ClassInterface-BlockAllocator.md#allocate
[begin]: https://github.com/RockingHeart/Modern-Librarys/blob/main/Describes/Allocator/BlockAllocator/ClassInterface-BlockAllocator.md#begin
[end]: https://github.com/RockingHeart/Modern-Librarys/blob/main/Describes/Allocator/BlockAllocator/ClassInterface-BlockAllocator.md#end