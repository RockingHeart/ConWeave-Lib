export module dast.symbol_table;

import symbol_table.impl;

import std;
import utility;

export using ::table_state;

export namespace dast
{
	template <rest::character CharType, class AddalType, std::size_t BufSize = 16>
	using symbol_table = basic_symtab<symtab_core<CharType, AddalType, BufSize>>;
}