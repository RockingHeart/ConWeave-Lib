export module fixed_vector.impl.box;

import <cstddef>;
import <type_traits>;

export template <class Traits, std::size_t Size>
struct fixed_vec_box {
public:

	using traits = Traits;

	using value_t         = typename traits::value_t;
	using reference_t     = typename traits::reference_t;
	using pointer_t       = typename traits::pointer_t;
	using const_pointer_t = typename traits::const_pointer_t;
	using size_t          = typename traits::size_t;
	using initlist_t      = typename traits::initlist_t;

public:

	constexpr static size_t max_size = Size;

private:

	template <bool>
	struct box_value;

	template <>
	struct box_value<false> {
		alignas(value_t)
		std::byte data[max_size * sizeof(value_t)];
	};

	template <>
	struct box_value<true> {
		value_t data[max_size];
	};

public:

	constexpr static bool trivial_copy = std::is_trivially_copyable_v<value_t>;

protected:

	box_value<trivial_copy> value;
	size_t                size;

public:

	constexpr pointer_t pointer() noexcept {
		if constexpr (trivial_copy) {
			return value.data;
		}
		return reinterpret_cast<pointer_t>(value.data);
	}

	constexpr const_pointer_t pointer() const noexcept {
		if constexpr (trivial_copy) {
			return value.data;
		}
		return reinterpret_cast<const_pointer_t>(value.data);
	}

	constexpr  fixed_vec_box()			  noexcept : size(0) {};
	constexpr  fixed_vec_box(size_t size) noexcept : size(size) {};
	constexpr ~fixed_vec_box()			  noexcept = default;
};