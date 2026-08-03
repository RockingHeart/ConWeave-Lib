export module utility : bitinfo;

import std;

export namespace bitinfo
{
	template <std::size_t bits>
	constexpr size_t size =
		bits >= 64 ? ~std::size_t{ 0 } :
		bits == 0  ? 0				   :
		(std::size_t{ 1 } << (bits - 1) << 1) - 1;

	template <std::size_t position, class ObjectType>
	constexpr bool at(const ObjectType& object) noexcept;

	template <std::size_t position, class ObjectType>
	constexpr void set(ObjectType& object, bool level) noexcept;

	enum class endpoint : bool;

	constexpr endpoint layout_endian() noexcept;
}

using namespace bitinfo;

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

enum class bitinfo::endpoint : bool {
	little, big
};

constexpr endpoint bitinfo::layout_endian() noexcept {
	struct layout {
		std::uint8_t low : 4;
		std::uint8_t hig : 4;
	};

	static constexpr layout bits{
		0b1111, 0b0000
	};

	auto byte = std::bit_cast<std::uint8_t>(bits);
	return static_cast<endpoint>(byte != 0x0F);
}