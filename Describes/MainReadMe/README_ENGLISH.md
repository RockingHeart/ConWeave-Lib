# This project is Converge & Weave: a library of convergence and weaving, implemented using modern C++ (from C++17 onwards) syntax features.

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
| string | Implemented | Basic implementation effective before November 2025 | 1.16.1 |
| fixed_vector | Implemented | Basics finalized 2026.5.22 | 1.6.0 |
| vector | Implemented | Basics finalized 2026.5.22 | 3.0.1 |
| filer | Implemented | 2026.5.27 | 1.0.2 |
| block_allocator | Implemented | 2026.6.21 | 1.5.1 |

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
### Q: Regarding String, why is the interface named `core`, with `basic` as the core implementation, and why does `basic` implement constructors, destructors, and operators?
### A: This String uses CRTP with explicit `this` to achieve separation and independent implementation of interface and implementation. In the CRTP interface class, the internal interface implementation actually uses explicit `this` to call the parent class's implementation. If the parent class's implementation needs to pass in its own type, then `this` needs to be cast within the CRTP interface, which introduces runtime conversion overhead, defeating the purpose. Combined with the philosophy that the foundation is implemented by me while the core may not necessarily be, this is a choice I made after careful consideration.

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