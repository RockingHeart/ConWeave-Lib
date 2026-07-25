export module symbol_table.impl.basic;

import std;
import utility;
import symbol_table.impl.box;

export template <class SymtaBox>
class basic_symtab
				 : SymtaBox {
protected:
	using box = SymtaBox;

public:
	using char_t  = typename box::char_t;
	using symbol  = typename box::symbol;
	using quicks  = typename box::quicks;
	using storage = typename box::storage;
	using state   = typename box::table_state;
	using value   = typename box::box_value;

public:

	constexpr basic_symtab()
		noexcept : box()
	{}

protected:

	constexpr void enable_storage() noexcept {
		value& val  = box::value;
		quicks temp = val.quick;
		box::reset();
		new (&val.data) storage();
		val.data.insert(temp.begin(), temp.end());
		val.state = state::storage;
	}

public:

	constexpr bool exist(symbol symbol) const noexcept {
		value& val = box::value;
		if (val.state == state::cache) {
			for (auto elem : val.quick) {
				if (elem == symbol) {
					return true;
				}
			}
			return false;
		}
		return val.data.contains(symbol);
	}

	constexpr void add(symbol symbol) noexcept {
		value& val = box::value;
		if (val.state == state::cache) {
			if (val.quick.push_back(symbol)) {
				return;
			}
			enable_storage();
		}
		val.data.insert(symbol);
	}

	template <class HandlerType>
	constexpr void for_each(HandlerType&& handler)
		noexcept requires (
			requires{ handler(symbol{}); }
		)
	{
		value& val = box::value;
		if (val.state == state::cache) {
			for (auto elem : val.quick) {
				handler(elem);
			}
			return;
		}
		for (auto elem : val.data) {
			handler(elem);
		}
	}

};