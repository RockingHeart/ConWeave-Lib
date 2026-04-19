export module string_box;

import string_traits;

using namespace traits;

export using ::string_mode;

export struct string_info {
	string_mode modes : 1;
	bool	 is_xored : 1;
};

export template <class StringTraits>
class string_box {
public:

	using string_traits = StringTraits;

	using char_t          = typename string_traits::char_t;
	using pointer_t       = typename string_traits::pointer_t;
	using const_pointer_t = typename string_traits::const_pointer_t;
	using size_t		  = typename string_traits::size_t;
	using value_traits	  = typename string_traits::value_traits;

	using cache_size_t = typename string_traits::cache_size_t;

private:

	template <value_traits>
	struct box_value_t;

	template <>
	struct box_value_t<value_traits::no_residue>
	{
		pointer_t pointer;
		size_t    count;
		size_t    alloc_size;
	};

	template <>
	struct box_value_t<value_traits::remain>
	{
		pointer_t pointer;
		size_t    count;
		size_t    alloc_size;
		pointer_t before;
		size_t    before_count;
		size_t    before_alloc_size;
	};

	template <>
	struct box_value_t<value_traits::enhance>
		:  box_value_t<value_traits::remain>
	{};

	struct residue_info {
		const_pointer_t address;
		const size_t    size;
		const size_t    alloc_size;
	};

private:

	static constexpr size_t bandwidth_of_specs() noexcept {
		switch (type_size) {
			case 1:
				return 5;
			case 2:
				return 4;
			case 4:
				return 3;
		}
		return 7;
	}

public:

	using box_value_type = box_value_t<string_traits::value_trait>;

	constexpr static size_t type_size   = sizeof(char_t);
	constexpr static size_t buffer_size = (sizeof(box_value_type) - 1) / type_size;
	static constexpr size_t bandwidth	= bandwidth_of_specs();

protected:

	struct cache_t {
		char_t pointer[buffer_size];
		cache_size_t specs [[indeterminate]] : bandwidth;
	};

	class status {
		char_t		 fill_buffer[sizeof(pointer_t) + sizeof(size_t)];
		size_t		 fill_allocsize;
		cache_size_t fill_specs : bandwidth;
	public:
		string_mode modes : 1 = string_mode::cache;
		bool	 is_xored : 1 = false;
		constexpr status(string_mode mode) noexcept : modes(mode) {}
	};

	union {
		cache_t        cache;
		box_value_type value;
		status		   state;
	};

public:

	constexpr  string_box()
		noexcept : cache {
			.pointer {},
			.specs = 0
		}
	{};

	constexpr  string_box(char_t char_value)
		noexcept : cache {
			.pointer {},
			.specs = 1
		}
	{
		cache.pointer[0] = char_value;
	};

	constexpr  string_box(size_t size)
		noexcept : state {
			string_mode::cache
		}
	{
		size >= buffer_size ? void([this, size] {
			value.count = size;
			state.modes = string_mode::storage;
		}()) : void(cache = cache_t {
			.specs = static_cast<cache_size_t>(size)
		});
	};

	constexpr ~string_box() noexcept = default;
};