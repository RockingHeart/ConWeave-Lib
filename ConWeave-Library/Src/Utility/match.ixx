export module utility : match;

import std;

export namespace match
{
	enum behav;

	template <typename ResultType>
	class option;

	template <class Type, typename... Kinds>
	constexpr bool   deserved(Type& resu, Kinds... kinds) noexcept;
	template <class Type, typename... Kinds>
	constexpr bool undeserved(Type& resu, Kinds... kinds) noexcept;
}

enum match::behav {
	failed,
	success
};

// It's not only used for looking up results ¡ª as the name suggests,
// it can be a match for a result.
template <typename ResultType>
class match_box_t {
public:
	using value_t           =       ResultType;
	using rvalue_t          =       ResultType&&;
	using reference_t		=		ResultType&;
	using const_reference_t = const ResultType&;

protected:

	match::behav found;
	value_t		 result;

public:

	constexpr match_box_t() noexcept = default;

	constexpr match_box_t(match::behav result, rvalue_t value)
		noexcept : found(result),
		result(std::move(value))
	{}

	constexpr match_box_t(const_reference_t result)
		noexcept : found(match::behav::success),
		result(result)
	{}

	constexpr match_box_t(rvalue_t result)
		noexcept : found(match::behav::success),
		result(std::move(result))
	{}

	constexpr match_box_t(match::behav result, const_reference_t value)
		noexcept : found(result),
		result(value)
	{}

	constexpr match_box_t(match::behav result)
		noexcept : found(result)
	{}

};

template <typename ResultType>
class match::option :
    public		    match_box_t<ResultType> {
private:
	using box_t = match_box_t<ResultType>;

public:
	using value_t           = typename box_t::value_t;
	using rvalue_t          = typename box_t::rvalue_t;
	using reference_t		= typename box_t::reference_t;
	using const_reference_t = typename box_t::const_reference_t;
	
public:
	using box_t::box_t;

public:

	[[nodiscard]]
	constexpr bool is_failed() const noexcept {
		return box_t::found == match::failed;
	}

	[[nodiscard]]
	constexpr bool is_success() const noexcept {
		return box_t::found == match::success;
	}

	constexpr reference_t value() noexcept {
		return box_t::result;
	}

public:

	[[nodiscard]]
	constexpr operator bool() const noexcept {
		return is_success();
	}
};

template <class Type, typename... Kinds>
constexpr bool match::deserved(Type& resu, Kinds... kinds) noexcept {
	auto& token = resu.value();
	return resu.is_success() && ((token.type == kinds) || ...);
}

template <class Type, typename... Kinds>
constexpr bool match::undeserved(Type& resu, Kinds... kinds) noexcept {
	auto& token = resu.value();
	return resu.is_failed() || ((token.type != kinds) && ...);
}