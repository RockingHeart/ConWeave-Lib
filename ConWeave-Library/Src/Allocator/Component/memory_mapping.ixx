module;
#include <windows.h>

export module dast.memory_mapping;

import std;
import dast.string;

std::size_t mapping_count = 0;

export namespace dast
{
	class memory_mapping;
};

class dast::memory_mapping {
public:
	using size_t	= std::size_t;
	using byte_t	= std::byte;
	using pointer_t =	   byte_t*;

private:
	using handle = HANDLE;

private:

	handle	  hand;
	pointer_t addr;
	size_t	  aend;

private:

	/*static constexpr dast::cstring mapping_name() noexcept {
		static char buffer[16];
		auto [end, _] = std::to_chars (
			buffer, buffer + sizeof(buffer),
			mapping_count
		);
		*end = '\0';
		dast::cstring mapping_identity (
			"CWL.MemoryMapping: "
		);
		mapping_identity += buffer;
		mapping_count	 += 1;
		return mapping_identity;
	}*/

	static const char* mapping_name() noexcept {
		static char buffer[64];
		static constexpr char prefix[] = "CWL.MemoryMapping: ";
		std::size_t offset = 0;
		for (; prefix[offset] != '\0'; ++offset)
			buffer[offset] = prefix[offset];
		auto [end, ec] = std::to_chars (
			buffer + offset, buffer + sizeof(buffer), mapping_count
		);
		*end = '\0';
		mapping_count += 1;
		return buffer;
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
				INVALID_HANDLE_VALUE, nullptr,
				PAGE_READWRITE, 0, size,
				mapping_name()
			)
		),
		addr (
			static_cast<pointer_t> (
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

	template <class AddrType = byte_t>
	constexpr AddrType* address(std::size_t offset) noexcept {
		return reinterpret_cast<AddrType*>(addr + offset);
	}

	constexpr pointer_t bytecode() noexcept {
		return address<byte_t>(0);
	}

	constexpr std::size_t size() const noexcept {
		return aend;
	}

	constexpr bool mapped() const noexcept {
		return hand != nullptr && !failed();
	}

	constexpr bool failed() const noexcept {
		return hand == nullptr;
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