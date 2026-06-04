export module dast.vector;

import vector.impl;

import <cstddef>;
import <memory>;

export using ::vector_mode;

export namespace dast
{
	template <class ValueType, std::size_t BufferSize = 0>
	using vector = basic_vector<traits::vector_traits<ValueType, std::allocator>, BufferSize>;
}