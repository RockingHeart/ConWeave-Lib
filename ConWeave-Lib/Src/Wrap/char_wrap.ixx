export module char_wrap;

import utility;

export namespace wrap {
	template <rest::character CharType, size_t Size>
	struct char_wrap;
}

template <rest::character CharType, size_t Size>
struct wrap::char_wrap {
	using char_t = CharType;
	char_t data[Size];
	consteval char_wrap(const CharType(&str)[Size]) {
		for (size_t i = 0; i < Size; ++i)
			data[i] = str[i];
	}
	constexpr static size_t size = Size;
};