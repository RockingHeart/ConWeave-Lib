export module dast.plain_memory;

export namespace dast
{
	class plain_memory;
}

import std;

class dast::plain_memory {
private:
	using size_t = std::size_t;

private:

	char*  addr;
	size_t aend;

private:

	constexpr void reset(plain_memory& memory) noexcept {
		memory.addr = nullptr;
		memory.aend = 0;
	}

	constexpr void copy_assign(plain_memory& memory) noexcept {
		addr = memory.addr;
		aend = memory.aend;
	}

	constexpr void move_assign(plain_memory&& memory) noexcept {
		copy_assign(memory);
		reset(memory);
	}

public:

	constexpr plain_memory()
		noexcept = default;

	constexpr plain_memory(size_t size)
		noexcept : addr (
			static_cast<char*>(std::malloc(size))
		), aend(size)
	{}

	constexpr plain_memory(plain_memory& memory)
		noexcept : addr(memory.addr), aend(memory.aend)
	{}

	constexpr plain_memory(plain_memory&& memory)
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

	constexpr void operator=(plain_memory& memory) {
		copy_assign(memory);
	}

	constexpr void operator=(plain_memory&& memory) {
		move_assign(std::move(memory));
	}

public:

	constexpr ~plain_memory() {
		std::free(addr);
	}

};