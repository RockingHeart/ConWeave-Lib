export module utility : sequence_list;

import std;

export template <std::integral Type, std::size_t Begin, std::size_t End>
	requires (End >= Begin)
class sequence_list
{
public:
	using value_t = Type;

private:

	static constexpr std::size_t seqlen = End - Begin;

protected:
	using make_sequence = std::make_integer_sequence<value_t, seqlen>;
	template <value_t... Is>
	using sequence = std::integer_sequence<value_t, Is...>;

private:

	template <value_t... Is>
	static constexpr auto make_source(sequence<Is...>)
		noexcept -> std::array<value_t, seqlen>
	{
		return { static_cast<value_t>(Begin + Is)... };
	}

private:

	static constexpr std::array<value_t, seqlen>	source = make_source(make_sequence{});
	static constexpr std::span<const value_t> span{ source };

public:

	constexpr sequence_list() noexcept			   = default;
	sequence_list(const sequence_list&)			   = delete;
	sequence_list& operator=(const sequence_list&) = delete;

public:

	constexpr std::span<const value_t> list() const noexcept {
		return span;
	}

};