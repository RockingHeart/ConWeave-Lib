cmake_minimum_required(VERSION 3.31)

project(CoaWeaveLib LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CoaWeaveLib_Path ${CMAKE_CURRENT_SOURCE_DIR})

if (MSVC)
    add_compile_options(/std:c++latest /std:clatest /experimental:module /permissive-)
endif()

file(GLOB ExportPath "Export/*.ixx")
file(GLOB StringPath "Src/String/*.ixx")
file(GLOB StringImplPath "Src/String/Impl/*.ixx")
file(GLOB FixedVectorPath "Src/FixedVector/*.ixx")
file(GLOB FixedVectorImplPath "Src/FixedVector/Impl/*.ixx")
file(GLOB VectorPath "Src/Vector/*.ixx")
file(GLOB VectorImplPath "Src/Vector/Impl/*.ixx")
file(GLOB FilerPath "Src/Filer/*.ixx")
file(GLOB FilerImplPath "Src/Filer/Impl/*.ixx")
file(GLOB SymbolTablePath "Src/SymbolTable/*.ixx")
file(GLOB SymbolTableImplPath "Src/SymbolTable/Impl/*.ixx")
file(GLOB WrapPath "Src/Wrap/*.ixx")
file(GLOB UtilityPath "Src/Utility/*.ixx")
file(GLOB AllocatorPath "Src/Allocator/*.ixx")
file(GLOB BlockAllocatorPath "Src/Allocator/BlockAllocator/*.ixx")
file(GLOB AllocatorComponentgPath "Src/Allocator/Component/*.ixx")

add_library(CoaWeaveLib)

target_sources(CoaWeaveLib
    PUBLIC
    FILE_SET mods TYPE CXX_MODULES
    BASE_DIRS ${CoaWeaveLib_Path}/Export
    FILES ${ExportPath}
    BASE_DIRS ${CoaWeaveLib_Path}/Src
    FILES
        ${UtilityPath}
        ${WrapPath}
        ${StringPath} ${StringImplPath}
        ${FixedVectorPath} ${FixedVectorImplPath}
        ${VectorPath} ${VectorImplPath}
        ${FilerPath} ${FilerImplPath}
        ${SymbolTablePath} ${SymbolTableImplPath}
        ${AllocatorPath}
        ${BlockAllocatorPath}
        ${AllocatorComponentgPath}
)

target_include_directories(CoaWeaveLib
    PUBLIC
    ${CoaWeaveLib_Path}/Export
)

source_group("Export" FILES ${ExportPath})
source_group("String" FILES ${StringPath})
source_group("String\\Impl" FILES ${StringImplPath})
source_group("FixedVector" FILES ${FixedVectorPath})
source_group("FixedVector\\Impl" FILES ${FixedVectorImplPath})
source_group("Vector" FILES ${VectorPath})
source_group("Vector\\Impl" FILES ${VectorImplPath})
source_group("Filer" FILES ${FilerPath})
source_group("Filer\\Impl" FILES ${FilerImplPath})
source_group("SymbolTable" FILES ${SymbolTablePath})
source_group("SymbolTable\\Impl" FILES ${SymbolTableImplPath})
source_group("Wrap" FILES ${WrapPath})
source_group("Utility" FILES ${UtilityPath})
source_group("Allocator" FILES ${AllocatorPath})
source_group("Allocator\\BlockAllocator" FILES ${BlockAllocatorPath})
source_group("Allocator\\Component" FILES ${AllocatorComponentgPath})

option(TEST "Build tests" OFF)

if(TEST)
    file(GLOB DebugSources "Debugs/*.cpp")
    add_executable(CoaWeaveLib_test
        ${DebugSources}
    )
    target_link_libraries(CoaWeaveLib_test PRIVATE CoaWeaveLib)
    source_group("Debugs" FILES ${DebugSources})
endif()