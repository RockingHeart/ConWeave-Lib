module;
#include <limits.h>

export module string.impl.box;

import string.impl.traits;
import string.impl.context;

import std;
import utility;

using namespace traits;
using namespace bitinfo;

export using ::string_mode;

template<size_t>
struct bandwidth_calculator {
	static constexpr size_t value = 6;
};

template<> struct bandwidth_calculator<1> { static constexpr size_t value = 5; };
template<> struct bandwidth_calculator<2> { static constexpr size_t value = 4; };
template<> struct bandwidth_calculator<4> { static constexpr size_t value = 3; };

export template <class StringTraits>
class string_box {
public:
	using string_traits = StringTraits;

	using char_t          = typename string_traits::char_t;
	using pointer_t       = typename string_traits::pointer_t;
	using const_pointer_t = typename string_traits::const_pointer_t;
	using size_t		  = typename string_traits::size_t;
	using value_traits	  = typename string_traits::value_traits;

	using alloc_t	   = typename string_traits::alloc_t;
	using cache_size_t = typename string_traits::cache_size_t;

protected:

	static constexpr size_t specs_max_length = 8;

private:

	struct residue_info {
		const_pointer_t const address;
		const size_t		  size;
		const size_t		  left;
	};

	struct value_concord_little {
		size_t left  : (sizeof(size_t)* CHAR_BIT) - 8;
		size_t specs : specs_max_length;
	};

	struct value_concord_big {
		size_t specs : specs_max_length;
		size_t left  : (sizeof(size_t)* CHAR_BIT) - 8;
	};


	template <endpoint EndPoint>
	struct value_concord {
		using type = std::conditional_t <
			EndPoint == endpoint::little,
			value_concord_little,
			value_concord_big
		>;
	};

	using value_concord_t = value_concord<layout_endian()>::type;

	struct base_value {
		pointer_t		pointer;
		size_t			count;
		value_concord_t concord;
	};

	struct remain_value {
		pointer_t before;
		size_t    before_count;
		size_t    before_left;
	};

	struct enhance_value : remain_value {};

	struct empty_value {};

	template <value_traits ValueTrait>
	using enhanced = std::conditional_t <
		ValueTrait == value_traits::remain,
			remain_value,
		std::conditional_t <
			ValueTrait == value_traits::enhance,
				enhance_value,
		empty_value
		>
	>;

	template <value_traits ValueTrait>
	struct box_value : base_value,
					   enhanced<ValueTrait>
	{
		using enhanced = enhanced<ValueTrait>;
	};

protected:

	using box_value_type = box_value<string_traits::value_trait>;

public:

	constexpr static size_t type_size   = sizeof(char_t);
	constexpr static size_t buffer_size = (sizeof(box_value_type) - 1) / type_size;
	constexpr static size_t bandwidth	= bandwidth_calculator<type_size>::value;

protected:

	constexpr static size_t padding_size = specs_max_length - bandwidth - 2;


	struct cache_concord_little {
		cache_size_t size	 : 5;
		cache_size_t padding : padding_size;
		string_mode  mode	 : 1;
		constexpr cache_concord_little() noexcept = default;
		constexpr cache_concord_little(cache_size_t size, string_mode mode)
			noexcept : size(size), mode(mode)
		{}
	};

	struct cache_concord_big {
		string_mode  mode	 : 1;
		cache_size_t padding : padding_size;
		cache_size_t size	 : 5;
		constexpr cache_concord_big() noexcept = default;
		constexpr cache_concord_big(cache_size_t size, string_mode mode)
			noexcept : mode(mode), size(size)
		{}
	};


	template <endpoint EndPoint>
	struct cache_concord {
		using type = std::conditional_t <
			EndPoint == endpoint::little,
			cache_concord_little,
			cache_concord_big
		>;
	};

	using cache_concord_t = cache_concord<layout_endian()>::type;

	struct specs_bits : cache_concord_t {
		using concord_t = cache_concord_t;
		using concord_t::concord_t;
		using concord_t::mode;
		using concord_t::size;
	};

	struct cache_t {
		char_t	   pointer[buffer_size];
		specs_bits specs [[indeterminate]];
	};

protected:

	union {
		cache_t        cache;
		box_value_type value;
	};

public:

	constexpr  string_box()
		noexcept : cache {}
	{};

	constexpr  string_box(char_t char_value)
		noexcept : cache {
			.pointer =   {},
			.specs   =	 {
				1, string_mode::cache
			}
		}
	{
		cache.pointer[0] = char_value;
	};

	constexpr  string_box(size_t size)
		noexcept : cache {}
	{
		if (size > buffer_size) {
			value.count		 = size;
			cache.specs.mode = string_mode::storage;
			return;
		}
		cache.specs.size = size;
	}

	constexpr ~string_box() noexcept = default;
};