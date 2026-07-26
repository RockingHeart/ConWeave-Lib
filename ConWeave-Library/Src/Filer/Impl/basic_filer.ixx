export module filer.basic;

import sys.filoader;
import sys.adapt_loader;
import sys.access;
import utility;

import std;
import <windows.h>;

using namespace sys;

template <
	class CharType,
	template <rest::character> class ObjectType
> concept reader_constr = requires (ObjectType<CharType> obj) {
	obj.data;
	obj.read;
};

template <
	class CharType,
	template <rest::character> class ObjectType
> concept writer_constr = requires (ObjectType<CharType> obj) {
	obj.write;
};

export template <
	rest::character CharType,
	template <rest::character> class FileLoader,
	template <rest::character> class Reader,
	template <rest::character> class Writer
> requires (reader_constr<CharType, Reader> &&
		    writer_constr<CharType, Writer>)
class basic_filer {
public:
	using char_t =		 CharType;
	using path_t = const CharType*;
	using text_t = const CharType*;

private:
	using loader_wap = FileLoader<CharType>;
	using reader_wap = Reader<CharType>;
	using writer_wap = Writer<CharType>;

private:

	loader_wap loader;
	reader_wap reader;
	writer_wap writer;

private:

	struct filer_com {
		loader_wap& loader;
		reader_wap& reader;
		writer_wap& writer;
	};

public:

	constexpr basic_filer()
		noexcept = default;

	constexpr basic_filer (path_t	  path,
						   permission permis = permission::read_only)
		noexcept : loader(path, permis)
	{
		reader.read(loader);
	}

	constexpr basic_filer(filoader<char_t>& filoader)
		noexcept : loader(filoader), reader(filoader)
	{}

public:

	constexpr auto data() const noexcept {
		return reader.data();
	}

	constexpr auto com() noexcept {
		return filer_com {
			loader, reader, writer
		};
	}
};

export enum class read_order : std::size_t {
	def, random
};

export template <rest::character CharType, read_order Order>
class basic_adapt_filer {
public:
	using char_t =		 CharType;
	using text_t =		 CharType*;
	using path_t = const CharType*;

public:
	using adapt_loader =		  adapt_loader<char_t>;
	using fileid_t     = typename adapt_loader::fileid_t;

protected:

	adapt_loader loader;
	text_t		 text;

public:

	constexpr size_t mapping_behavior() noexcept {
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

	constexpr text_t mapping_read() noexcept {
		return static_cast<text_t> (
			MapViewOfFile (
				loader.file(),
				mapping_behavior(),
				0, 0, 0
			)
		);
	}

	constexpr auto read_impl(text_t buffer) noexcept {
		unsigned long size = 0;
		fileid_t base_file = loader.base();
		bool result = ReadFile (
			base_file, buffer, 255, &size, nullptr
		);
		return result && size;
	}

	constexpr text_t copy_read() noexcept {
		text_t result = new char_t [
			loader.size() + 1
		];
		static char_t buffer[255] {};
		unsigned long tosi = 0;
		while (read_impl(buffer)) {
			std::memcpy (
				result + tosi,
				buffer, size
			);
			tosi += size;
		}
		result[tosi] = char_t();
		return result;
	}

	template <std::size_t size>
	constexpr std::size_t to_megabyte() const noexcept {
		return 2048 * size;
	}

	[[nodiscard]]
	constexpr bool can_mapping() const noexcept {
		if constexpr (Order == read_order::random) {
			return false;
		}
		if (loader.size() >= to_megabyte<100>()) {
			return true;
		}
		return false;
	}

	constexpr text_t read_text() noexcept {
		if (can_mapping()) {
			loader.mapping();
			return mapping_read();
		}
		return copy_read();
	}

public:

	constexpr basic_adapt_filer()
		noexcept : text(nullptr)
	{}

	constexpr basic_adapt_filer (path_t		path,
								 permission permis = permission::read_only)
		noexcept : loader(path, permis)
	{}

public:

	constexpr void read() noexcept {
		text = read_text();
	}

	constexpr auto data() const noexcept {
		return text;
	}

	constexpr auto size() const noexcept {
		return loader.size();
	}

	constexpr void release() noexcept {
		if (loader.mapped()) {
			UnmapViewOfFile(text);
			return;
		}
		delete[] text;
	}

public:

	constexpr ~basic_adapt_filer() noexcept {
		if (text == nullptr) {
			return;
		}
		release();
	}
};
