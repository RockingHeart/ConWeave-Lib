# 此为介绍BlockAllocator类与其接口的文档

## 定长向量接口快照表

| Interfaces | Shoots |
| :---: | :---: |
| construct | [construct] |
| allocate | [allocate] |

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
                    - 用于构造size为页数的页内存块 <br>
            
            ## allocate<AllocType>
            - 返回值类型：AllocType*
                - 参数列表：(AllocType&&)
                - allocate(AllocType&&)
                    - 用于申请一块大小为AllocType长度的内存
                    - 以移动构造函数构造
                - 参数列表：(AllocType&)
                - allocate(AllocType&)
                    - 用于申请一块大小为AllocType长度的内存
                    - 以拷贝构造函数构造

[construct]: https://github.com/RockingHeart/Modern-Librarys/blob/main/Describes/Allocator/BlockAllocator/ClassInterface-BlockAllocator.md#construct
[allocate]: https://github.com/RockingHeart/Modern-Librarys/blob/main/Describes/Allocator/BlockAllocator/ClassInterface-BlockAllocator.md#allocate