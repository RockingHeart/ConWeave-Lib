module;
#include <windows.h>

export module dast.memory_mapping;

import dast.string;
import std;

std::size_t mapping_count = 0;

export class memory_mapping {
private:
	using size_t = std::size_t;
	using handle =		HANDLE;

private:

	handle		hand;
	char*		addr;
	std::size_t aend;

private:

	static constexpr dast::cstring mapping_name() noexcept {
		static char buffer[16];
		auto [end, _] = std::to_chars (
			buffer, buffer + sizeof(buffer),
			mapping_count
		);
		*end = '\0';
		dast::cstring mapping_identity (
			"CW.MemoryMapping: "
		);
		mapping_identity += buffer;
		mapping_count	 += 1;
		return mapping_identity;
	}

	constexpr void reset(memory_mapping& mapping) noexcept {
		mapping.hand = nullptr;
		mapping.addr = nullptr;
		mapping.aend = 0;
	}

	constexpr void copy_assign(memory_mapping& mapping) noexcept {
		hand = mapping.hand;
		addr = mapping.addr;
		aend = mapping.aend;
	}

	constexpr void move_assign(memory_mapping&& mapping) noexcept {
		copy_assign(mapping);
		reset(mapping);
	}

public:

	constexpr memory_mapping()
		noexcept = default;

	constexpr memory_mapping(size_t size) noexcept :
		hand (
			CreateFileMappingA (
				INVALID_HANDLE_VALUE, NULL,
				PAGE_READWRITE, 0, size,
				mapping_name().const_string()
			)
		),
		addr (
			static_cast<char*> (
				MapViewOfFile (
					hand, FILE_MAP_ALL_ACCESS,
					0, 0, size
				)
			)
		), aend(size)
	{}

	constexpr memory_mapping(memory_mapping& mapping)
		noexcept : hand(mapping.hand), addr(mapping.addr),
				   aend(mapping.aend)
	{}

	constexpr memory_mapping(memory_mapping&& mapping)
		noexcept : hand(mapping.hand), addr(mapping.addr),
				   aend(mapping.aend)
	{
		reset(mapping);
	}

public:

	template <class AddrType = char>
	constexpr AddrType* address(std::size_t size) noexcept {
		return reinterpret_cast<AddrType*>(addr + size);
	}

	constexpr std::size_t size() const noexcept {
		return aend;
	}

	constexpr bool mapped() const noexcept {
		return hand != nullptr && !failed();
	}

	constexpr bool failed() const noexcept {
		return hand == INVALID_HANDLE_VALUE;
	}

public:

	constexpr void operator=(memory_mapping& mapping) {
		copy_assign(mapping);
	}

	constexpr void operator=(memory_mapping&& mapping) {
		move_assign(std::move(mapping));
	}

public:

	constexpr ~memory_mapping() noexcept {
		if (!mapped()) {
			return;
		}
		UnmapViewOfFile(addr);
		CloseHandle(hand);
	}

};