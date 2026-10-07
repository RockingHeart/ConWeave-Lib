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
private:
	using block_holder = BlockHolder;

public:
	using size_t	=		   std::size_t;
	using pointer_t = typename block_holder::pointer_t;

protected:
	using memory_block = memory_block_t<block_holder>;

private:

	memory_block* base;
	memory_block* current;
	memory_block* last;

private:

	constexpr size_t align_to(size_t value,
							  size_t align)
		const noexcept
	{
		return (value + align - 1) & ~(align - 1);
	}

	constexpr void set_cur_block(size_t block_size) noexcept {
		std::construct_at(current, block_size, 0);
	}

	constexpr size_t block_size(size_t base) noexcept {
		SYSTEM_INFO system_info{};
		GetSystemInfo(&system_info);
		size_t block_size = align_to(
			base, system_info.dwPageSize
		);
		return block_size;
	}

	constexpr size_t total_number_block() const noexcept {
		return static_cast<size_t>(last - base);
	}

	constexpr size_t spacing() const noexcept {
		return static_cast<size_t>(last - current);
	}

	constexpr void exten_block() noexcept {
		memory_block* old = base;
		size_t spalen	  = spacing();
		size_t newlen	  = total_number_block() + 2;
		base = static_cast<memory_block*> (
			std::malloc(sizeof(memory_block) * newlen)
		);
		current = base + spalen;
		last	= base + newlen;
		for (size_t i = 0; i < spalen; i++) {
			base[i] = std::move(old[i]);
		}
	}

	constexpr bool leftover_block() const noexcept {
		return current + 1 < last;
	}

	constexpr void respace() noexcept {
		if (!leftover_block()) {
			exten_block();
		}
		set_cur_block(4000);
	}

	constexpr auto search_block(std::size_t size)
		noexcept -> memory_block*
	{
		if (base == nullptr) {
			return nullptr;
		}
		block_holder& block    = current->area;
		std::size_t block_size = current->acur;
		if (block_size + size > block.size()) {
			if (!leftover_block()) {
				return nullptr;
			}
			++current;
		}
		return current;
	}

	constexpr void check_block(std::size_t size) noexcept {
		block_holder& block    = current->area;
		std::size_t block_size = current->acur;
		std::size_t next_size  = block_size + size;
		if (next_size < block.size()) {
			return;
		}
		respace();
	}

	template <class AllocType, class... ArgsTyp>
	constexpr auto hold_space(ArgsTyp&&...   args)
		noexcept
	{
		std::size_t& block_size = current->acur;
		block_holder& block     = current->area;
		auto result = block.template address<AllocType> (
			block_size
		);
		block_size += sizeof(AllocType);
		new (result) AllocType(std::forward<ArgsTyp>(args)...);
		return result;
	}

	template <class AllocType, class... ArgsType>
	AllocType* allocate_impl(ArgsType&&... args) noexcept {
		check_block(sizeof(AllocType));
		return hold_space<AllocType> (
			std::forward<ArgsType>(args)...
		);
	}

	template <rest::character CharType>
	constexpr auto hold_space(const CharType*     string,
									size_t	    size)
		noexcept
	{
		std::size_t& block_size = current->acur;
		block_holder& block		= current->area;
		auto result = block.template address<CharType>(block_size);
		if (size >= 1 && string[size - 1] == CharType()) {
			block_size += 1;
			result[size] = CharType();
		}
		if constexpr (std::is_same_v<CharType, char>) {
			std::memcpy(result, string, size);
		}
		else {
			std::wmemcpy(result, string, size);
			size *= sizeof(wchar_t);
		}
		block_size += size;
		return result;
	}

	template <rest::character CharType>
	constexpr CharType* allocate_impl(const CharType* string,
											size_t	  size)
		noexcept
	{
		check_block(sizeof(CharType*));
		return hold_space<CharType> (
			string, size
		);
	}

	constexpr void construct(std::size_t size) noexcept {
		base = static_cast<memory_block*> (
			std::malloc(sizeof(memory_block) * 2)
		);
		current = base;
		last	= base + 2;
		set_cur_block(size);
	}

	constexpr void initialize() noexcept {
		if (!is_empty()) {
			return;
		}
		construct(1);
	}

