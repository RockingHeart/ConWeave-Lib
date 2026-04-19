export module utility : match;

import <utility>;

export namespace match {
	enum behav {
		failed,
		success
	};
}

template <typename ResultType>
class match_box_t {
public:
	using value_t           =       ResultType;
	using rvalue_t          =       ResultType&&;
	using const_reference_t = const ResultType&;

protected:

	match::behav found; value_t result;

protected:

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

export template <typename ResultType>
class match_t :
    protected match_box_t<ResultType> {
private:
	using box_t = match_box_t<ResultType>;

public:
	using value_t           = typename box_t::value_t;
	using rvalue_t          = typename box_t::rvalue_t;
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

public:

	[[nodiscard]]
	constexpr operator bool() const noexcept {
		return is_success();
	}
};