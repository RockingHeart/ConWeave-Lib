module;
#include <windows.h>

export module dast.memory_mapping;

import std;

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

public:

	memory_mapping(const memory_mapping&)			 = delete;
	memory_mapping& operator=(const memory_mapping&) = delete;

private:

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

	void release() noexcept {
		if (addr != nullptr) {
			UnmapViewOfFile(addr);
		}
		if (hand != nullptr) {
			CloseHandle(hand);
		}
		hand = nullptr;
		addr = nullptr;
		aend = 0;
	}

	constexpr void move_assign(memory_mapping&& mapping) noexcept {
		release();
		copy_assign(mapping);
		reset(mapping);
	}

public:

	constexpr memory_mapping()
		noexcept : hand(nullptr),
				   addr(nullptr),
				   aend(0ull)
	{}

	constexpr memory_mapping(size_t size)
		noexcept : memory_mapping()
	{
		hand = CreateFileMappingA (
			INVALID_HANDLE_VALUE, nullptr,
			PAGE_READWRITE, 0,
			static_cast<DWORD>(size), nullptr
		);
		if (hand == nullptr) {
			return;
		}
		addr = static_cast<pointer_t>(
			MapViewOfFile(hand, FILE_MAP_ALL_ACCESS, 0, 0, size)
		);
		if (addr == nullptr) {
			CloseHandle(hand);
			hand = nullptr;
			return;
		}
		aend = size;
	}

	constexpr memory_mapping(memory_mapping&& mapping)
		noexcept : hand(mapping.hand), addr(mapping.addr),
				   aend(mapping.aend)
	{
		reset(mapping);
	}

public:

	template <class AddrType = byte_t>
	constexpr AddrType* address(size_t offset) noexcept {
		return reinterpret_cast<AddrType*>(addr + offset);
	}

	constexpr pointer_t bytecode() noexcept {
		return address<byte_t>(0);
	}

	constexpr size_t size() const noexcept {
		return aend;
	}

	constexpr bool mapped() const noexcept {
		return hand != nullptr;
	}

	constexpr bool unmapped() const noexcept {
		return hand == nullptr;
	}

	constexpr bool failed() const noexcept {
		return addr == nullptr;
	}

public:

	constexpr memory_mapping& operator=(memory_mapping&& mapping) {
		if (this == &mapping) return *this;
		move_assign(std::move(mapping));
		return *this;
	}

public:

	constexpr ~memory_mapping() noexcept {
		release();
	}

};