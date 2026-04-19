export module utility : bitinfo;

import <cstddef>;

export namespace bitinfo {

	template <std::size_t bits>
	constexpr size_t size = bits > 64 ? 0xffffffffffffffffu :
		(1 << bits) - 1;

	template <std::size_t position, class ObjectType>
	constexpr bool at(const ObjectType& object) noexcept;

	template <std::size_t position, class ObjectType>
	constexpr void set(ObjectType& object, bool level) noexcept;
}

template <std::size_t position, class ObjectType>
constexpr bool bitinfo::at(const ObjectType& object) noexcept {
	return (object >> position) & 1;
}

template <std::size_t position, class ObjectType>
constexpr void bitinfo::set(ObjectType& object, bool level) noexcept {
	if (level)
		object |=  (1 << position);
	else
		object &= ~(1 << position);
}