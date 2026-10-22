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
	size_t		acur;
};

template <class BlockHolder>
class dast::block_allocator
{
private:
	using block_holder  = BlockHolder;
	using system_info_t = SYSTEM_INFO;

public:
	using size_t	= typename block_holder::size_t;
	using byte_t	= typename block_holder::byte_t;
	using pointer_t = typename block_holder::pointer_t;

protected:
	using memory_block = memory_block_t<block_holder>;

private:

	memory_block* base;
	memory_block* current;
	memory_block* last;

public:

	block_allocator(const block_allocator&)			   = delete;
	block_allocator& operator=(const block_allocator&) = delete;

private:

	constexpr size_t align_to (size_t value,
							   size_t align)
		const noexcept
	{
		return (value + align - 1) & ~(align - 1);
	}

	constexpr void set_cur_block(size_t block_size) noexcept {
		std::construct_at(current, block_size, 0);
	}

	constexpr system_info_t sys_info() noexcept {
		SYSTEM_INFO info {};
		GetSystemInfo(&info);
		return info;
	}

	constexpr size_t block_size(size_t base) noexcept {
		return align_to(base, sys_info().dwPageSize);
	}

	constexpr size_t total_number_block() const noexcept {
		return static_cast<size_t>(last - base);
	}

	constexpr void exten_move_to(memory_block* new_block) noexcept {
		size_t used_block = used();
		for (size_t i = 0; i <= used_block; i++)
			std::construct_at(new_block + i, std::move(base[i]));
	}

	constexpr void exten_init(memory_block* new_block) noexcept {
		size_t used_block = used();
		size_t old_size   = total_number_block();
		size_t newlen	  = old_size + 2;
		for (size_t i = used_block; i <= newlen; i++)
			std::construct_at(new_block + i, memory_block{});
	}

	constexpr void exten_update(memory_block* new_block) noexcept {
		size_t used_block = used();
		size_t old_size   = total_number_block();
		size_t newlen	  = old_size + 2;
		base			  = new_block;
		last			  = base + newlen;
		current			  = base + used_block;
	}

	constexpr memory_block* alloc_block(size_t size) noexcept {
		return static_cast<memory_block*>(
			std::malloc(sizeof(memory_block) * size)
		);
	}

	constexpr memory_block* alloc_new_block() noexcept {
		return alloc_block(total_number_block() + 2);
	}

	constexpr void exten_block() noexcept {
		auto* new_block = alloc_new_block();
		exten_move_to(new_block);
		exten_init(new_block);
		release_used();
		exten_update(new_block);
	}

	constexpr bool reachable() const noexcept {
		return current + 1 < last;
	}

	constexpr void respace(size_t size) noexcept {
		if (!reachable()) {
			exten_block();
		}
		current = current + 1;
	}

	constexpr bool leftover(size_t size)
		const noexcept
	{
		block_holder& block  = current->area;
		return block.size() > current->acur + size;
	}

	constexpr void enhance_current() noexcept {
		++current; current->acur = 0;
	}

	constexpr auto enhance_block(size_t size)
		noexcept -> memory_block*
	{
		if (base == nullptr) return nullptr;
		if (!cross(size))	 return current;
		if (!reachable())	 return nullptr;
		return enhance_current();
	}

	constexpr void check_block(size_t size) noexcept {
		block_holder& block = current->area;
		size_t block_size   = current->acur;
		size_t next_size    = block_size + size;
		if (next_size <= block.size()) {
			return;
		}
		respace(next_size + 1 * 2);
	}

	template <class Type>
	constexpr Type* enhance_space(size_t size) noexcept {
		block_holder& block = current->area;
		size_t block_size   = current->acur;
		auto result = block.template address<Type> (
			block_size
		);
		current->acur += size;
		return result;
	}

	template <class AllocType, class... ArgsTyp>
	constexpr auto hold_space(ArgsTyp&&...   args)
		noexcept -> AllocType*
	{
		auto result = enhance_space<AllocType> (
			sizeof(AllocType)
		);
		new (result) AllocType (
			std::forward<ArgsTyp>(args)...
		);
		return result;
	}

	constexpr bool write_segment(size_t size) noexcept {
		return cross(size) && leftover(size);
	}

	template <class CharType>
	constexpr size_t write_remain(CharType* string) noexcept {
		CharType* pointer = section<CharType>();
		size_t    length  = remain() - 1;
		strutil::strcopy(pointer, string, length);
		pointer[length] = '\0';
		return length;
	}

	template <class Type>
	constexpr Type* append_string (Type*  string,
								   size_t size)
		noexcept
	{
		auto result = enhance_space<Type> (
			size * sizeof(Type)
		);
		strutil::strcopy(result, string, size);
		return result;
	}

	template <class Type>
	constexpr Type* chunks_write (Type*  string,
								  size_t size) noexcept {
		size_t length = write_remain<Type>(string);
		size -= length; string += length;
		respace(size * sizeof(Type));
		return append_string(string, size);
	};
	 
	template <rest::character CharType>
	constexpr auto hold_space (CharType* string,
							   size_t	 size)
		noexcept -> CharType*
	{
		if (write_segment(size)) {
			return chunks_write(string, size);
		}
		check_block(size * sizeof(CharType));
		return append_string(string, size);
	}

