export module symbol_table.impl.core;

import std;
import utility;

import dast.fixed_vector;

export enum class symtab_state : std::size_t {
	cache, storage
};

export template <
	rest::character CharType,
	class AddalInfo,
	std::size_t BufSize
>
class symtab_core {
	static_assert(BufSize != 0, "BufSize Can't be zero");
public:
	using char_t	 =		CharType;
	using symbol	 =  std::basic_string_view<char_t>;
	using addal_info =		 AddalInfo;
	using full_info  =  std::pair<symbol, addal_info>;
	using quicks	 = dast::fixed_vector<full_info, BufSize>;
	using storage	 =  std::unordered_map<symbol, addal_info>;

protected:

	static constexpr std::size_t cache_index   = static_cast<std::size_t>(symtab_state::cache);
	static constexpr std::size_t storage_index = static_cast<std::size_t>(symtab_state::storage);

protected:

	using box_value = std::variant<quicks, storage>;
		  box_value value;

public:

	constexpr symtab_core()
		noexcept (
			std::is_nothrow_constructible_v<quicks>
		) : value(std::in_place_index<cache_index>)
	{}

	constexpr ~symtab_core() noexcept = default;

};