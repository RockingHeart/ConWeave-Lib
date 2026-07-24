module;
#include <windows.h>

export module sys.error_info;

import utility;
import dast.string;

import std;

export namespace sys
{
    template <rest::character CharType>
    dast::string<CharType> detailed_error(unsigned long error) noexcept;
}

template <rest::character CharType = char>
dast::string<CharType> sys::detailed_error(unsigned long error) noexcept {
    dast::string<CharType> result;
    CharType* message = nullptr;

    if constexpr (std::is_same_v<CharType, char>) {
        FormatMessageA (
            FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
            nullptr, error,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            reinterpret_cast<char*>(&message),
            0, nullptr
        );
        if (message) {
            result = message;
            LocalFree(message);
        }
    }
    else {
        FormatMessageW (
            FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
            nullptr, error,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            reinterpret_cast<wchar_t*>(&message),
            0, nullptr
        );
        if (message) {
            int len = WideCharToMultiByte (
                CP_UTF8, 0, message, -1,
                nullptr, 0, nullptr, nullptr
            );
            if (len > 0) {
                result.resize(len - 1);
                WideCharToMultiByte (
                    CP_UTF8, 0, message, -1,
                    &result[0], len, nullptr, nullptr
                );
            }
            LocalFree(message);
        }
    }

    return result;
}