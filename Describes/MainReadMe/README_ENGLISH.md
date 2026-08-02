# This project is Converge & Weave: It's a library, implemented using modern C++ (from C++17 onwards) syntax features.

English：[EnglishMD] <br>

## String Implementation in Progress

* Quick overview of the project
    - Project source code: [Source]
    - Project standards: [DevStandard]
    - Project namespace: [Namespace]
    - Project highlights: [Rejoice]
    - Performance report: [PerformanceReport]
    - String
        - Interface: [ClassInterface-String]
        - Future plans: [Future-String]
        - Updates: [UpdateLog-String]
        - Implementation files: [ImplationFile-String]
        - Basics: [Base-String]
    - FixedVector
        - Interface: [ClassInterface-FixedVector]
        - Future plans: [Future-FixedVector]
        - Updates: [UpdateLog-FixedVector]
        - Implementation files: [ImplationFile-FixedVector]
    - Vector
        - Interface: [ClassInterface-Vector]
        - Future plans: [Future-Vector]
        - Updates: [UpdateLog-Vector]
        - Implementation files: [ImplationFile-Vector]
        - Defect report: [DefectReport-Vector]
    - Allocator
        - Document: [Describes-Allocator]
    - Utility
        - Implementation files: [ImplationFile-Utility]

## Status Table
> *Status updates at any time*

| Implementation | Status | Completion Time | Version |
| :---: | :---: | :---: | :---: |
| string | Implemented | Basic implementation effective before November 2025 | 1.17.0 |
| fixed_vector | Implemented | Basics finalized 2026.5.22 | 1.6.1 |
| vector | Implemented | Basics finalized 2026.5.22 | 3.0.2 |
| filer | Implemented | 2026.5.27 | 1.0.2 |
| block_allocator | Implemented | 2026.6.21 | 1.7.0 |
| symbol_table | Implemented | 2026.7.25 | 2.0.0 |

## Performance Evaluation
- string
    Please refer to: [PerformanceReport-String]

<br>

> [!TIP]
> The String interface uses C++23's explicit `this` and template variadic parameters
>
> If you're unsure how to use the interface, please read the designated documentation

<dl>
    <dt>About string's characteristics under the no_residue value feature</dt>
</dl>

- [x] 64-bit occupies 24 bytes (MSVC, char type, with stateless allocator)
- [x] 64-bit buffer size is 23 bytes (MSVC, char type, with stateless allocator)


<dl>
    <dt>About string's characteristics under the remain value feature</dt>
</dl>

- [x] Dual data
- [x] 64-bit occupies 48 bytes (MSVC, char type, with stateless allocator)
- [x] 64-bit buffer size is 47 bytes (MSVC, char type, with stateless allocator)

<dl>
    <dt>About fixed_vector's characteristics</dt>
</dl>

- [x] If the underlying type is fundamental, compile-time support is available

<dl>
    <dt>About vector's characteristics</dt>
</dl>

- [x] No buffer when buffer size is 0
- [x] Internal buffer set when buffer is too small
- [x] When buffer isn't too small, user settings apply

## CMake Usage Guide
[*Download*][Download] this project locally <br>
! Cross-platform support is not currently available
**If CMake is not installed, please install [CMake](https://cmake.org/). You can check by typing `CMake -V` in the console to see the version and confirm whether CMake is installed** <br>
**In the project's current directory, enter the command in the console: `CMake -B "/CML"`. This command specifies the output directory — the value after the -B parameter is the designated directory, and the value type is a string** <br>

## Q&A
### About the code: (str1 + str2).const_string
### Q: Why is the return type of operator+ basic_string?
```C++
// This is the interface for operator+
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
Call chain:
    Caller(BasicString::Operator+) -> Return(BasicString)
    Caller(BasicCore::ConstString) -> FromExplicitThis(CallerOfBasicString) -> Inheritance(StringCore)
    Finally MemFun(BasicCore::ConstString) -> FromCaller(BasicString::Pointer)
    ConstStringOfImpl: Call(ExplicitThis) -> MemFun(ExplicitThis::Pointer)
```
### A: If it returns string_core, the explicit this type would be string_core. Since pointer() needs to be called from the explicit this inside const_string, and string_core has no pointer(), the call fails. <br> If it returns basic_string, the call chain requirement is satisfied.

[Download]: https://github.com/RockingHeart/ConWeave-Lib/archive/refs/heads/main.zip

[EnglishMD]: https://github.com/RockingHeart/ConWeave-Lib/blob/main/Describes/MainReadMe/README_ENGLISH.md

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