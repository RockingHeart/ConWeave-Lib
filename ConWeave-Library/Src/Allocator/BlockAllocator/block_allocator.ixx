module;
#include <windows.h>

export module dast.allocator : block;

import std;
import dast.memory_mapping;

struct mapping_block {
	memory_mapping area;
	std::size_t	   acur;
};

export class block_allocator {
private:

	mapping_block* block;
	mapping_block* current;
	mapping_block* end;

private:

	constexpr std::size_t align_to(std::size_t value,
		std::size_t align)
		const noexcept
	{
		return (value + align - 1) & ~(align - 1);
	}

	constexpr void init_cur_block(std::size_t block_size) noexcept {
		mapping_block& curr_block = *current;
		curr_block.area.memory_mapping::memory_mapping(block_size);
		curr_block.acur = 0;
	}

	// Reduce TLB miss rate
	constexpr std::size_t block_size(std::size_t base) noexcept {
		SYSTEM_INFO system_info{};
		GetSystemInfo(&system_info);
		std::size_t block_size = align_to (
			base, system_info.dwPageSize
		);
		return block_size;
	}

	constexpr std::size_t total_number_block() const noexcept {
		return static_cast<std::size_t>(end - block);
	}

	constexpr std::size_t spacing() const noexcept {
		return static_cast<std::size_t>(end - current);
	}

	constexpr void exten_block() noexcept {
		mapping_block* old = block;
		std::size_t spalen = spacing();
		std::size_t newlen = total_number_block() + 2;
		block = static_cast<mapping_block*> (
			std::malloc(sizeof(mapping_block) * newlen)
		);
		current = block + spalen;
		end		= block + newlen;
		for (std::size_t i = 0; i < spalen; i++) {
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
	AllocType* allocate_impl(AllocType&& value) {
		memory_mapping& mapping = current->area;
		std::size_t cursize		= current->acur;
		std::size_t sumsize		= cursize + sizeof(AllocType);
		if (sumsize > mapping.size()) {
			respace();
			mapping = current->area;
			cursize = current->acur;
		}
		auto result    = mapping.address<AllocType>(cursize);
		current->acur += sizeof(AllocType);
		new (result) AllocType(std::forward<AllocType>(value));
		return result;
	}

public:

	constexpr block_allocator()
		noexcept : block_allocator(1)
	{}

	constexpr block_allocator(std::size_t size) noexcept :
		block (
			static_cast<mapping_block*> (
				std::malloc(sizeof(mapping_block) * 2)
			)
		),
		current(block), end(block + 2)
	{
		init_cur_block(block_size(size * 4000));
	}


public:

	template <class AllocType>
	AllocType* allocate(AllocType&& value) {
		return allocate_impl(std::move(value));
	}

	template <class AllocType>
	AllocType* allocate(AllocType& value) {
		return allocate_impl(value);
	}

public:

	constexpr ~block_allocator() noexcept {
		if (block == nullptr) {
			return;
		}
		mapping_block* mapping = block;
		for (; mapping != current; ++mapping) {
			mapping->~mapping_block();
		}
		std::free(block);
	}

};