export module dast.symbol_table;

import symbol_table.impl;

import std;
import utility;

export namespace dast
{
	template <rest::character CharType, std::size_t BufSize = 16>
	using symbol_table = basic_symtab<symtabox<CharType, BufSize>>;
}