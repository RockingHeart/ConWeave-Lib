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
	using block_holder = BlockHolder;

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

	constexpr size_t block_size(size_t base) noexcept {
		SYSTEM_INFO system_info{};
		GetSystemInfo(&system_info);
		size_t block_size = align_to (
			base, system_info.dwPageSize
		);
		return block_size;
	}

	constexpr size_t total_number_block() const noexcept {
		return static_cast<size_t>(last - base);
	}

	constexpr void exten_block() noexcept {
		block_allocator old = *this;
		size_t used_block   = used();
		size_t newlen	    = total_number_block() + 2;
		base = static_cast<memory_block*> (
			std::malloc(sizeof(memory_block) * newlen)
		);
		memory_block* old_base = old.base;
		for (size_t i = 0; i <= used_block; i++) {
			std::construct_at(&base[i], std::move(old_base[i]));
		}
		last	= base + newlen;
		current = base + used_block;
		old.release();
	}

	constexpr bool reachable() const noexcept {
		return current + 1 < last;
	}

	constexpr void respace(size_t size) noexcept {
		if (!reachable()) {
			exten_block();
		}
		current = current + 1;
		set_cur_block(size);
	}

	constexpr void respace() noexcept {
		return respace(current->acur * 2);
	}

	constexpr void enhance_current(size_t size) noexcept {
		++current;
		set_cur_block(size);
	}

	constexpr bool cross(size_t size)
		const noexcept
	{
		block_holder& block = current->area;
		size_t length = current->acur;
		return length + size > block.size();
	}

	constexpr bool leftover(size_t size)
		const noexcept
	{
		block_holder& block  = current->area;
		return block.size() > current->acur + size;
	}

	constexpr auto enhance_block(size_t size)
		noexcept -> memory_block*
	{
		if (base == nullptr) {
			return nullptr;
		}

		if (!cross(size)) {
			return current;
		}

		if (!reachable()) {
			return nullptr;
		}

		enhance_current(size);
		return current;
	}

	constexpr void check_block(size_t size) noexcept {
		block_holder& block    = current->area;
		size_t block_size = current->acur;
		size_t next_size  = block_size + size;
		if (next_size <= block.size()) {
			return;
		}
		respace();
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
	constexpr size_t write_remain(const CharType* string) noexcept {
		CharType* pointer = section<CharType>();
		size_t    length  = remain() - 1;
		strutil::strcopy(pointer, string, length);
		pointer[length] = '\0';
		return length;
	}

	template <class Type>
	constexpr Type* append_string (const Type*  string,
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
	constexpr Type* chunks_write (const Type*  string,
										size_t size) noexcept {
		size_t length = write_remain<Type>(string);
		size -= length; string += length;
		respace(size * sizeof(Type));
		return append_string(string, size);
	};
	 
	template <rest::character CharType>
	constexpr auto hold_space (const CharType* string,
									 size_t	   size)
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
		if (is_empty()) return true;
		block_holder& block  = current->area;
		size_t		  length = current->acur;
		return length + size > block.size();
	}

private:

	constexpr void release() noexcept {
		if (base == nullptr) {
			return;
		}

		for (auto& block : *this) {
			block.~memory_block();
		}
		std::free(base);
		base    = nullptr;
		current = nullptr;
		last    = nullptr;
	}

public:

	constexpr block_allocator()
		noexcept : base	  (nullptr),
				   current(nullptr),
				   last	  (nullptr)
	{};

	constexpr block_allocator(const block_allocator& allocator)
		noexcept : base(allocator.base),
				   current(allocator.current),
				   last(allocator.last)
	{}

	constexpr block_allocator(block_allocator&& allocator)
		noexcept : block_allocator(allocator)
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
	constexpr std::optional<AllocType*> try_alloc(size_t		size,
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

public:

	constexpr void reset_state(memory_block& block) noexcept {
		block_holder& memory  = block.area;
		char*		  address = block.bytecode();
		size_t&		  size	  = block.acur;
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
		release();
		assign(allocator);
		allocator.base = nullptr;
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