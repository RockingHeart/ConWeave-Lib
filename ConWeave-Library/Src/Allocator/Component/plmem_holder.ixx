export module dast.plmem_holder;

export namespace dast
{
	class plmem_holder;
}

import std;

class dast::plmem_holder {
public:
	using size_t	= std::size_t;
	using byte_t	= std::byte;
	using pointer_t =	   byte_t*;

private:

	pointer_t addr;
	size_t	  aend;

public:

	plmem_holder(const plmem_holder&)            = delete;
    plmem_holder& operator=(const plmem_holder&) = delete;

private:

	constexpr void release() noexcept {
		std::free(addr);
		addr = nullptr;
		aend = 0;
	}

	constexpr void reset(plmem_holder& memory) noexcept {
		memory.addr = nullptr; memory.aend = 0;
	}

	constexpr void set(pointer_t pointer, size_t size) noexcept {
		addr = pointer; aend = size;
	}

	constexpr pointer_t malloc(size_t size) noexcept {
		return static_cast<pointer_t>(std::malloc(size));
	}

	constexpr size_t next_size(size_t size) const noexcept {
		return aend + size;
	}

	constexpr pointer_t copy(size_t size) noexcept {
		pointer_t ptr = malloc(next_size(size));
		std::memcpy(ptr, addr, aend);
		return ptr;
	}

	constexpr void copy_assign(plmem_holder& memory) noexcept {
		addr = memory.addr; aend = memory.aend;
	}

	constexpr void move_assign(plmem_holder&& memory) noexcept {
		release();
		copy_assign(memory);
		reset(memory);
	}

public:

	constexpr plmem_holder()
		noexcept : addr(nullptr),
				   aend(0)
	{}

	constexpr plmem_holder(size_t size)
		noexcept : addr(malloc(size)),
				   aend(size)
	{}

	constexpr plmem_holder(size_t alloc_size, size_t init_size)
		noexcept : addr(malloc(alloc_size)),
				   aend(init_size)
	{}

	constexpr plmem_holder(plmem_holder&& memory)
		noexcept : addr(memory.addr),
				   aend(memory.aend)
	{
		reset(memory);
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

	constexpr bool failed() const noexcept {
		return addr == nullptr;
	}

	constexpr void reallocate(size_t size) noexcept {
		release();
		set(malloc(size), size);
	}

	constexpr void resize(size_t size) noexcept {
		pointer_t ptr = copy(size);
		std::free(addr);
		set(ptr, next_size(size));
	}

public:

	constexpr plmem_holder& operator=(plmem_holder&& memory) {
		move_assign(std::move(memory));
		return *this;
	}

public:

	constexpr ~plmem_holder() {
		release();
	}

};