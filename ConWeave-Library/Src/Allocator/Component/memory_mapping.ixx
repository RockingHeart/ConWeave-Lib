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
	using handle		= HANDLE;
	using system_info_t = SYSTEM_INFO;

private:

	handle	  hand;
	pointer_t addr;
	size_t	  aend;

public:

	memory_mapping(const memory_mapping&)			 = delete;
	memory_mapping& operator=(const memory_mapping&) = delete;
	constexpr void resize(size_t) noexcept			 = delete;

private:

	constexpr void reset(memory_mapping& mapping) noexcept {
		mapping.hand = nullptr;
		mapping.addr = nullptr;
		mapping.aend = 0;
	}

	constexpr void reset() noexcept {
		return reset(*this);
	}

	constexpr void close_hand() noexcept {
		if (hand == nullptr) {
			return;
		}
		CloseHandle(hand);
	}

	constexpr void close_addr() noexcept {
		if (addr == nullptr) {
			return;
		}
		UnmapViewOfFile(addr);
	}

	constexpr void copy_assign(memory_mapping& mapping) noexcept {
		hand = mapping.hand;
		addr = mapping.addr;
		aend = mapping.aend;
	}

	constexpr void release() noexcept {
		close_hand();
		close_addr();
		reset();
	}

	constexpr void move_assign(memory_mapping&& mapping) noexcept {
		release();
		copy_assign(mapping);
		reset(mapping);
	}

	constexpr size_t align_to(size_t value,
							  size_t align)
		const noexcept
	{
		return (value + align - 1) & ~(align - 1);
	}

	static constexpr system_info_t sys_info() noexcept {
		system_info_t info {};
		GetSystemInfo(&info);
		return info;
	}

	static constexpr auto page_size() noexcept {
		static const auto size = sys_info().dwPageSize;
		return size;
	}

	static constexpr auto granularity() noexcept {
		static const size_t size = sys_info().dwAllocationGranularity;
		return size;
	}

	size_t block_size(size_t base) noexcept {
		return align_to(base, page_size());
	}

	constexpr handle create_mapping(size_t& size) noexcept {
		size = block_size(size);
		if (size == 0) return nullptr;
		return CreateFileMappingA (
			INVALID_HANDLE_VALUE, nullptr,
			PAGE_READWRITE,
			static_cast<DWORD>(size >> 32),
			static_cast<DWORD>(size & 0xFFFFFFFF),
			nullptr
		);
	}

	constexpr pointer_t create_memory() noexcept {
		if (unmapped()) return nullptr;
		return static_cast<pointer_t> (
			MapViewOfFile (
				hand, FILE_MAP_ALL_ACCESS, 0, 0, granularity()
			)
		);
	}

	constexpr size_t set_aend(size_t size) const noexcept {
		if (failed()) return 0; return size;
	}

public:

	constexpr memory_mapping()
		noexcept : hand(nullptr),
				   addr(nullptr),
				   aend(0ull)
	{}

	constexpr memory_mapping(size_t size)
		noexcept : hand(create_mapping(size)),
				   addr(create_memory()),
				   aend(set_aend(size))
	{}

	constexpr memory_mapping(memory_mapping&& mapping)
		noexcept : hand(mapping.hand),
				   addr(mapping.addr),
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