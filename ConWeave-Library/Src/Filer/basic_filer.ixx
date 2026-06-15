export module filer.basic;

import sys.filoader;
import sys.access;

import utility;
import <type_traits>;

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
> requires (
	reader_constr<CharType, Reader> &&
	writer_constr<CharType, Writer>
) class basic_filer;

template <
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

	constexpr basic_filer(path_t path, permission permis = permission::read_only)
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

	constexpr auto write(const text_t text) {
		if constexpr (requires{writer.write(loader, text); }) {
			return writer.write(loader, text);
		}
		else if constexpr (requires{ writer.write(loader, reader, text); }) {
			return writer.write(loader, reader, text);
		}
	}

public:

	constexpr ~basic_filer()
		noexcept = default;

};