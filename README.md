# 此项目为Converge & Weave：汇智与编织的库，利用现代C++（C++17起包含之前）语法特性实现。

## 正在实现String

* 快速了解这个项目
    - 项目源码：[Source]
    - 项目规范：[DevStandard]
    - 项目名间：[Namespace]
    - 项目欢庆：[Rejoice]
    - 项目能报：[PerformanceReport]
    - String
        - 接文：[ClassInterface-String]
        - 未来：[Future-String]
        - 更新：[UpdateLog-String]
        - 文介：[ImplationFile-String]
        - 基础：[Base-String]
    - FixedVector
        - 接文：[ClassInterface-FixedVector]
        - 未来：[Future-FixedVector]
        - 更新：[UpdateLog-FixedVector]
        - 文介：[ImplationFile-FixedVector]
    - Vector
        - 接文：[ClassInterface-Vector]
        - 未来：[Future-Vector]
        - 更新：[UpdateLog-Vector]
        - 文介：[ImplationFile-Vector]
        - 缺报：[DefectReport-Vector]
    - Allocator
        - 文档：[Describes-Allocator]
    - Utility
        - 文介：[ImplationFile-Utility]

## 状态表
> *状态随时更新*

| 项目实现 | 实现状态 | 实现时间 | 版本号 |
| :---: | :---: | :---: | :---: |
| string | 已实现 | 实现基础有效时间为2025年11月份之前 | 1.17.0 |
| fixed_vector | 已实现 | 2026.5.22完善基础 | 1.6.1 |
| vector | 已实现 | 2026.5.22完善基础 | 3.0.2 |
| filer | 已实现 | 2026.5.27 | 1.0.2 |
| block_allocator | 已实现 | 2026.6.21 | 1.7.0 |
| symbol_table | 已实现 | 2026.7.25 | 2.0.0 |

## 性能评估
- string
    请查阅：[PerformanceReport-String]

<br>

> [!TIP]
> String接口调用采用C++23的显式This与模板可变参数
>
> 若不知晓接口如何使用，请阅读指定文档

<dl>
    <dt>关于string在no_residue值特性下的特性</dt>
</dl>

- [x] 64位占用大小为24字节（MSVC，char类型，且allocator无状态）
- [x] 64位缓存大小为23字节（MSVC，char类型，且allocator无状态）


<dl>
    <dt>关于string在remain值特性下的特性</dt>
</dl>

- [x] 双重数据
- [x] 64位占用大小为48字节（MSVC，char类型，且allocator无状态）
- [x] 64位缓存大小为47字节（MSVC，char类型，且allocator无状态）

<dl>
    <dt>关于fixed_vector的特性</dt>
</dl>

- [x] 若为基础类型，则支持编译期

<dl>
    <dt>关于vector的特性</dt>
</dl>

- [x] 缓冲区为0时无缓冲区
- [x] 缓冲区太小时内部设定缓冲区
- [x] 缓冲区若非过小则依用户设定

