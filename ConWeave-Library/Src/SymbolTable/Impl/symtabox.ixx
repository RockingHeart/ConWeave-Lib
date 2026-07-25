export module symbol_table.impl.box;

import std;
import utility;

import dast.fixed_vector;

export template <rest::character CharType, std::size_t BufSize>
class symtabox {
	static_assert(BufSize != 0, "BufSize Can't be zero");
public:
	using char_t  =		 CharType;
	using symbol  =  std::basic_string_view<char_t>;
	using quicks  = dast::fixed_vector<symbol, BufSize>;
	using storage =  std::unordered_set<symbol>;

protected:

	enum class table_state : std::size_t {
		cache, storage
	};

	struct box_value {
		union {
			quicks  quick;
			storage data;
		};
		table_state state;
		constexpr  box_value()
			noexcept : quick(),
					   state(table_state::cache)
		{};
		constexpr ~box_value() noexcept {};
	};

	box_value value;

protected:

	constexpr void reset() noexcept {
		if (value.state == table_state::cache) {
			value.quick.~quicks();
			return;
		}
		value.data.~storage();
	}

public:

	constexpr  symtabox()
		noexcept : value() {};

	constexpr ~symtabox() noexcept {
		reset();
	}

};