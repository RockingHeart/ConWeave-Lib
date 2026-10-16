export module utility : sequence_list;

import std;

export template <class Source>
class sequence_list
				  : Source
{
public:
	using source  =			 Source;
	using value_t = typename Source::value_t;

	static constexpr std::size_t seqlen = Source::length;

private:

	static constexpr std::span<const value_t> span {
		source::sequence
	};

public:

	constexpr sequence_list() noexcept			   = default;
	sequence_list(const sequence_list&)			   = delete;
	sequence_list& operator=(const sequence_list&) = delete;

	static constexpr std::span<const value_t> list() noexcept {
		return span;
	}

};

export template <std::integral Type, Type... Sequence>
struct unordered_source
{
public:
	using value_t = Type;
	static constexpr std::size_t length { sizeof...(Sequence) };
	using array_t = std::array<value_t, length>;

protected:

	static constexpr array_t sequence { Sequence... };

};

export template <std::integral Type, std::size_t Begin, std::size_t End>
	requires (End >= Begin)
class ordered_source
{
public:
	using value_t = Type;
	static constexpr std::size_t length = End - Begin;

private:

	template <std::size_t Length>
	using make_sequence = std::make_integer_sequence<Type, Length>;

	template <Type... Is>
	using sequence = std::integer_sequence<Type, Is...>;

protected:

	template <value_t... Is>
	static constexpr auto make_source(sequence<Is...>)
		noexcept -> std::array<value_t, length>
	{
		return { (Is + Begin)... };
	}

	static constexpr std::array<value_t, length> sequence = {
		make_source(make_sequence<length>{})
	};

};