export module dast.plmem_holder;

export namespace dast
{
	class plmem_holder;
}

import std;

class dast::plmem_holder {
public:
	using size_t	= std::size_t;
	using pointer_t = char*;

private:

	pointer_t addr;
	size_t	  aend;

private:

	constexpr void reset(plmem_holder& memory) noexcept {
		memory.addr = nullptr;
		memory.aend = 0;
	}

	constexpr void set(void* pointer, size_t size) noexcept {
		addr = static_cast<pointer_t> (
			pointer
		);
		aend = size;
	}

	constexpr void copy_assign(plmem_holder& memory) noexcept {
		addr = memory.addr;
		aend = memory.aend;
	}

	constexpr void move_assign(plmem_holder&& memory) noexcept {
		copy_assign(memory);
		reset(memory);
	}

private:

	constexpr void release() noexcept {
		std::free(addr);
		addr = nullptr;
	}

public:

	constexpr plmem_holder()
		noexcept = default;

	constexpr plmem_holder(size_t size)
		noexcept : addr (
			static_cast<pointer_t>(std::malloc(size))
		), aend(size)
	{}

	constexpr plmem_holder(size_t alloc_size, size_t init_size)
		noexcept : addr (
			static_cast<pointer_t>(std::malloc(alloc_size))
		), aend(init_size)
	{}

	constexpr plmem_holder(plmem_holder& memory)
		noexcept : addr(memory.addr), aend(memory.aend)
	{}

	constexpr plmem_holder(plmem_holder&& memory)
		noexcept : addr(memory.addr), aend(memory.aend)
	{
		reset(memory);
	}

public:

	template <class AddrType = char>
	constexpr AddrType* address(std::size_t offset) noexcept {
		return reinterpret_cast<AddrType*> (
			reinterpret_cast<char*>(addr) + offset
		);
	}

	constexpr std::size_t size() const noexcept {
		return aend;
	}

	constexpr bool failed() const noexcept {
		return addr == nullptr;
	}

	constexpr void reallocate(size_t size) noexcept {
		release();
		set(std::malloc(size), size);
	}

public:

	constexpr void operator=(plmem_holder& memory) {
		copy_assign(memory);
	}

	constexpr void operator=(plmem_holder&& memory) {
		move_assign(std::move(memory));
	}

public:

	constexpr ~plmem_holder() {
		release();
	}

};