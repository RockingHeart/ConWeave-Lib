export module utility : adder;

template <class C, class V> concept appendable = requires(C & c, const V & v) { c.append(v); };
template <class C, class V> concept assignable = requires(C & c, const V & v) { c += v; };

export template <class Type>
class adder
{
public:
    using value_t = Type;

private:

    value_t& value;

public:

    constexpr explicit adder(value_t& v) noexcept
        : value(v) {}

    constexpr adder(const adder&)            = default;
    constexpr adder& operator=(const adder&) = delete;

public:

    template <class ValueType>
        requires appendable<value_t, ValueType>
    constexpr adder operator,(const ValueType& arg) noexcept {
        value.append(arg);
        return *this;
    }

    template <class ValueType>
        requires assignable<value_t, ValueType>
    constexpr adder operator|(const ValueType& arg) noexcept {
        value += arg;
        return *this;
    }
};