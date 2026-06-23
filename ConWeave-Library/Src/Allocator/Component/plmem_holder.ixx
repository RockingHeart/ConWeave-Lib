export module dast.plmem_holder;

export namespace dast
{
	class plmem_holder;
}

import std;

class dast::plmem_holder {
private:
	using size_t = std::size_t;

private:

	char*  addr;
	size_t aend;

private:

	constexpr void reset(plmem_holder& memory) noexcept {
		memory.addr = nullptr;
		memory.aend = 0;
	}

	constexpr void copy_assign(plmem_holder& memory) noexcept {
		addr = memory.addr;
		aend = memory.aend;
	}

	constexpr void move_assign(plmem_holder&& memory) noexcept {
		copy_assign(memory);
		reset(memory);
	}

public:

	constexpr plmem_holder()
		noexcept = default;

	constexpr plmem_holder(size_t size)
		noexcept : addr (
			static_cast<char*>(std::malloc(size))
		), aend(size)
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
	constexpr AddrType* address(std::size_t size) noexcept {
		return reinterpret_cast<AddrType*>(addr + size);
	}

	constexpr std::size_t size() const noexcept {
		return aend;
	}

	constexpr bool failed() const noexcept {
		return addr == nullptr;
	}

	constexpr void reallocate(size_t size) noexcept {
		addr = static_cast<char*>(std::malloc(size));
		aend = size;
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
		std::free(addr);
	}

};