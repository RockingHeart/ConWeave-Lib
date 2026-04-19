export module utility : adder;

export template <class Type>
class adder {

private:
	Type& value;

public:

	constexpr adder(Type& value)
		noexcept requires (
			requires {
				value.append;
				value.operator+=;
			}
		) : value(value)
	{}

public:

	template <class ValueType>
	constexpr adder operator,(const ValueType& arg) noexcept {
		void(value.append(arg));
		return value;
	}

	template <class ValueType>
	constexpr adder operator|(const ValueType& arg) noexcept {
		void(value += arg);
		return value;
	}
};