## CMake使用教程
[*下载*][Download]此项目至本地 <br>
! 目前暂不支持跨平台
**若未安装CMake，请安装[CMake](https://cmake.org/)，可在控制台输入CMake -V查看版本，侧面知晓CMake是否安装** <br>
**在项目当前目录下使用控制台输入指令：CMake -B "/ML"，此指令用于指定生成目录，而-B命令参数之后的值是指定的目录，值类型为字符串** <br>

## Q&A
### 关于代码：(str1 + str2).const_string
### Q: 为什么operator+的返回类型是basic_string？
```C++
// 此为operator+的接口
template <class... ArgsType>
constexpr basic_string operator+(this basic_string& self, ArgsType&&... args)
	noexcept requires (
		requires {
			basic_string { self, std::forward<ArgsType>(args)... };
		}
	)
{
	return { self, std::forward<ArgsType>(args)... };
}
```
``` text
调用链：
    Caller(BasicString::Operator+) -> Return(BasicString)
    Caller(BasicCore::ConstString) -> FromExplicitThis(CallerOfBasicString) -> Inheritance(StringCore)
    Finally MemFun(BasicCore::ConstString) -> FromCaller(BasicString::Pointer)
    ConstStringOfImpl: Call(ExplicitThis) -> MemFun(ExplicitThis::Pointer)
```
### A：如果返回string_core，因显式this类型要求是basic_string以调用pointer（若是string_core，显式this是string_core，不满足），故而查找不到const_string <br> 若返回basic_string，则满足调用链要求。

[Download]: https://github.com/RockingHeart/ConWeave-Lib/archive/refs/heads/main.zip

[Source]: https://github.com/RockingHeart/ConWeave-Lib/tree/main/ConWeave-Library/Src
[DevStandard]: https://github.com/RockingHeart/ConWeave-Lib/blob/main/Describes/DevStandard.md
[Namespace]: https://github.com/RockingHeart/ConWeave-Lib/blob/main/Describes/Namespace.md
[Rejoice]: https://github.com/RockingHeart/ConWeave-Lib/blob/main/Describes/Rejoice.md
[PerformanceReport]: https://github.com/RockingHeart/ConWeave-Lib/tree/main/PerformanceReport
[PerformanceReport-String]: https://github.com/RockingHeart/ConWeave-Lib/tree/main/PerformanceReport/String

<!-- String -->
[ClassInterface-String]: https://github.com/RockingHeart/ConWeave-Lib/blob/main/Describes/String/ClassInterface-String.md
[Future-String]: https://github.com/RockingHeart/ConWeave-Lib/blob/main/Describes/String/Future-String.md
[UpdateLog-String]: https://github.com/RockingHeart/ConWeave-Lib/blob/main/Describes/String/UpdateLog-String.md
[ImplationFile-String]: https://github.com/RockingHeart/ConWeave-Lib/blob/main/Describes/String/ImplationFile-String.md
[Base-String]: https://github.com/RockingHeart/ConWeave-Lib/blob/main/Describes/String/Base-String.md

<!-- FixedVector -->
[ClassInterface-FixedVector]: https://github.com/RockingHeart/ConWeave-Lib/blob/main/Describes/FixedVector/ClassInterface-FixedVector.md
[Future-FixedVector]: https://github.com/RockingHeart/ConWeave-Lib/blob/main/Describes/FixedVector/Future-FixedVector.md
[UpdateLog-FixedVector]: https://github.com/RockingHeart/ConWeave-Lib/blob/main/Describes/FixedVector/UpdateLog-FixedVector.md
[ImplationFile-FixedVector]: https://github.com/RockingHeart/ConWeave-Lib/blob/main/Describes/FixedVector/ImplationFile-FixedVector.md

<!-- Vector -->
[ClassInterface-Vector]: https://github.com/RockingHeart/ConWeave-Lib/blob/main/Describes/Vector/ClassInterface-Vector.md
[Future-Vector]: https://github.com/RockingHeart/ConWeave-Lib/blob/main/Describes/Vector/Future-Vector.md
[UpdateLog-Vector]: https://github.com/RockingHeart/ConWeave-Lib/blob/main/Describes/Vector/UpdateLog-Vector.md
[ImplationFile-Vector]: https://github.com/RockingHeart/ConWeave-Lib/blob/main/Describes/Vector/ImplationFile-Vector.md
[DefectReport-Vector]: https://github.com/RockingHeart/ConWeave-Lib/blob/main/Describes/Vector/DefectReport-Vector.md

<!-- Allocator -->
[Describes-Allocator]: https://github.com/RockingHeart/ConWeave-Lib/tree/main/Describes/Allocator

<!-- Utility -->
[ImplationFile-Utility]: https://github.com/RockingHeart/ConWeave-Lib/blob/main/Describes/Utility/ImplationFile.md