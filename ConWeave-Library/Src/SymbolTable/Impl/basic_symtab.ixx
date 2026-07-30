export module symbol_table.impl.basic;

import std;
import utility;
import symbol_table.impl.core;

export template <class SymtaCore>
class basic_symtab
	: SymtaCore {
protected:
	using core = SymtaCore;
	using core::cache_index;
	using core::storage_index;

public:
	using char_t = typename core::char_t;
	using symbol = typename core::symbol;
	using addal_info = typename core::addal_info;

private:
	using quicks	= typename core::quicks;
	using storage	= typename core::storage;
	using box_value = typename core::box_value;

public:

	constexpr basic_symtab()
		noexcept : core()
	{}

protected:

	constexpr void enable_storage() noexcept {
		box_value& val = core::value;
		quicks temp = std::get<cache_index>(val);
		val.template emplace<storage_index>(
			temp.begin(), temp.end()
		);
	}

public:

	constexpr bool exist(symbol sym) const noexcept {
		const box_value& val = core::value;
		return std::visit([&](const auto& container) {
			return container.contains(sym);
		}, val);
	}

	constexpr void add(symbol sym, const addal_info& info) noexcept {
		box_value& val = core::value;
		if (auto* quick_ptr = std::get_if<cache_index>(&val)) {
			if (quick_ptr->push_back({ sym, info })) {
				return;
			}
			enable_storage();
		}
		std::get<storage_index>(val).insert({ sym, info });
	}

	template <class HandlerType>
	constexpr void for_each(HandlerType&& handler)
		noexcept requires (
			std::is_invocable_v <
				HandlerType,
				symbol, addal_info&
			>
		)
	{
		std::visit([&](auto& container) {
			for (auto& [key, value] : container) {
				handler(key, value);
			}
		}, core::value);
	}

	[[nodiscard]]
	constexpr match_result<addal_info> operator[](symbol sym)
		const noexcept
	{
		const box_value& val = core::value;
		return std::visit([&](const auto& container)
			-> match_result<addal_info>
		{
			for (const auto& [key, value] : container) {
				if (key == sym) {
					return value;
				}
			}
			return match::failed;
		}, val);
	}

};