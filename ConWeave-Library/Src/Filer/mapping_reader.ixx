export module sys.mapping_reader;

import sys.filoader;
import sys.access;

import utility;
import dast.string;

import <type_traits>;
import <cstddef>;
import <memory>;
import <windows.h>;

export namespace sys
{
	template <rest::character CharType>
	class mapping_reader;
}

template <rest::character CharType>
class sys::mapping_reader {
public:
	using char_t = CharType;
	using text_t = CharType*;

private:
	using loader   =		  filoader<char_t>;
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

	constexpr void read_text(loader& loader) noexcept {
		text = static_cast<text_t> (
			MapViewOfFile(loader.id(), mapping_behavior(loader), 0, 0, 0)
		);
	}

public:

	constexpr mapping_reader()
		noexcept : text()
	{}

public:

	constexpr bool read(loader& loader) noexcept {
		if (!loader.is_loaded() || text) {
			return false;
		}
		fileid_t old = loader.mapping();
		CloseHandle(old);
		read_text(loader);
		return true;
	}

	constexpr auto data() const noexcept {
		return text;
	}

	constexpr dast::string<char_t> copy() const noexcept {
		return text;
	}

public:

	constexpr ~mapping_reader() noexcept {
		if (!text) {
			return;
		}
		UnmapViewOfFile(text);
	}

};
