export module fixed_vector.impl.basic;

import fixed_vector.impl.core;

import <cstddef>;
import <memory>;

export template <class Traits, std::size_t Size>
class basic_fixed_vec :
    private           fixed_vec_core<Traits, Size> {
private:
	using core_t = fixed_vec_core<Traits, Size>;

public:
	using value_t         = typename core_t::value_t;
	using reference_t     = typename core_t::reference_t;
	using pointer_t       = typename core_t::pointer_t;
	using const_pointer_t = typename core_t::const_pointer_t;
	using size_t          = typename core_t::size_t;
	using initlist_t      = typename core_t::initlist_t;

public:

	constexpr basic_fixed_vec() = default;

	constexpr basic_fixed_vec(size_t size)
		noexcept(noexcept(core_t::construct_vector()))
			: core_t(size)
	{
		core_t::construct_vector();
	}

	constexpr basic_fixed_vec(const initlist_t& list)
		noexcept(noexcept(core_t::construct_vector(list)))
			: core_t(list.size())
	{
		core_t::construct_vector(list);
	}

	constexpr basic_fixed_vec(const basic_fixed_vec& vec)
		noexcept(noexcept(core_t::construct_vector(vec)))
			: core_t(vec.size())
	{
		core_t::construct_vector(vec);
	}

	constexpr basic_fixed_vec(basic_fixed_vec&& vec)
		noexcept(noexcept(core_t::construct_vector(std::move(vec))))
			: core_t(vec.size())
	{
		core_t::construct_vector(std::move(vec));
	}

public:

	constexpr bool push_back(const value_t& value)
		noexcept(noexcept(core_t::push_element(value)))
	{
		return core_t::push_element(value);
	}

	constexpr bool push_back(value_t&& value)
		noexcept(noexcept(core_t::push_element(std::forward<value_t>(value))))
	{
		return core_t::push_element(std::forward<value_t&&>(value));
	}

	constexpr bool pop_back() noexcept(core_t::pop_element())
	{
		return core_t::pop_element();
	}

	constexpr bool unchcked_push_back(value_t&& value)
		noexcept(noexcept(core_t::unchcked_push_element(std::forward<value_t>(value))))
	{
		return core_t::unchcked_push_element(std::forward<value_t&&>(value));
	}

	constexpr bool unchcked_pop_back()
		noexcept(noexcept(core_t::unchcked_pop_element()))
	{
		return core_t::unchcked_pop_element();
	}

public:

	constexpr void resize(size_t size)
		noexcept(noexcept(core_t::resize_vector(0ull)))
	{
		return core_t::resize_vector(size);
	}

public:

	constexpr pointer_t begin() noexcept {
		return reinterpret_cast<pointer_t>(core_t::value.data);
	}

	constexpr const_pointer_t begin() const noexcept {
		return reinterpret_cast<const_pointer_t>(core_t::value.data);
	}

	constexpr pointer_t end() noexcept {
		return begin() + core_t::size;
	}

	constexpr const_pointer_t end() const noexcept {
		return begin() + core_t::size;
	}

public:

	constexpr size_t size() const noexcept {
		return core_t::size;
	}

	static constexpr size_t max_byte_size() noexcept {
		return core_t::max_size * sizeof(value_t);
	}

	static constexpr size_t remain() noexcept {
		return core_t::max_size - core_t::size;
	}

	constexpr static size_t max_size() noexcept {
		return core_t::max_size;
	}

public:

	constexpr reference_t at(size_t position) {
		if (position >= Size) {
			throw "Out of Range";
		}
		return core_t::pointer()[position];
	}

public:

	constexpr reference_t operator[](size_t position)
		noexcept(noexcept(core_t::pointer()))
	{
		return core_t::pointer()[position];
	}

	constexpr value_t operator[](size_t position)
		const noexcept(noexcept(core_t::pointer()))
	{
		return core_t::pointer()[position];
	}

	constexpr basic_fixed_vec& operator=(const basic_fixed_vec& vec)
		noexcept (
			noexcept(core_t::deconstruct(0, size())) &&
			noexcept(core_t::construct_vector(vec))
		)
	{
		core_t::resize_vector(0);
		size_t size = vec.size();
		core_t::construct_vector(vec);
		core_t::size = size;
		return *this;
	}

	constexpr basic_fixed_vec& operator=(basic_fixed_vec&& vec)
		noexcept (
			noexcept(core_t::deconstruct(0, size())) &&
			noexcept(core_t::construct_vector(std::move(vec)))
		)
	{
		core_t::resize_vector(0);
		size_t size = vec.size();
		core_t::construct_vector(std::move(vec));
		core_t::size = size;
		return *this;
	}

public:

	constexpr ~basic_fixed_vec()
		noexcept(noexcept(core_t::destroy_vector()))
	{
		core_t::destroy_vector();
	}

};

template <class Traits> class basic_fixed_vec<Traits, 0> {
private:
	using traits = Traits;

public:
	using value_t		  = typename traits::value_t;
	using reference_t	  = typename traits::reference_t;
	using pointer_t		  = typename traits::pointer_t;
	using const_pointer_t = typename traits::const_pointer_t;
	using size_t		  = typename traits::size_t;
	using initlist_t	  = typename traits::initlist_t;

public:

	static constexpr void push_back() noexcept {}
	static constexpr void push_back(value_t&& value) noexcept {}
	static constexpr void pop_back() {}
	static constexpr void unchcked_push_back(value_t&& value) noexcept {}
	static constexpr void unchcked_pop_back() noexcept {}
	static constexpr void begin() noexcept {}
	static constexpr void end() noexcept {}
	static constexpr void size() noexcept {}
	static constexpr void max_byte_size() noexcept {}
	static constexpr void remain() noexcept {}
	static constexpr void max_size() noexcept {}
	static constexpr void at(size_t position) noexcept {}
	static constexpr void operator[](size_t position) noexcept {}

public:

	constexpr ~basic_fixed_vec() noexcept {};
};