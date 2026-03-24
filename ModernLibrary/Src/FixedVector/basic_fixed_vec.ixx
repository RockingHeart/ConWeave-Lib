export module basic_fixed_vec;

import fixed_vec_core;

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

	constexpr basic_fixed_vec() noexcept = default;

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

public:

	constexpr bool push_back(const value_t& value)
		noexcept(noexcept(core_t::push_element(value)))
	{
		return core_t::push_element(value);
	}

	constexpr bool push_back(value_t&& value)
		noexcept(noexcept(core_t::push_element(std::forward<value_t&&>(value))))
	{
		return core_t::push_element(std::forward<value_t&&>(value));
	}

	constexpr bool pop_back() noexcept(core_t::pop_element())
	{
		return core_t::pop_element();
	}

public:

	constexpr pointer_t begin() noexcept {
		return core_t::value.data;
	}

	constexpr const_pointer_t begin() const noexcept {
		return core_t::value.data;
	}

	constexpr pointer_t end() noexcept {
		return core_t::value.data + core_t::size;
	}

	constexpr const_pointer_t end() const noexcept {
		return core_t::value.data + core_t::size;
	}

public:

	constexpr size_t size() const noexcept {
		return core_t::size;
	}

	constexpr size_t capacity() const noexcept {
		return core_t::max_size - core_t::size;
	}

	constexpr static size_t max_size() noexcept {
		return Size;
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

public:

	constexpr ~basic_fixed_vec()
		noexcept(core_t::destroy_vector())
	{
		core_t::destroy_vector();
	}

};