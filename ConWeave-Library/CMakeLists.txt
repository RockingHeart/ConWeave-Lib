cmake_minimum_required(VERSION 3.31)

if (NOT MSVC)
    set(CMAKE_EXPERIMENTAL_CXX_MODULE_SCAN ON)
endif()

project(ConWeaveLib)

set(CMAKE_CXX_STANDARD 23)
set(ConWeaveLib_Path ${CMAKE_CURRENT_SOURCE_DIR})

if (MSVC)
    add_compile_options(/std:c++latest /std:clatest /experimental:module /permissive-)
else()
    add_compile_options(-std=c++23 -stdlib=libc++)
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
file(GLOB ComponentgPath "Src/Allocator/Component/*.ixx")

add_library(ConWeaveLib)

target_sources(ConWeaveLib
    PUBLIC
    FILE_SET mods TYPE CXX_MODULES
    BASE_DIRS ${ConWeaveLib_Path}/Export
    FILES ${ExportPath}
    BASE_DIRS ${ConWeaveLib_Path}/Src
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
        ${ComponentgPath}
)

target_include_directories(ConWeaveLib
    PUBLIC
    ${ConWeaveLib_Path}/Export
    ${ConWeaveLib_Path}/Src
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
source_group("Allocator\\Component" FILES ${ComponentgPath})

option(TEST "Build tests" OFF)

if(TEST)
    file(GLOB DebugSources "Debugs/*.cpp")
    add_executable(ConWeaveLib_test
        ${DebugSources}
    )
    target_link_libraries(ConWeaveLib_test PRIVATE ConWeaveLib)
    source_group("Debugs" FILES ${DebugSources})
endif()