private:

	constexpr void assign(block_allocator& allocator) noexcept {
		base	= allocator.base;
		current = allocator.current;
		last	= allocator.last;
	}

	constexpr bool should_allocate(std::size_t size) noexcept {
		block_holder& block = current->area;
		std::size_t length  = current->acur;
		return is_empty() || length + size > block.size();
	}

public:

	constexpr block_allocator()
		noexcept = default;

	constexpr block_allocator(block_allocator&& allocator)
		noexcept : base(allocator.base),
				   current(allocator.current),
				   last(allocator.last)
	{
		allocator.base = nullptr;
	}

	constexpr block_allocator(size_t size) noexcept :
		base (
			static_cast<memory_block*> (
				std::malloc(sizeof(memory_block) * 2)
			)
		),
		current(base), last(base + 2)
	{
		set_cur_block(size);
	}


public:

	template <class AllocType, class... ArgsType>
	AllocType* allocate(ArgsType&&... args) noexcept {
		initialize();
		return allocate_impl<AllocType>(std::forward<ArgsType>(args)...);
	}

	template <rest::character CharType>
	CharType* allocate(const CharType* string,
							 size_t	   size)
		noexcept
	{
		initialize();
		return allocate_impl<CharType>(string, size);
	}

	template <class AllocType, class... ArgsType>
	constexpr std::optional<AllocType*> try_alloc(std::size_t		 size,
													   ArgsType&&... args)
		noexcept
	{
		if (should_allocate(size)) {
			return std::nullopt;
		}
		return hold_space<AllocType> (
			std::forward<ArgsType>(args)...
		);
	}

	template <rest::character CharType>
	constexpr std::optional<CharType*> try_alloc(const CharType* string,
													   size_t	 size)
		noexcept
	{
		if (should_allocate(size)) {
			return std::nullopt;
		}
		return hold_space<CharType> (
			string, size
		);
	}

public:

	constexpr void reset_state(memory_block& block) noexcept {
		block_holder& memory = block.area;
		char* address		 = block.template address(0);
		std::size_t& size	 = block.acur;
		std::memset(address, 0, size);
		size = 0;
	}

	constexpr void reset_status() noexcept {
		for (auto& block : *this) {
			reset_state(block);
		}
	}

	constexpr void start_over() noexcept {
		current = base;
	}

public:

	template <class DataType>
	constexpr std::optional<DataType*> data(std::size_t off) const noexcept {
		return current->area.template address<DataType>(off);
	}

	template <class AccType>
	constexpr decltype(auto) unchecked_retreat(std::size_t size = sizeof(AccType))
		noexcept
	{
		block_holder& block = search_block(size)->area;
		return block.template address<AccType>(size);
	};

public:

	constexpr memory_block* begin() const noexcept {
		return base;
	}

	constexpr memory_block* end() const noexcept {
		return current;
	}

	constexpr bool is_empty() const noexcept {
		return base == nullptr;
	}

	constexpr std::size_t number() const noexcept {
		return last - base;
	}

	constexpr std::size_t size() const noexcept {
		return current->acur;
	}

public:

	constexpr void operator=(block_allocator&& allocator) noexcept {
		assign(allocator);
		allocator.base = nullptr;
	}

	constexpr memory_block& operator[](std::size_t position) const noexcept {
		if (position > number()) {
			throw "No such base";
		}
		return base[position];
	};

public:

	constexpr ~block_allocator() noexcept {
		if (base == nullptr) {
			return;
		}
		for (auto& block : *this) {
			block.~memory_block();
		}
		std::free(base);
	}

};