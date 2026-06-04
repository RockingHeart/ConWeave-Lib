export module sys.writer;

import sys.filoader;
import sys.access;

import utility;

import <windows.h>;
import <cstddef>;

export namespace sys
{
	template <rest::character CharType>
	class writer;
}

template <rest::character CharType>
class sys::writer {
public:
	using char_t = CharType;
	using text_t = const CharType*;

private:
	
	using loader = filoader<char_t>;
	
public:

	constexpr writer()
		noexcept = default;

public:

	static constexpr bool write(loader& loader, text_t text) noexcept {
		if (!loader.template has<comaccess::write>()) {
			return false;
		}
		std::size_t size    = text.size();
		unsigned long bytes = 0;
		bool write_result   = WriteFile (
			loader.id(), text.data(),
			size, nullptr,
			nullptr
		);
		return write_result || bytes == size;
	}
};