	template <class AllocType, class... ArgsType>
	AllocType* allocate_impl(ArgsType&&... args) noexcept {
		check_block(sizeof(AllocType));
		return hold_space<AllocType> (
			std::forward<ArgsType>(args)...
		);
	}

	template <rest::character CharType>
	CharType* allocate_impl (const CharType* string,
								   size_t	 size)
		noexcept
	{
		return hold_space<CharType>(string, size);
	}

	constexpr void construct(size_t size) noexcept {
		base = static_cast<memory_block*> (
			std::malloc(sizeof(memory_block) * 2)
		);
		current = base;
		last	= base + 2;
		set_cur_block(size);
	}

	constexpr void initialize(size_t size) noexcept {
		if (!is_empty()) {
			return;
		}
		construct(size);
	}

private:

	constexpr void assign(block_allocator& allocator) noexcept {
		base	= allocator.base;
		current = allocator.current;
		last	= allocator.last;
	}

	constexpr bool should_allocate(size_t size) noexcept {
		if (is_empty()) {
			return true;
		}
		return cross(size);
	}

private:

	constexpr void reset_member() noexcept {
		base    = nullptr;
		current = nullptr;
		last    = nullptr;
	}

	constexpr void release_used() noexcept {
		size_t used_block = used();
		for (size_t i = 0; i <= used_block; ++i)
			std::destroy_at(base + i);
		std::free(base);
	}

	constexpr void release() noexcept {
		if (is_empty()) {
			return;
		}
		release_used();
		reset_member();
	}

public:

	constexpr block_allocator()
		noexcept : base	  (nullptr),
				   current(nullptr),
				   last	  (nullptr)
	{};

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
		initialize((sizeof(args) + ...));
		return allocate_impl<AllocType> (
			std::forward<ArgsType>(args)...
		);
	}

	template <class AllocType, class... ArgsType>
	constexpr auto try_alloc(size_t		   size,
							 ArgsType&&... args)
		noexcept -> std::optional<AllocType*>
	{
		if (should_allocate(size)) {
			return std::nullopt;
		}

		return hold_space<AllocType> (
			std::forward<ArgsType>(args)...
		);
	}

public:

	constexpr void reset_state(memory_block& block) noexcept {
		size_t size	= block.acur;
		block.acur = 0;
		std::memset(block.area.address(0), 0, size);
	}

	constexpr void reset_status() noexcept {
		for (auto& block : *this) {
			reset_state(block);
		}
	}

	constexpr void rewind() noexcept {
		current = base; current->acur = 0;
	}

	constexpr bool next_block() noexcept {
		if (!reachable()) return false;
		size_t next_size = current->acur * 2;
		++current;
		set_cur_block(next_size);
		return true;
	}

	constexpr void resize_curr() noexcept {
		current->acur = 0;
	}

public:

	constexpr pointer_t bytecode() const noexcept {
		return current->area.bytecode();
	}

	template <class DataType>
	constexpr DataType* data(size_t off = 0) const noexcept {
		return current->area.template address<DataType>(off);
	}

	template <class AccType = byte_t>
	constexpr auto section(memory_block& block) noexcept {
		block_holder& holder = block.area;
		return holder.template address<AccType>(block.acur);
	}

	template <class AccType = byte_t>
	constexpr AccType* section() noexcept {
		return data<AccType>(current->acur);
	}

	template <class AccType>
	constexpr auto retreat(size_t size = sizeof(AccType))
		noexcept -> AccType*
	{
		memory_block* result = enhance_block(size);
		if (result == nullptr) return nullptr;
		memory_block& block  = *result;
		auto current_element = section<AccType>(block);
		block.acur			+= sizeof(AccType);
		return current_element;
	};

public:

	constexpr memory_block* begin() const noexcept {
		return base;
	}

	constexpr memory_block* cur_block() const noexcept {
		return current;
	}

	constexpr memory_block* end() const noexcept {
		return cur_block() + 1;
	}

public:

	constexpr bool cross(size_t size)
		const noexcept
	{
		block_holder& block = current->area;
		size_t length = current->acur;
		return length + size > block.size();
	}

	constexpr bool is_empty() const noexcept {
		return base == nullptr;
	}

	constexpr bool based() const noexcept {
		return current == base;
	}

public:

	constexpr size_t spacing() const noexcept {
		return static_cast<size_t>(last - current);
	}

	constexpr size_t remain() const noexcept {
		block_holder& holder = current->area;
		return holder.size() - current->acur;
	}

	constexpr size_t used() const noexcept {
		return current - base;
	}

	constexpr size_t size() const noexcept {
		return current->acur;
	}

public:

	constexpr void operator=(block_allocator&& allocator) noexcept {
		if (this == &allocator) return;
		release();
		assign(allocator);
		allocator.reset_member();
	}

	constexpr memory_block& operator[](size_t position) const {
		if (position >= used()) {
			throw "No such base";
		}
		return base[position];
	};

public:

	constexpr ~block_allocator() noexcept {
		release();
	}

};