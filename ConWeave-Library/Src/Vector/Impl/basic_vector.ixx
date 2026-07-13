export module vector.impl.basic;

import vector.impl.core;

import std;

export using ::vector_mode;

export template <class VectorTraits, std::size_t BufSize>
class basic_vector :
    protected      vector_core<VectorTraits, BufSize> {
private:
    using core_t = vector_core<VectorTraits, BufSize>;
    using core_t::core_t;

public:
    using value_t         = typename core_t::value_t;
    using reference_t     = typename core_t::reference_t;
    using pointer_t       = typename core_t::pointer_t;
    using const_pointer_t = typename core_t::const_pointer_t;
    using size_t          = typename core_t::size_t;

    using alloc_t      = typename core_t::alloc_t;
    using sequence_t   = typename core_t::sequence_t;
    using box_data_t   = typename core_t::box_data;
    using box_buffer_t = typename core_t::box_buffer;

public:

    constexpr basic_vector()
        noexcept = default;

    constexpr basic_vector(size_t size)
		noexcept (
			noexcept(core_t::template respace<true, 0ull>(0ull)) &&
			noexcept(core_t::construct(0ull))
        )
	{
        if constexpr (core_t::buffer_size) {
            if (size <= core_t::buffer_size) {
                core_t::value.buffer.resize(size);
                return;
            }
        }
        core_t::init_data(size);
        core_t::construct(size);
    };

    template <class Ty>
		requires (
			std::is_constructible_v<value_t, Ty> ||
			std::is_convertible_v<Ty, value_t>
        )
    constexpr basic_vector(std::initializer_list<Ty> list)
		noexcept (
			noexcept(core_t::template construct<Ty>(nullptr, nullptr, nullptr))
			&&
			noexcept(core_t::template respace<false, 0ull>(0ull))
        )
	{
        size_t size = list.size();
        if constexpr (core_t::buffer_size) {
            if (size <= core_t::buffer_size) {
                core_t::construct (
                    core_t::value.buffer.begin(),
                    list.begin(), list.end()
                );
                core_t::value.buffer.resize(size);
                return;
            }
        }
        core_t::template respace<true, 2>(size);
        core_t::construct (
            core_t::value.data.origin,
            list.begin(), list.end()
        );
    }

    constexpr basic_vector(const basic_vector& vec)
		noexcept(noexcept(core_t::construct(vec)))
	{
        core_t::construct(vec);
    }

    constexpr basic_vector(basic_vector&& vec)
        noexcept(noexcept(core_t::construct(std::move(vec))))
    {
        core_t::construct(std::move(vec));
    }

public:

    constexpr void push_back(const value_t& value)
        noexcept(noexcept(core_t::push_to_data(value_t())) &&
        noexcept(core_t::template new_space<2>()))
    {
        if constexpr (core_t::buffer_size) {
            if (core_t::value.mode == vector_mode::cache) {
                box_buffer_t& buf = core_t::value.buffer;
                if (buf.push_back(value)) {
                    return;
                }
                core_t::template heapify_cache<2>(buf.max_size());
                core_t::push_to_data(value);
                return;
            }
        }

        box_data_t& data = core_t::value.data;
        size_t data_size = static_cast<size_t> (
            data.curent - data.origin
        );
        if (data_size == data.remain) {
            core_t::template new_space<2>();
        }

        core_t::push_to_data(value);
    }

    constexpr bool pop_back()
        noexcept(noexcept(core_t::pop_out_from_data()))
    {
        return core_t::pop_out_from_data();
    }

public:

    constexpr void resize(size_t size)
		noexcept(noexcept(core_t::resize_impl(size)))
	{
        return core_t::resize_impl(size);
    }

public:

    constexpr size_t size() const noexcept {
        if constexpr (core_t::buffer_size) {
            if (core_t::value.mode == vector_mode::cache) {
                return core_t::value.buffer.size();
            }
        }

        const box_data_t& data = core_t::value.data;
        return static_cast<size_t>(data.curent - data.origin);
    }

    constexpr size_t capacity() const noexcept {
        if constexpr (core_t::buffer_size) {
            if (core_t::value.mode == vector_mode::cache) {
                return core_t::value.buffer.max_size();
            }
        }
        return core_t::value.data.remain;
    }

    static constexpr size_t bufsize() noexcept {
        return core_t::buffer_size;
    }

public:

    constexpr pointer_t begin() noexcept {
        return core_t::begin();
    }

    constexpr const_pointer_t begin() const noexcept {
        return core_t::begin();
    }

    constexpr const_pointer_t end() noexcept {
        return core_t::end();
    }

    constexpr const_pointer_t end() const noexcept {
        return core_t::end();
    }

public:

    constexpr reference_t at(size_t position) {
        if (position >= size()) {
            throw "Out of range";
        }
        return begin()[position];
    }

public:

    constexpr reference_t operator[](size_t position) noexcept {
        return begin()[position];
    }

    constexpr value_t operator[](size_t position) const noexcept {
        return begin()[position];
    }

    constexpr basic_vector& operator=(const basic_vector& vec)
		noexcept(noexcept(core_t::assign(vec)))
	{
        core_t::assign(vec);
        return *this;
    }

    constexpr basic_vector& operator=(basic_vector&& vec)
		noexcept(noexcept(core_t::assign(std::move(vec))))
	{
        core_t::assign(std::move(vec));
        return *this;
    }

public:

    constexpr ~basic_vector() noexcept {
        if constexpr (core_t::buffer_size) {
            if (core_t::value.mode == vector_mode::cache) {
                return;
            }
        }

        box_data_t& data = core_t::value.data;

        if constexpr (!core_t::trivial_copy) {
            size_t size = static_cast<size_t> (
                data.curent - data.origin
            );
            for (size_t i = 0; i < size; ++i) {
                data.origin[i].~value_t();
            }
        }

        if (data.origin) {
            core_t::allocator().deallocate (
                reinterpret_cast<sequence_t*>(data.origin),
                core_t::sesize(data.remain)
            );
        }
    }
};