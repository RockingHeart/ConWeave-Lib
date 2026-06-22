export module vector.impl.box;

import vector.impl.traits;

import std;

import dast.fixed_vector;

export using ::vector_mode;

export template <
    class    VectorTraits,
    typename VectorTraits::size_t BuferSize
> class vector_box {
public:

    using vector_traits = VectorTraits;

    using value_t         = typename vector_traits::value_t;
    using reference_t     = typename vector_traits::reference_t;
    using pointer_t       = typename vector_traits::pointer_t;
    using const_pointer_t = typename vector_traits::const_pointer_t;

    using alloc_t    = typename vector_traits::alloc_t;
    using size_t     = typename vector_traits::size_t;
    using sequence_t = typename vector_traits::sequence_t;

protected:

    template <size_t BufferSize>
    using fixed_vector = dast::fixed_vector<value_t, BufferSize>;

    struct box_data {
        pointer_t origin {};
        pointer_t curent {};
        size_t    remain {};
    };

    static constexpr size_t buffer_size = BuferSize == 0 ? 0 : BuferSize * sizeof(value_t)
        <= sizeof(box_data) ? sizeof(box_data) / sizeof(value_t) : BuferSize;

    using box_buffer = fixed_vector<buffer_size>;

    template <size_t buffer_size>
    struct box {
        union {
            fixed_vector<buffer_size> buffer;
            box_data                  data;
        };

        vector_mode mode;

        constexpr  box() noexcept : buffer(), mode(vector_mode::cache) {};
        constexpr ~box() noexcept {};
    };

    template <>
    struct box<0> {
        box_data data;
        constexpr  box() noexcept {};
        constexpr ~box() noexcept {};
    };

    box<buffer_size> value;

public:

    constexpr static bool trivial_copy = std::is_trivially_copyable_v<value_t>;

public:

    constexpr  vector_box() noexcept = default;
    constexpr ~vector_box() noexcept = default;
};