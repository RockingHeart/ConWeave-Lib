module;
#include <windows.h>;

export module sys.filoader;

import sys.access;
import sys.error_sentinel;

import <cstddef>;
import <type_traits>;

import utility;

constexpr std::size_t generic(std::size_t mark) noexcept {
	size_t access = 0;
	if (mark & comaccess::read)	   access |= 0x80000000;
	if (mark & comaccess::write)   access |= 0x40000000;
	if (mark & comaccess::execute) access |= 0x20000000;
	if (access == 0) {
		return 0x10000000;
	}
	return access;
}

constexpr std::size_t share(std::size_t mark) noexcept {
	size_t access = 0;
	if (mark & comaccess::read_share)   access |= 0x00000001;
	if (mark & comaccess::write_share)  access |= 0x00000002;
	if (mark & comaccess::delete_share) access |= 0x00000004;
	return access; // Return comaccess::exclusive if access is 0
}

constexpr std::size_t create(std::size_t mark) noexcept {
	if (mark & comaccess::create) return 2;
	if (mark & comaccess::make)   return 1;
	if (mark & comaccess::clear)  return 5;
	return 3;
}

export namespace sys
{
	template <rest::character CharType>
	class filoader;
}

template <rest::character CharType>
class sys::filoader {
public:
	using char_t   = CharType;
	using path_t   = const CharType*;
	using fileid_t = HANDLE;

private:

	fileid_t   fileid;
	permission permis;

private:

	constexpr fileid_t open_file(path_t path, const permission& permiss) {
		const std::size_t per = static_cast<std::size_t>(permiss);
		if constexpr (std::is_same_v<char_t, char>) {
			return CreateFileA (
				path, generic(per), share(per),
				nullptr, create(per), 0, nullptr
			);
		}
		else {
			return CreateFileW (
				path, generic(per), share(per),
				nullptr, create(per), 0, nullptr
			);
		}
	}

	constexpr std::size_t mapping_access() const noexcept {
		if (permis & comaccess::read) {
			if (permis & comaccess::write) {
				return 0x04;
			}
			return 0x02;
		}
		return 0x01;
	}

public:

	constexpr filoader (path_t	   path,
						permission per = permission::read_only)
		noexcept : fileid(open_file(path, per)), permis(per)
	{
		error_sentinel<error_type::open_file> sentinel{};
	}

	constexpr filoader(const filoader& loader)
		noexcept : fileid(loader.fileid), permis(loader.permis)
	{}

	constexpr filoader(filoader&& loader)
		noexcept : fileid(loader.fileid), permis(loader.permis)
	{
		loader.fileid = nullptr;
		loader.permis = 0;
	}

public:

	constexpr bool load (path_t		path,
				  const permission& per = permission::read_only)
		noexcept
	{
		if (open_file(path, per); !is_loaded()) {
			return false;
		}
		return true;
	}

	constexpr fileid_t mapping(const char_t* name = nullptr) noexcept {
		fileid_t old = fileid;
		if constexpr (std::is_same_v<char_t, char>) {
			fileid = CreateFileMappingA (
				old, nullptr,
				mapping_access(),
				0, 0, name
			);
		}
		else {
			fileid = CreateFileMappingW (
				old, nullptr,
				mapping_access(),
				0, 0, name
			);
		}
		if (!fileid) {
			fileid = old;
			return nullptr;
		}
		return old;
	}

	constexpr match_result<size_t> size() const noexcept {
		LARGE_INTEGER size;
		if (GetFileSizeEx(fileid, &size)) {
			return size.QuadPart;
		}
		return match::failed;
	}

	constexpr bool close() const noexcept {
		if (!is_loaded()) {
			return false;
		}
		return CloseHandle(fileid);
	}

	constexpr fileid_t id() const noexcept {
		return fileid;
	}

	template <comaccess ComAccess>
	[[nodiscard]] constexpr bool has() const noexcept {
		return permis & ComAccess;
	}

	[[nodiscard]] constexpr bool is_loaded() const noexcept {
		return fileid != INVALID_HANDLE_VALUE;
	}

public:

	constexpr ~filoader() noexcept {
		if (!is_loaded()) {
			return;
		}
		CloseHandle(fileid);
		fileid = nullptr;
	}
};