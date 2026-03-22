export module string_core;

import string_box;

import <type_traits>;

export using ::string_mode;
export using ::string_info;

export template <class BasicString, class StringTraits>
	requires (
	    requires {
	        typename StringTraits::alloc_t;
        }
	)
class string_core :
	private       StringTraits::alloc_t,
	protected     string_box<StringTraits> {
public:
	using string_traits = StringTraits;

protected:
	using box_t        =          string_box<StringTraits>;
	using box_value_t  = typename box_t::box_value_type;
	using box_cache_t  = typename box_t::cache_t;
	using box_t::box_t;

	using string_info = ::string_info;

public:
	using char_t          = typename string_traits::char_t;
	using reference_t     = typename string_traits::reference_t;
	using pointer_t       = typename string_traits::pointer_t;
	using const_pointer_t = typename string_traits::const_pointer_t;
	using size_t          = typename string_traits::size_t;

	using alloc_t = typename string_traits::alloc_t;

private:
	using basic_string =          BasicString;
	using strutil      = typename string_traits::strutil;

protected:
	using box_t::value;
	using box_t::cache;
	using box_t::buffer_size;

public:

	template <class SelfType>
	constexpr auto size(this SelfType&& self) noexcept {
		return self.string_length();
	}

	template <class SelfType>
	constexpr auto capacity(this SelfType&& self) noexcept {
		return self.string_capacity();
	}

	template <class SelfType, class... ArgsType>
	constexpr auto max_size(this SelfType&& self, ArgsType&&... args)
		noexcept requires (
		    requires {
		        self.string_max_size(std::forward<ArgsType>(args)...);
	        }
		)
	{
		return self.string_max_size(std::forward<ArgsType>(args)...);
	};

public:

	template <class SelfType>
	constexpr auto info(this SelfType&& self) {
		return self.curr_info();
	}

	template <class SelfType>
	constexpr auto const_string(this SelfType&& self) {
		return self.pointer();
	}

public:

	template <class SelfType, class... ArgsType>
	constexpr auto begin(this SelfType&& self, ArgsType&&... args)
		noexcept requires (
		    requires {
		        self.string_begin(std::forward<ArgsType>(args)...);
	        }
		)
	{
		return self.string_begin(std::forward<ArgsType>(args)...);
	}

	template <class SelfType, class... ArgsType>
	constexpr auto end(this SelfType&& self, ArgsType&&... args)
		noexcept requires (
		    requires {
		        self.string_end(std::forward<ArgsType>(args)...);
	        }
		)
	{
		return self.string_end(std::forward<ArgsType>(args)...);
	}

	template <class SelfType, class... ArgsType>
	constexpr auto cut(this SelfType&& self, ArgsType&&... args)
		noexcept requires (
		    requires {
		        self.strcut(std::forward<ArgsType>(args)...);
	        }
		)
	{
		return self.strcut(std::forward<ArgsType>(args)...);
	}

	template <class SelfType, class... ArgsType>
	constexpr auto reverse(this SelfType&& self, ArgsType&&... args)
		noexcept requires (
		    requires {
		        self.reverse_string(std::forward<ArgsType>(args)...);
	        }
		)
	{
		return self.reverse_string(std::forward<ArgsType>(args)...);
	}

	template <class SelfType, class... ArgsType>
	constexpr auto trimmed(this SelfType&& self, ArgsType&&... args)
		noexcept requires (
		    requires {
		        self.trimmed_string(std::forward<ArgsType>(args)...);
	        }
		)
	{
		return self.trimmed_string(std::forward<ArgsType>(args)...);
	}

public:

	template <class SelfType, class... ArgsType>
	constexpr auto lower(this SelfType&& self, ArgsType&&... args)
		noexcept requires (
		    requires {
		        self.lower_string(std::forward<ArgsType>(args)...);
	        }
		)
	{
		return self.lower_string(std::forward<ArgsType>(args)...);
	}

	template <class SelfType, class... ArgsType>
	constexpr auto upper(this SelfType&& self, ArgsType&&... args)
		noexcept requires (
		    requires {
		        self.upper_string(std::forward<ArgsType>(args)...);
	        }
		)
	{
		return self.upper_string(std::forward<ArgsType>(args)...);
	}

public:

	template <class SelfType, class... ArgsType>
	[[nodiscard]] constexpr auto resize(this SelfType&& self, ArgsType&&... args)
		noexcept requires (
		    requires {
		        self.resize_string(std::forward<ArgsType>(args)...);
	        }
		)
	{
		return self.resize_string(std::forward<ArgsType>(args)...);
	}

	template <class SelfType, class... ArgsType>
	[[nodiscard]] constexpr auto reserve(this SelfType&& self, ArgsType&&... args)
		noexcept requires (
		    requires {
		        self.reserve_string(std::forward<ArgsType>(args)...);
	        }
		)
	{
		return self.reserve_string(std::forward<ArgsType>(args)...);
	}

	template <class SelfType, class... ArgsType>
	[[nodiscard]] constexpr auto restore_cache_mode(this SelfType&& self, ArgsType&&... args)
		noexcept requires (
		    requires {
		        self.restore_string_cache_mode(std::forward<ArgsType>(args)...);
	        }
		)
	{
		return self.restore_string_cache_mode(std::forward<ArgsType>(args)...);
	}

	template <class SelfType, class... ArgsType>
	[[nodiscard]] constexpr auto toggle_large_mode(this SelfType&& self, ArgsType&&... args)
		noexcept requires (
		    requires {
		        self.toggle_string_large_mode(std::forward<ArgsType>(args)...);
	        }
		)
	{
		return self.toggle_string_large_mode(std::forward<ArgsType>(args)...);
	}

	template <class SelfType, class... ArgsType>
	[[nodiscard]] constexpr auto replace(this SelfType&& self, ArgsType&&... args)
		noexcept requires (
		    requires {
		        self.replace_string(std::forward<ArgsType>(args)...);
	        }
		)
	{
		return self.replace_string(std::forward<ArgsType>(args)...);
	}

	template <class SelfType, class... ArgsType>
	[[nodiscard]] constexpr auto insert(this SelfType&& self, ArgsType&&... args)
		noexcept requires (
		    requires {
		        self.insert_string(std::forward<ArgsType>(args)...);
	        }
		)
	{
		return self.insert_string(std::forward<ArgsType>(args)...);
	}

public:

	template <class SelfType, class... ArgsType>
	constexpr decltype(auto) exclusive_or(this SelfType&& self, ArgsType&&... args)
		noexcept requires (
			requires {
				self.xor_string(std::forward<ArgsType>(args)...);
			}
		)
	{
		return self.xor_string(std::forward<ArgsType>(args)...);
	}

public:

	template <class SelfType, class... ArgsType>
	[[nodiscard]] constexpr auto index(this SelfType&& self, ArgsType&&... args)
		noexcept requires (
		    requires {
		        self.index_string(std::forward<ArgsType>(args)...);
	        }
		)
	{
		return self.index_string(std::forward<ArgsType>(args)...);
	}

	template <class SelfType, class... ArgsType>
	constexpr decltype(auto) at(this SelfType&& self, ArgsType&&... args)
		requires (
		    requires {
		        self.at_string(std::forward<ArgsType>(args)...);
	        }
		)
	{
		return self.at_string(std::forward<ArgsType>(args)...);
	}

	template <class SelfType, class... ArgsType>
	constexpr decltype(auto) element(this SelfType&& self, ArgsType&&... args)
		noexcept requires (
		    requires {
		        self.string_element(std::forward<ArgsType>(args)...);
	        }
		)
	{
		return self.string_element(std::forward<ArgsType>(args)...);
	}

public:

	template <class SelfType, class... ArgsType>
	constexpr auto swap(this SelfType&& self, ArgsType&&... args)
		noexcept requires (
		    requires {
		        self.exchange_string(std::forward<ArgsType>(args)...);
	        }
		)
	{
		return self.exchange_string(std::forward<ArgsType>(args)...);
	}

	template <class SelfType, class... ArgsType>
	constexpr auto move(this SelfType&& self, ArgsType&&... args)
		noexcept requires (
		    requires {
		        self.move_string(std::forward<ArgsType>(args)...);
	        }
		)
	{
		return self.move_string(std::forward<ArgsType>(args)...);
	}

public:

	template <class SelfType, typename CastType, class... ArgsType>
	constexpr decltype(auto) to(this SelfType&& self, ArgsType&&... args)
		noexcept requires (
		    requires {
		        self.template cast_to<CastType>(std::forward<ArgsType>(args)...);
	        }
		)
	{
		return self.template cast_to<CastType>(std::forward<ArgsType>(args)...);
	}

	template <class SelfType>
	constexpr decltype(auto) swapcase(this SelfType&& self) noexcept {
		return self.delivered (
			[](reference_t value) constexpr noexcept {
			    if (value >= 'A' && value <= 'Z') {
				    value += 32;
			    }
				else
				if (value >= 'a' && value <= 'z') {
					value -= 32;
				}
		    }
		);
	}

	template <class SelfType>
	constexpr decltype(auto) to_lower(this SelfType&& self) noexcept {
		return self.delivered (
			[](reference_t value) constexpr noexcept {
			    if (value >= 'A' && value <= 'Z') {
				    value += 32;
			    }
		    }
		);
	}

	template <class SelfType>
	constexpr decltype(auto) to_upper(this SelfType&& self) noexcept {
		return self.delivered (
			[](reference_t value) constexpr noexcept {
			    if (value >= 'a' && value <= 'z') {
				    value -= 32;
			    }
		    }
		);
	}

public:

	template <class SelfType, class... ArgsType>
	constexpr auto tick(this SelfType&& self, ArgsType&&... args)
		noexcept requires (
		    requires {
		        self.count_string(std::forward<ArgsType>(args)...);
	        }
		)
	{
		return self.count_string(std::forward<ArgsType>(args)...);
	}

public:

	template <class SelfType, class... ArgsType>
	constexpr auto extract(this SelfType&& self, ArgsType&&... args)
		noexcept requires (
		    requires {
		        self.extract_string(std::forward<ArgsType>(args)...);
	        }
		)
	{
		return self.extract_string(std::forward<ArgsType>(args)...);
	}

public:

	template <class SelfType, class... ArgsType>
	constexpr void append(this SelfType&& self, ArgsType&&... args)
		noexcept requires (
			requires {
				(self.append_impl(std::forward<ArgsType>(args)), ...);
			}
		)
	{
		(self.append_impl(std::forward<ArgsType>(args)), ...);
	}

	template <class SelfType, class... ArgsType>
	constexpr void center(this SelfType&& self, ArgsType&&... args)
		noexcept requires (
		    requires {
		        self.center_string(std::forward<ArgsType>(args)...);
	        }
		)
	{
		return self.center_string(std::forward<ArgsType>(args)...);
	}

	template <class SelfType, class... ArgsType>
	constexpr auto left(this SelfType&& self, ArgsType&&... args)
		noexcept requires (
		    requires {
		        self.left_string(std::forward<ArgsType>(args)...);
	        }
		)
	{
		return self.left_string(std::forward<ArgsType>(args)...);
	}

	template <class SelfType, class... ArgsType>
	constexpr auto right(this SelfType&& self, ArgsType&&... args)
		noexcept requires (
		    requires {
		        self.right_string(std::forward<ArgsType>(args)...);
	        }
		)
	{
		return self.right_string(std::forward<ArgsType>(args)...);
	}

public:

	template <class SelfType, class... ArgsType>
	constexpr auto expand_prefix(this SelfType&& self, ArgsType&&... args)
		noexcept requires (
		    requires {
		        self.expand_prefix_string(std::forward<ArgsType>(args)...);
	        }
		)
	{
		return self.expand_prefix_string(std::forward<ArgsType>(args)...);
	}

public:

	template <class SelfType>
	[[nodiscard]] constexpr auto empty(this SelfType&& self) noexcept {
		return self.is_empty();
	}

	template <class SelfType>
	[[nodiscard]] constexpr auto is_blank(this SelfType&& self) noexcept {
		return self.entrusted([](char_t value) constexpr noexcept {
			if (value == ' ') {
				return true;
			}
			return false;
		});
	}

	template <class SelfType>
	[[nodiscard]] constexpr auto is_digit(this SelfType&& self) noexcept {
		return self.entrusted([](char_t value) constexpr noexcept {
			if (value >= '0' && value <= '9') {
				return true;
			}
			return false;
		});
	}

	template <class SelfType>
	[[nodiscard]] constexpr auto is_lower(this SelfType&& self) noexcept {
		return self.entrusted([](char_t value) constexpr noexcept {
			if (value >= 'a' && value <= 'z') {
				return true;
			}
			return false;
		});
	}

	template <class SelfType>
	[[nodiscard]] constexpr auto is_upper(this SelfType&& self) noexcept {
		return self.entrusted([](char_t value) constexpr noexcept {
			if (value >= 'A' && value <= 'Z') {
				return true;
			}
			return false;
		});
	}

	template <class SelfType>
	[[nodiscard]] constexpr auto is_enstr(this SelfType&& self) noexcept {
		return self.entrusted([](char_t value) constexpr noexcept {
			if (value >= 'A' && value <= 'Z') {
				return true;
			}
			if (value >= 'a' && value <= 'z') {
				return true;
			}
			return false;
		});
	}

	template <class SelfType>
	[[nodiscard]] constexpr auto is_ascii(this SelfType&& self) noexcept {
		return self.entrusted([](char_t value) constexpr noexcept {
			return (value & 0x80) == 0;
		});
	}

public:

	template <class SelfType>
	constexpr auto residue(this SelfType&& self)
	    noexcept requires (
		    requires {
		        self.value.before;
	        }
		)
	{
		return typename box_t::residue_info {
			self.value.before,
			self.value.before_count,
			self.value.before_alloc_size
		};
	}

	template <class SelfType>
	[[nodiscard]] constexpr auto leave_residue(this SelfType&& self)
		noexcept requires (
		    requires {
		        self.value.before;
	        }
		)
	{
		if (self.is_cache_mode()) {
			return false;
		}
		if (!self.value.before) {
			return false;
		}
		return true;
	}

	template <class SelfType>
	[[nodiscard]] constexpr auto clear_residue(this SelfType&& self)
		noexcept requires (
		    requires {
		        self.value.before;
	        }
		)
	{
		if (!self.leave_residue()) {
			return false;
		}
		auto& value = self.value;
		self.allocator().deallocate (
			value.before, value.before_alloc_size
		);
		value.before = nullptr;
		return true;
	}
};