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
	using full_info = typename core::full_info;

public:

	constexpr basic_symtab()
		noexcept(std::is_nothrow_default_constructible_v<core>)
		: core()
	{}

protected:

	constexpr void enable_storage() 
		noexcept (
			std::is_nothrow_move_constructible_v<quicks> &&
			noexcept(std::declval<box_value&>().template emplace<storage_index> (
				std::declval<typename quicks::pointer_t>(),
				std::declval<typename quicks::pointer_t>()
			))
		)
	{
		box_value& val = core::value;
		quicks temp    = std::move(std::get<cache_index>(val));
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

	constexpr void add(symbol sym, const addal_info& info) 
		noexcept (
			noexcept (
				std::declval<quicks&>().push_back (
					std::declval<full_info>()
				)
			)
			&&
			noexcept (
				std::declval<storage&>().insert (
					std::declval<typename storage::value_type>()
				)
			)
		)
	{
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
		noexcept(
			std::is_nothrow_invocable_v<HandlerType, symbol, addal_info&>
		)
		requires (
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
	constexpr symtab_state state() const noexcept {
		return static_cast<symtab_state>(core::value.index());
	}

	[[nodiscard]]
	constexpr std::optional<addal_info> operator[](symbol sym)
		const noexcept
	{
		const box_value& val = core::value;
		return std::visit([&](const auto& container)
			noexcept -> std::optional<addal_info>
		{
			for (const auto& [key, value] : container) {
				if (key == sym) {
					return value;
				}
			}
			return std::nullopt;
		}, val);
	}

};