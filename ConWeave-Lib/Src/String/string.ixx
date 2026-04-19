export module string;

import string_traits;
import string_core;
import basic_string;

import utility;

import <xmemory>;

export using ::string_mode;
export using ::string_info;

template <rest::character CharType>
using primary_traits = traits::string_traits <
	CharType, traits::string_value_traits::no_residue,
	strutil, std::allocator<CharType>
>;

export namespace dast {
	template <rest::character CharType, template<class, class> class StringCore = string_core>
	using string = basic_string<::primary_traits<CharType>, StringCore>;

	using cstring = basic_string<::primary_traits<char>,    string_core>;
	using wstring = basic_string<::primary_traits<wchar_t>, string_core>;
}

template <rest::character CharType, template<class, class> class StringCore = string_core>
basic_string(const CharType*) -> basic_string<::primary_traits<CharType>, StringCore>;

export constexpr dast::cstring operator ""_cs(const char* str, std::size_t size) {
	return { str, size };
}

export constexpr dast::wstring operator ""_ws(const wchar_t* str, std::size_t size) {
	return { str, size };
}