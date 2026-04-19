export module utility : type_restion;

import <type_traits>;

export template<typename Type, typename... Pars>
struct is_any_of : std::disjunction<std::is_same<Type, Pars>...> {};

export template<typename Type, typename... Pars>
constexpr bool is_any_of_v = is_any_of<Type, Pars...>::value;

export template <class Type>
struct is_character {
	static constexpr bool value = is_any_of_v <
		Type, char, wchar_t, char8_t, char16_t, char32_t
	>;
};

export template <class Type>
constexpr bool is_character_v = is_character<Type>::value;

export namespace rest {

	template <class Type>
	concept character = is_character_v<Type>;

	template <typename Type>
	concept size = std::is_unsigned_v<Type>
		&& ::is_any_of_v<short, int, long, long long>;

	template <class AllocType>
	concept allocator = requires(AllocType alloc) {
		alloc.allocate;
		alloc.deallocate;
	};

}