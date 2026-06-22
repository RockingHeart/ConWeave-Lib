export module sys.adapt_loader;

import utility;
import sys.filoader;
import sys.access;

import <windows.h>;
import std;

export namespace sys
{
	template <rest::character>
	class adapt_loader;
};

template <rest::character CharType>
class sys::adapt_loader {
public:
	using char_t   =	   CharType;
	using path_t   = const CharType*;

private:
	using fileloader = filoader<char_t>;
	
public:
	using fileid_t = typename fileloader::fileid_t;

private:

	fileloader  loader;
	fileid_t	mapping_file;

public:

	constexpr adapt_loader()
		noexcept = default;

	constexpr adapt_loader(path_t	  path,
						   permission per = permission::read_only)
		noexcept : loader(path, per), mapping_file(nullptr)
	{}

public:

	template <comaccess ComAccess>
	[[nodiscard]] constexpr bool has() const noexcept {
		return loader.template has<ComAccess>();
	}

	constexpr void mapping() noexcept {
		mapping_file = loader.mapping();
	}

	constexpr bool mapped() const noexcept {
		return mapping_file != nullptr;
	}

	constexpr fileid_t file() noexcept {
		return mapping_file;
	}

	constexpr fileid_t base() const noexcept {
		return loader.id();
	}

	constexpr std::size_t size() const noexcept {
		return loader.size().value();
	}

	constexpr bool resize(std::size_t size) noexcept {
		LARGE_INTEGER file_info {};
		file_info.QuadPart = size;

		fileid_t file = loader.id();

		bool success = SetFilePointerEx (
			file, file_info, nullptr, 0
		);

		return success && SetEndOfFile(file);
	}

	constexpr void unmapping() noexcept {
		CloseHandle(mapping_file);
		mapping_file = nullptr;
	}

	[[nodiscard]]
	static constexpr bool can_open(path_t path) noexcept {
		return fileloader::can_open(path);
	}

public:

	constexpr ~adapt_loader() noexcept {
		if (!mapping_file) {
			return;
		}
		unmapping();
	}
};