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
	memory_block* last;

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
		return static_cast<size_t>(last - block);
	}

	constexpr size_t spacing() const noexcept {
		return static_cast<size_t>(last - current);
	}

	constexpr void exten_block() noexcept {
		memory_block* old = block;
		size_t spalen = spacing();
		size_t newlen = total_number_block() + 2;
		block = static_cast<memory_block*> (
			std::malloc(sizeof(memory_block) * newlen)
		);
		current = block + spalen;
		last	= block + newlen;
		for (size_t i = 0; i < spalen; i++) {
			block[i] = std::move(old[i]);
		}
	}

	constexpr void respace() noexcept {
		if (current + 1 >= last) {
			exten_block();
		}
		init_cur_block(4000);
	}

	constexpr auto curr_info(std::size_t size) noexcept {
		struct info {
			block_holder& memory;
			size_t		  cursize;
			size_t		  sumsize;
		};
		info result  = {
			.memory  = current->area,
			.cursize = current->acur
		};
		result.sumsize = result.cursize + size;
		return result;
	}

	constexpr auto block_info(std::size_t size) noexcept {
		auto result = curr_info(size);
		if (result.sumsize > result.memory.size()) {
			respace();
			result.memory = current->area;
			result.cursize = current->acur;
		}
		return result;
	}

	template <class AllocType, class InfoType, class... ArgsTyp>
	constexpr auto hold_space (InfoType&	info,
							   ArgsTyp&&... args)
		noexcept
	{
		auto result    = info.memory.template address<AllocType>(info.cursize);
		current->acur += sizeof(AllocType);
		new (result) AllocType(std::forward<ArgsTyp>(args)...);
		return result;
	}

	template <class AllocType, class... ArgsType>
	AllocType* allocate_impl(ArgsType&&... args) noexcept {
		auto info = block_info(sizeof(AllocType));
		return hold_space<AllocType> (
			info, std::forward<ArgsType>(args)...
		);
	}

	template <rest::character CharType, class InfoType>
	constexpr auto hold_space (InfoType& info,
						 const CharType* string,
							   size_t	 size)
		noexcept
	{
		auto result = info.memory.template address<CharType>(info.cursize);
		if (size >= 1 && string[size - 1] == CharType()) {
			current->acur += 1;
			result[size] = CharType();
		}
		if constexpr (std::is_same_v<CharType, char>) {
			std::memcpy(result, string, size);
		}
		else {
			std::wmemcpy(result, string, size);
			size *= sizeof(wchar_t);
		}
		current->acur += size;
		return result;
	}

	template <rest::character CharType>
	constexpr CharType* allocate_impl (const CharType* string,
											 size_t	   size)
		noexcept
	{
		auto info = block_info(sizeof(CharType*));
		return hold_space<CharType> (
			info, string, size
		);
	}

	constexpr void reconstruct(std::size_t size) noexcept {
		block = static_cast<memory_block*> (
				std::malloc(sizeof(memory_block) * 2)
		);
		current = block;
		last	= block + 2;
		init_cur_block(size);
	}

	constexpr void checked() noexcept {
		if (!is_empty()) {
			return;
		}
		reconstruct(1);
	}

private:

	constexpr void assign(block_allocator& allocator) noexcept {
		block	= allocator.block;
		current = allocator.current;
		last	= allocator.last;
	}

	constexpr bool need_realloc(std::size_t size) noexcept {
		return is_empty() || current->acur + size > current->area.size();
	}

public:

	constexpr block_allocator()
		noexcept = default;

	constexpr block_allocator(block_allocator&& allocator)
		noexcept : block(allocator.block),
				   current(allocator.current),
				   last(allocator.last)
	{
		allocator.block = nullptr;
	}

	constexpr block_allocator(size_t size) noexcept :
		block (
			static_cast<memory_block*> (
				std::malloc(sizeof(memory_block) * 2)
			)
		),
		current(block), last(block + 2)
	{
		init_cur_block(size);
	}


public:

	template <class AllocType, class... ArgsType>
	AllocType* allocate(ArgsType&&... args) noexcept {
		checked();
		return allocate_impl<AllocType>(std::forward<ArgsType>(args)...);
	}

	template <rest::character CharType>
	CharType* allocate (const CharType* string,
							  size_t	size)
		noexcept
	{
		checked();
		return allocate_impl<CharType>(string, size);
	}

	template <class AllocType, class... ArgsType>
	constexpr std::optional<AllocType*> try_alloc (std::size_t		 size,
													    ArgsType&&... args)
		noexcept
	{
		if (need_realloc(size)) {
			return std::nullopt;
		}
		auto info = curr_info(sizeof(AllocType));
		return hold_space<AllocType> (
			info, std::forward<ArgsType>(args)...
		);
	}

	template <rest::character CharType>
	constexpr std::optional<CharType*> try_alloc(const CharType* string,
													   size_t	 size)
		noexcept
	{
		if (need_realloc(size)) {
			return std::nullopt;
		}
		auto info = curr_info(sizeof(CharType*));
		return hold_space<CharType> (
			info, string, size
		);
	}

	constexpr char* begin() const noexcept {
		return data<char>(0).value();
	}

	constexpr char* end() const noexcept {
		return current->area.template address<>(current->acur);
	}

	constexpr void reset() noexcept {
		current->acur = 0;
		begin()[0]	  = char();
	}

	template <class DataType>
	constexpr std::optional<DataType*> data(std::size_t off) const noexcept {
		return current->area.template address<DataType>(off);
	}

	constexpr bool is_empty() const noexcept {
		return !block;
	}

	constexpr std::size_t size() const noexcept {
		return current->acur;
	}

public:

	constexpr void operator=(block_allocator&& allocator) noexcept {
		assign(allocator);
		allocator.block = nullptr;
	}

public:

	constexpr ~block_allocator() noexcept {
		if (block == nullptr) {
			return;
		}
		memory_block* memory = block;
		memory_block* end    = current + 1;
		for (; memory != end; ++memory) {
			memory->~memory_block();
		}
		std::free(block);
	}

};