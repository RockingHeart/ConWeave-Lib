module;
#include <windows.h>

export module dast.allocator : block;

import std;
import utility;

export namespace dast
{
	template <class>
	class block_allocator;
}

template <class BlockHolder>
struct memory_block_t {
	BlockHolder area;
	std::size_t acur;
};

template <class BlockHolder>
class dast::block_allocator {
public:
	using size_t = std::size_t;

public:
	using memory_block = memory_block_t<BlockHolder>;
	using block_holder = BlockHolder;

private:

	memory_block* block;
	memory_block* current;
	memory_block* end;

private:

	constexpr size_t align_to(size_t value,
		size_t align)
		const noexcept
	{
		return (value + align - 1) & ~(align - 1);
	}

	constexpr void init_cur_block(size_t block_size) noexcept {
		memory_block& curr_block = *current;
		curr_block.area.reallocate(block_size);
		curr_block.acur = 0;
	}

	constexpr size_t block_size(size_t base) noexcept {
		SYSTEM_INFO system_info{};
		GetSystemInfo(&system_info);
		size_t block_size = align_to (
			base, system_info.dwPageSize
		);
		return block_size;
	}

	constexpr size_t total_number_block() const noexcept {
		return static_cast<size_t>(end - block);
	}

	constexpr size_t spacing() const noexcept {
		return static_cast<size_t>(end - current);
	}

	constexpr void exten_block() noexcept {
		memory_block* old = block;
		size_t spalen = spacing();
		size_t newlen = total_number_block() + 2;
		block = static_cast<memory_block*> (
			std::malloc(sizeof(memory_block) * newlen)
		);
		current = block + spalen;
		end		= block + newlen;
		for (size_t i = 0; i < spalen; i++) {
			block[i] = std::move(old[i]);
		}
	}

	constexpr void respace() noexcept {
		if (current + 1 >= end) {
			exten_block();
		}
		init_cur_block(4000);
	}

	template <class AllocType>
	AllocType* allocate_impl(AllocType&& value) noexcept {
		block_holder& memory = current->area;
		size_t cursize		 = current->acur;
		size_t sumsize		 = cursize + sizeof(AllocType);
		if (sumsize > memory.size()) {
			respace();
			memory  = current->area;
			cursize = current->acur;
		}
		auto result    = memory.template address<AllocType>(cursize);
		current->acur += sizeof(AllocType);
		new (result) AllocType(std::forward<AllocType>(value));
		return result;
	}

	template <rest::character CharType>
	CharType* allocate_impl (const CharType* string,
								   size_t	 size)
		noexcept
	{
		block_holder& memory = current->area;
		size_t cursize		 = current->acur;
		size_t sumsize		 = cursize + sizeof(CharType*);
		if (sumsize > memory.size()) {
			respace();
			memory  = current->area;
			cursize = current->acur;
		}
		auto result    = memory.template address<CharType>(cursize);
		if (size >= 1 && string[size - 1] != CharType()) {
			current->acur += 1;
		}
		current->acur += size;
		if constexpr (std::is_same_v<CharType, char>) {
			std::memcpy(result, string, size);
		}
		else {
			std::wmemcpy(result, string, size);
		}
		result[size] = CharType();
		return result;
	}

public:

	constexpr block_allocator()
		noexcept : block_allocator(1)
	{}

	constexpr block_allocator(size_t size) noexcept :
		block (
			static_cast<memory_block*> (
				std::malloc(sizeof(memory_block) * 2)
			)
		),
		current(block), end(block + 2)
	{
		init_cur_block(block_size(size * 4000));
	}


public:

	template <class AllocType>
	AllocType* allocate(AllocType&& value) noexcept {
		return allocate_impl(std::move(value));
	}

	template <class AllocType>
	AllocType* allocate(AllocType& value) noexcept {
		return allocate_impl(value);
	}

	template <rest::character CharType>
	CharType* allocate (const CharType* string,
							  size_t	size)
		noexcept
	{
		return allocate_impl(string, size);
	}

public:

	constexpr ~block_allocator() noexcept {
		if (block == nullptr) {
			return;
		}
		memory_block* memory = block;
		for (; memory != current; ++memory) {
			memory->~memory_block();
		}
		std::free(block);
	}

};