export module sys.mapping_writer;

import sys.filoader;
import sys.access;

import utility;

import <windows.h>;
import <cstddef>;
import <memory>;
import <string>;

export namespace sys
{
	template <rest::character CharType>
	class mapping_writer;
}

template <rest::character CharType>
class sys::mapping_writer {
public:
	using char_t = CharType;
	using text_t = CharType*;

private:
	using loader = filoader<char_t>;

public:

	constexpr mapping_writer()
		noexcept = default;

public:

	template <class Reader, class ContextType>
	static constexpr bool write(loader&		 loader,
								Reader&		 reader,
								ContextType  text)
		noexcept
	{
		if (!loader.template has<comaccess::write>()) {
			return false;
		}
		text_t pointer   = reader.data();
		std::size_t size = std::strlen(text);
		std::memcpy (
			pointer,
			text, size
		);
		return FlushViewOfFile(pointer, size);
	}

public:

	constexpr ~mapping_writer() noexcept {
	}
};
