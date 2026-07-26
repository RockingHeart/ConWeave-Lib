export module string.impl.context;

import std;
import utility;

export template <class StringModeType>
struct string_info {
	using string_mode = StringModeType;
	string_mode modes : 1;
	bool	 is_xored : 1;
};

export enum class assoptions {
	pull, async
};

export template <assoptions, class>
struct assign_operation {};

template <rest::character char_t>
struct assign_operation<assoptions::pull, char_t> {
	using type = std::generator<char_t>;
};

template <class Type>
struct assign_operation<assoptions::async, Type> {
	
};

export template <assoptions AssignOption, class Type>
using assign_operation_t = typename assign_operation<AssignOption, Type>::type;