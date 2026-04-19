export module string_traits;

import utility;
import <cstddef>;

template <class Utility>
concept string_utility_type = requires(Utility util) {
	util.strcopy;
	util.strset;
	util.strcmp;
	util.strlenof;
};

export enum class string_mode : bool {
	cache, storage
};

export namespace traits {

	enum class string_value_traits;

	template <
		rest::character CharType, string_value_traits StringValueTrait,
		template <rest::character, class> class StringUtility,
		rest::allocator AllocatorType, class SizeType
	> requires (
		is_character_v<typename AllocatorType::value_type>
		&&
		string_utility_type<StringUtility<CharType, SizeType>>
	) struct string_traits;

};

enum class traits::string_value_traits {
	no_residue,
	remain,
	enhance
};

template <
	rest::character CharType, traits::string_value_traits StringValueTrait,
	template <rest::character, class> class StringUtility,
	rest::allocator AllocatorType, class SizeType = std::size_t
> requires (
	is_character_v<typename AllocatorType::value_type>
	&&
	string_utility_type<StringUtility<CharType, SizeType>>
) struct traits::string_traits {
	using char_t          =       CharType;
	using reference_t     =       CharType&;
	using pointer_t       =       CharType*;
	using const_pointer_t = const CharType*;

	using size_t = SizeType;

	using alloc_t	   = AllocatorType;
	using strutil	   = StringUtility<CharType, SizeType>;
	using value_traits = string_value_traits;

	using cache_size_t = unsigned char;

	constexpr static value_traits value_trait = StringValueTrait;
};