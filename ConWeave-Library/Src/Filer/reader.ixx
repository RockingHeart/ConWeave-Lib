export module sys.reader;

import sys.filoader;

import utility;
import dast.string;

import <type_traits>;
import <cstddef>;
import <windows.h>;

export namespace sys
{
	template <rest::character CharType>
	class reader;
}

template <rest::character CharType>
class sys::reader{
public:
	using char_t = CharType;
	using text_t = dast::string<char_t>;

private:
	using loader   =		  filoader<char_t>;
	using fileid_t = typename loader::fileid_t;

private:

	text_t text;

private:

	constexpr void read_text(fileid_t id) noexcept {
		char_t		  buffer[256];
		unsigned long bytes = 0;
		while (ReadFile(id, buffer, 256, &bytes, 0) && bytes) {
			text.append(buffer, bytes);
		}
	}

public:

	constexpr reader()
		noexcept = default;

	constexpr reader(loader& loader) noexcept
	{
		read(loader);
	}

public:

	constexpr bool read(loader& loader) noexcept {
		if (!loader.is_loaded()) {
			return false;
		}
		if (text) {
			text.resize(0);
		}
		read_text(loader.id());
		return true;
	}

	constexpr auto data() const noexcept {
		return text.data();
	}

	constexpr text_t copy() const noexcept {
		return text;
	}

};
