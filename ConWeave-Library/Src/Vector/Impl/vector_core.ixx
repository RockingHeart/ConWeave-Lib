export module vector.impl.core;

import vector.impl.box;

import std;

export using ::vector_mode;

export template <class VectorTraits, std::size_t BufSize>
class vector_core :
    private       VectorTraits::alloc_t,
    protected     vector_box<VectorTraits, BufSize> {
protected:
    using box_t = vector_box<VectorTraits, BufSize>;
    using box_t::box_t;

protected:
    using value_t         = typename box_t::value_t;
    using reference_t     = typename box_t::reference_t;
    using pointer_t       = typename box_t::pointer_t;
    using const_pointer_t = typename box_t::const_pointer_t;
    using size_t          = typename box_t::size_t;

    using alloc_t      = typename box_t::alloc_t;
    using sequence_t   = typename box_t::sequence_t;
    using box_data_t   = typename box_t::box_data;
    using box_buffer_t = typename box_t::box_buffer;

protected:

    constexpr alloc_t& allocator() noexcept {
        return static_cast<alloc_t&>(*this);
    }

    constexpr size_t sesize(size_t size) const noexcept {
        return size * sizeof(value_t);
    }

    constexpr void transfer(pointer_t old_ptr, pointer_t new_ptr, size_t size)
        noexcept (
			box_t::trivial_copy ||
			std::is_nothrow_move_constructible_v<value_t>
        )
    {
        if constexpr (box_t::trivial_copy) {
            std::memcpy(new_ptr, old_ptr, size * sizeof(value_t));
        }
        else {
            for (size_t i = 0; i < size; ++i) {
                new (new_ptr + i) value_t(std::move(old_ptr[i]));
            }
        }
    }

    template <std::size_t Expand = 1>
    constexpr void heapify_cache(size_t size)
        noexcept (
			noexcept(allocator().allocate(0ull)) &&
			noexcept(transfer(nullptr, nullptr, 0ull))
        )
    {
        alloc_t& alloc    = allocator();
        box_buffer_t& buf = box_t::value.buffer;
        pointer_t buf_ptr = buf.begin();
        size_t buf_size   = buf.size();
        size_t target     = std::max(size, buf_size);
        size_t new_cap    = target * Expand;

        pointer_t new_ptr = reinterpret_cast<pointer_t> (
            alloc.allocate(sesize(new_cap))
        );
        transfer(buf_ptr, new_ptr, buf_size);

        box_data_t& data = box_t::value.data;
        data.origin      = new_ptr;
        data.curent      = data.origin;
        data.remain      = new_cap;

        if constexpr (box_t::buffer_size) {
            box_t::value.mode = vector_mode::storage;
        }
    }

    template <std::size_t Expand = 1>
    constexpr void init_data(size_t size)
        noexcept(noexcept(allocator().allocate(0ull)))
    {
        alloc_t& alloc = allocator();
        size_t new_cap = size * Expand;
        pointer_t new_ptr = reinterpret_cast<pointer_t> (
            alloc.allocate(sesize(new_cap))
        );
        box_data_t& data = box_t::value.data;
        data.origin      = new_ptr;
        data.curent      = data.origin;
        data.remain      = new_cap;
        if constexpr (box_t::buffer_size) {
            box_t::value.mode = vector_mode::storage;
        }
    }

    template <std::size_t Expand>
    constexpr void init(size_t size)
        noexcept(box_t::buffer_size ?
        noexcept(heapify_cache<1>(0ull)) :
        noexcept(init_data<1>(0ull)))
    {
        if constexpr (box_t::buffer_size) {
            heapify_cache<Expand>(size);
        }
        else {
            init_data<Expand>(size);
        }
        box_data_t& data = box_t::value.data;
        data.curent     += size;
        data.remain     -= size;
    }

    template <std::size_t Expand = 1>
    constexpr void new_space(size_t size = 0)
        noexcept(noexcept(allocator().allocate(0ull)) &&
        noexcept(allocator().deallocate(nullptr, 0ull)) &&
        noexcept(transfer(nullptr, nullptr, 0ull)))
    {
        alloc_t& alloc    = allocator();
        box_data_t& data  = box_t::value.data;
        pointer_t old_ptr = data.origin;

        if constexpr (!box_t::buffer_size) {
            if (old_ptr == nullptr) {
                std::size_t sum = size + 5;
                data.origin = reinterpret_cast<pointer_t> (
                    alloc.allocate(sesize(sum))
                );
                data.curent = data.origin + size;
                data.remain = 5;
                return;
            }
        }

        size_t old_cap = data.remain;
        size_t new_cap = std::max(size, old_cap * Expand);

        pointer_t new_ptr = reinterpret_cast<pointer_t> (
            alloc.allocate(sesize(new_cap))
        );

        size_t old_size = static_cast<size_t>(data.curent - old_ptr);
        transfer(old_ptr, new_ptr, old_size);

        data.origin = new_ptr;
        data.curent = new_ptr + (!size ? old_size : size);
        data.remain = new_cap;

        alloc.deallocate (
            reinterpret_cast<std::byte*>(old_ptr),
            sesize(old_cap)
        );
    }

    template <bool IsInit, std::size_t Expand = 1>
    constexpr void respace(size_t size)
        noexcept(noexcept(allocator().allocate(0ull)) &&
        noexcept(allocator().deallocate(nullptr, 0ull)))
    {
        if constexpr (IsInit) {
            return init<Expand>(size);
        }
        else {
            return new_space<Expand>();
        }
    }

protected:

    constexpr void construct(size_t count)
        noexcept
	{
        box_data_t& data = box_t::value.data;
        for (std::size_t i = 0; i < count; i++) {
            new (data.origin + i) value_t();
        }
    }

    constexpr void construct(size_t begin, size_t end)
		noexcept
	{
        box_data_t& data = box_t::value.data;
        for (; begin < end; ++begin) {
            new (data.origin + begin) value_t();
        }
    }

    template <class Ty>
    constexpr void construct_at(pointer_t address, Ty* src)
		noexcept(std::is_nothrow_move_constructible_v<value_t>)
	{
        new (address) value_t(std::move(*src));
    }

    template <class Ty>
    constexpr void construct_at(pointer_t address, const Ty* src)
        noexcept(std::is_nothrow_copy_constructible_v<value_t>)
	{
        new (address) value_t(*src);
    }

    template <class Ty>
    constexpr void construct (pointer_t address,
							  Ty* begin,
							  Ty* end)
        noexcept (
			box_t::trivial_copy ||
			noexcept(construct_at(nullptr, begin))
        )
	{
        for (; begin != end; ++begin, ++address) {
            if constexpr (box_t::trivial_copy) {
                *address = *begin;
            }
            else {
                construct_at(address, begin);
            }
        }
    }

    template <class Ty>
    constexpr void construct_impl(Ty&& vec)
		noexcept (
			box_t::buffer_size ?
			std::is_nothrow_assignable_v<box_buffer_t, Ty> :
			noexcept(construct(nullptr, vec.begin(), vec.end()))
        )
	{
        size_t size = vec.vector_size();
        if constexpr (box_t::buffer_size) {
            if (size <= box_t::buffer_size) {
                if constexpr (std::is_rvalue_reference_v<decltype(vec)>) {
                    box_t::value.buffer = std::move(vec.value.buffer);
                }
            	else {
                    box_t::value.buffer = vec.value.buffer;
                }
                return;
            }
        }
    	respace<true, 1>(size);
        construct (
            box_t::value.data.origin,
            vec.begin(), vec.end()
        );
    }

    constexpr void construct(const vector_core& vec)
        noexcept(noexcept(construct_impl(vec)))
	{
        return construct_impl(vec);
    }

    constexpr void construct(vector_core&& vec)
        noexcept(noexcept(construct_impl(std::move(vec))))
	{
    	construct_impl(std::move(vec));
        box_data_t& data = vec.value.data;
        data.origin      = nullptr;
        data.curent      = nullptr;
    }

protected:

    template <class Ty>
    constexpr void assign_impl(Ty&& vec)
		noexcept (
			noexcept(resize_impl(0ull)) &&
			noexcept(construct<value_t>(nullptr, nullptr, nullptr))
        )
	{
        if constexpr (box_t::buffer_size) {
            if (vec.value.mode == vector_mode::cache) {
                auto& vec_buffer = vec.value.buffer;
                size_t size      = vec_buffer.size();
                resize_impl(size);
                construct (
                    begin(),
                    vec_buffer.begin(),
                    vec_buffer.end()
                );
                return;
            }
        }
        using pointer_type = std::conditional_t <
            std::is_const_v<std::remove_reference_t<Ty>>,
            const_pointer_t,
            pointer_t
        >;
        auto& vec_data           = vec.value.data;
        pointer_type data_origin = vec_data.origin;
        pointer_type data_curent = vec_data.curent;
        size_t size              = static_cast<size_t> (
            vec_data.curent - vec_data.origin
        );
        resize_impl(size);
        construct (
            begin(),
            data_origin,
            data_curent
        );
    }

    constexpr void assign(const vector_core& vec)
		noexcept(noexcept(assign_impl(vec)))
	{
        return assign_impl(vec);
    }

    constexpr void assign(vector_core&& vec)
		noexcept(noexcept(assign_impl(std::move(vec))))
	{
        return assign_impl(std::move(vec));
    }

protected:

    auto begin() noexcept -> decltype(auto) {
        if constexpr (box_t::buffer_size) {
            if (box_t::value.mode == vector_mode::cache) {
                return box_t::value.buffer.begin();
            }
        }
        return box_t::value.data.origin;
    }

    auto end() noexcept -> decltype(auto) {
        if constexpr (box_t::buffer_size) {
            if (box_t::value.mode == vector_mode::cache) {
                return box_t::value.buffer.end();
            }
        }
        return box_t::value.data.curent;
    }

protected:

    constexpr void deconstruct(size_t begin, size_t end)
		noexcept(std::is_nothrow_destructible_v<value_t>)
	{
        box_data_t& data = box_t::value.data;
        for (; begin < end; ++begin) {
            (data.origin + begin)->~value_t();
        }
    }

    constexpr void deconstruct (box_data_t& data,
								size_t begin,
								size_t end)
		noexcept(std::is_nothrow_destructible_v<value_t>)
	{
        for (; begin < end; ++begin) {
            (data.origin + begin)->~value_t();
        }
    }

    constexpr void deconstruct(pointer_t begin, pointer_t end)
		noexcept(noexcept(std::is_nothrow_destructible_v<value_t>))
	{
        for (; begin < end; ++begin) {
            begin->~value_t();
        }
    }

protected:

    constexpr size_t vector_size() const noexcept {
        if constexpr (box_t::buffer_size) {
            if (box_t::value.mode == vector_mode::cache) {
                return box_t::value.buffer.size();
            }
        }

        const box_data_t& data = box_t::value.data;
        return static_cast<size_t>(data.curent - data.origin);
    }

protected:

    constexpr void resize_buffer(size_t size) noexcept {
        if (size < box_t::buffer_size) {
            box_t::value.buffer.resize(size);
        }
        else {
            size_t old_size = box_t::value.buffer.size();
            heapify_cache<1>(size);
            construct(old_size, size);
        }
    }

    constexpr void resize_data(size_t size)
		noexcept (
			noexcept(deconstruct(nullptr, nullptr)) &&
			noexcept(new_space<0ull>(0ull))
        )
	{
        box_data_t& data = box_t::value.data;
        size_t old_size  = static_cast<size_t> (
            data.curent - data.origin
        );

        if (size < old_size) {
            deconstruct(data.origin + size, data.curent);
            data.curent = data.origin + size;
            return;
        }

        new_space<1>(size);
        construct(old_size, size);
    }

    constexpr void resize_impl(size_t size)
        noexcept (
			noexcept(resize_buffer(0ull)) &&
			noexcept(resize_data(0ull)) &&
			noexcept(construct(0ull, 0ull))
        )
    {
        if constexpr (box_t::buffer_size) {
            if (box_t::value.mode == vector_mode::cache) {
                return resize_buffer(size);
            }
        }
        return resize_data(size);
    }

protected:

    constexpr void push_to_data(const value_t& value)
        noexcept (
			box_t::trivial_copy ||
			std::is_nothrow_copy_constructible_v<value_t>
        )
    {
        box_data_t& data = box_t::value.data;
        if constexpr (box_t::trivial_copy) {
            *data.curent = value;
        }
        else {
            new (data.curent) value_t(value);
        }
        ++data.curent;
    }

    constexpr void push_to_data(value_t&& value)
        noexcept (
			box_t::trivial_copy ||
			std::is_nothrow_move_constructible_v<value_t>
        )
    {
        box_data_t& data = box_t::value.data;
        if constexpr (box_t::trivial_copy) {
            *data.curent = std::move(value);
        }
        else {
            new (data.curent) value_t(std::move(value));
        }
        ++data.curent;
    }

protected:

    constexpr bool pop_out_from_data()
        noexcept (
			box_t::trivial_copy ||
			std::is_nothrow_destructible_v<value_t>
        )
    {
        box_data_t& data = box_t::value.data;
        if (data.curent == data.origin) {
            return false;
        }
        if constexpr (!box_t::trivial_copy) {
            (data.curent - 1)->~value_t();
        }
        --data.curent;
        return true;
    }
};