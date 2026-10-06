export module string.impl.context;

import std;
import utility;

export template <class StringModeType>
struct string_info {
	using string_mode = StringModeType;
	string_mode modes : 1;
	bool	 is_xored : 1;
};

export enum class string_mode : bool {
	cache, storage
};