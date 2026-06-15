module;
#include <windows.h>

export module sys.adapt_reader;

import sys.adapt_loader;
import sys.access;

import utility;
import dast.string;

import <type_traits>;
import <cstddef>;
import <memory>;

export namespace sys
{
	template <rest::character CharType>
	class adapt_reader;
}

template <rest::character CharType>
class sys::adapt_reader {
public:
	using char_t = CharType;
	using text_t = CharType*;

private:
	using loader   =		  adapt_loader<char_t>;
	using fileid_t = typename loader::fileid_t;

private:

	text_t text;

public:

	static constexpr size_t mapping_behavior(loader& loader) noexcept {
		if (loader.template has<comaccess::read>()) {
			size_t result = 0x0004;
			if (loader.template has<comaccess::write>()) {
				return result | 0x0002;
			}
			return result;
		}
		return 0xF001F;
	}

private:

	constexpr text_t mapping_read(loader& loader) noexcept {
		if (!loader.mapped()) {
			loader.mapping();
		}
		return static_cast<text_t> (
			MapViewOfFile(loader.file(), mapping_behavior(loader), 0, 0, 0)
		);
	}

	constexpr text_t copy_read(loader& loader) noexcept {
		text_t result = new char_t[loader.size() + 1];
		static char_t buffer[255] {};
		fileid_t base_file = loader.base();
		unsigned long size = 0;
		unsigned long tosi = 0;
		auto read		   = [&]() {
			bool result = ReadFile (
				base_file, buffer, 255, &size, NULL
			);
			return result && size;
		};
		while (read()) {
			std::memcpy (
				result + tosi,
				buffer, size
			);
			tosi += size;
		}
		result[tosi] = char_t();
		return result;
	}

	constexpr text_t read_text(loader& loader) noexcept {
		if (loader.size() >= 2048 * 100) {
			return mapping_read(loader);
		}
		return copy_read(loader);
	}

public:

	constexpr adapt_reader()
		noexcept : text()
	{}

	constexpr adapt_reader(loader& loader)
		noexcept : text(read(loader))
	{}

public:

	constexpr text_t read(loader& loader) noexcept {
		return read_text(loader);
	}

	constexpr auto data() const noexcept {
		return text;
	}

	constexpr void release_copy() noexcept {
		delete[] text;
		text = nullptr;
	}
	
	constexpr void release_mapping() noexcept {
		UnmapViewOfFile(text);
	}
};
