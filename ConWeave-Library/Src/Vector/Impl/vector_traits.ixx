export module vector.impl.traits;

import utility;

import std;

export namespace traits
{
	template <
		class ValueType,
		template <class> class AllocatorType,
		class SizeType
	> requires (
		rest::allocator<AllocatorType<ValueType>>
	) struct vector_traits;

}

export enum class vector_mode {
	cache, storage
};

template <
	class ValueType,
	template <class> class AllocatorType,
	class SizeType = std::size_t
> requires (
	rest::allocator<AllocatorType<ValueType>>
) struct traits::vector_traits {
	
	using value_t		  = ValueType;
	using reference_t	  = ValueType&;
	using pointer_t		  = ValueType*;
	using const_pointer_t = const ValueType*;

	using size_t	 = SizeType;
	using sequence_t = std::byte;
	using alloc_t	 = AllocatorType<sequence_t>;

};