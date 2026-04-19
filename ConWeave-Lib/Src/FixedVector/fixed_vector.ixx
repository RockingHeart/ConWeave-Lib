export module fixed_vector;

import <cstddef>;

import fixed_vec_traits;
import basic_fixed_vec;

export namespace dast {
	template <class ValueType, std::size_t Size, class SizeType = std::size_t>
	using fixed_vector = basic_fixed_vec<traits::fixed_vec_traits<ValueType, SizeType>, Size>;
}