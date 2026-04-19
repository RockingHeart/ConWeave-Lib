export module basic_string;

import string_traits;
import char_wrap;

import utility;
import <iterator>;
import <initializer_list>;

template <class Traits>
concept string_traits_type = requires {
	typename Traits::char_t;
	typename Traits::reference_t;
	typename Traits::pointer_t;
	typename Traits::const_pointer_t;

	Traits::value_trait;
};

template <class Core>
concept string_core_type = requires {
	Core::buffer_size;
	Core::value; Core::buffer;
};

export template <string_traits_type StringTraits, template<class, class> class StringCore>
class basic_string :
	public         StringCore<basic_string<StringTraits, StringCore>, StringTraits> {
private:
	friend class   StringCore<basic_string, StringTraits>;
	using core_t = StringCore<basic_string, StringTraits>;

private:
	static_assert(requires{ string_core_type<core_t>; });

private:
	using box_value_t  = typename core_t::box_value_type;
	using box_cache_t  = typename core_t::cache_t;

private:
	using string_info = typename core_t::string_info;

public:
	using string_traits =          StringTraits;
	using strutil       = typename string_traits::strutil;
	using alloc_t       = typename string_traits::alloc_t;

public:
	using char_t          = typename string_traits::char_t;
	using reference_t     = typename string_traits::reference_t;
	using pointer_t       = typename string_traits::pointer_t;
	using const_pointer_t = typename string_traits::const_pointer_t;
	using size_t          = typename string_traits::size_t;

public:
	using self_t = basic_string;

public:

public:

	constexpr basic_string()
		noexcept
	{};

	constexpr basic_string(char_t char_value)
		noexcept : core_t(char_value) {
	}

	constexpr basic_string(const_pointer_t str)
		noexcept : core_t(strutil::strlenof(str))
	{
		assign_init(str);
	}

	constexpr basic_string(const std::initializer_list<const char_t*>& list, char_t fill = ' ')
		noexcept : core_t(strutil::strlenof(list) + ((list.size() - 1) * fill))
	{
		assign_init(list, fill);
	}

	constexpr basic_string(const_pointer_t str, size_t size)
		noexcept : core_t(size)
	{
		assign_init(str);
	}

	constexpr basic_string(char_t char_value, size_t size)
		noexcept : core_t(size)
	{
		assign_init(char_value);
	}

	constexpr basic_string(basic_string& object)
		noexcept
	{
		assign_init(object);
	}

	constexpr basic_string(basic_string&& object)
		noexcept
	{
		object.move_string(*this);
	}

private:

	enum class char_action {
		lower,
		upper
	};

	enum class fill_action {
		left,
		center,
		right
	};

private:

	constexpr basic_string(basic_string& object, char_t value)
		noexcept
	{
		assign_init(object, &value, 1);
	}

	constexpr basic_string(basic_string& object, const_pointer_t pointer)
		noexcept
	{
		assign_init(object, pointer, strutil::strlenof(pointer));
	}

	constexpr basic_string(basic_string& object, basic_string& right_object)
		noexcept
	{
		assign_init(object, right_object.pointer(), right_object.string_length());
	}


	constexpr basic_string(basic_string& object, char_action act)
		noexcept
	{
		assign_init(object);
		shift(object, act);
	}

private:

	consteval static bool trait_is_noresidue_mode() noexcept {
		return string_traits::value_trait == traits::string_value_traits::no_residue;
	}

	consteval static bool trait_is_remain_mode() noexcept {
		return string_traits::value_trait == traits::string_value_traits::remain;
	}

	consteval static bool trait_is_enhance_mode() noexcept {
		return string_traits::value_trait == traits::string_value_traits::enhance;
	}

	consteval static bool trait_is_advanced_mode() noexcept {
		return trait_is_enhance_mode() || trait_is_remain_mode();
	}

public:

	struct reverse_iterator {
		basic_string* self;
		auto string_begin() { return std::reverse_iterator{ self->end()   }; }
		auto end()   { return std::reverse_iterator{ self->string_begin() }; }
	};

private:

	constexpr alloc_t& allocator() noexcept {
		return *reinterpret_cast<alloc_t*>(this);
	}

	constexpr size_t ration_size(size_t size) const noexcept {
		static constexpr size_t type_size = sizeof(char_t);
		if constexpr (trait_is_enhance_mode() && type_size > 1) {
			if (size % type_size != 0) {
				return (size + type_size - 1) & ~(type_size - 1);
			}
		}
		return size;
	}

	constexpr void reserve_cache(size_t size) noexcept {
		box_value_t& value = core_t::value;
		alloc_t& alloc     = allocator();
		box_cache_t& cache = core_t::cache;
		size_t buf_size    = cache.specs;
		pointer_t buffer = alloc.allocate(size);
		strutil::strcopy(buffer, cache.pointer, buf_size);
		buffer[buf_size] = char_t();
		value.pointer    = buffer;
		if constexpr (trait_is_advanced_mode()) {
			value.before = nullptr;
		}
		value.count      = buf_size;
		value.alloc_size = size;
	}

	template <class ValueType>
	constexpr void heapify_cache (alloc_t&   alloc,
		                          ValueType& value,
		                          size_t     size)
		noexcept (
			noexcept(allocator().allocate(0ull)) &&
			noexcept(allocator().deallocate(nullptr, 0ull))
		)
	{
		box_cache_t& cache = core_t::cache;
		size_t buf_size    = cache.specs;
		pointer_t buffer   = alloc.allocate(size);
		strutil::strcopy(buffer, cache.pointer, buf_size);
		buffer[buf_size]	= char_t();
		value.pointer		= buffer;
		core_t::state.modes = string_mode::storage;
		if constexpr (trait_is_advanced_mode()) {
			value.before = nullptr;
		}
	}

	template <class ValueType>
	constexpr void reserve_space (alloc_t&   alloc,
	                              ValueType& value,
		                          size_t	 size)
		noexcept (
			noexcept(allocator().allocate(0ull)) &&
			noexcept(allocator().deallocate(nullptr, 0ull))
		)
	{
		alloc.deallocate(value.before, value.before_alloc_size);
		value.before            = value.pointer;
		value.before_count      = value.count;
		value.before_alloc_size = value.alloc_size;
		value.pointer           = alloc.allocate(size);
		strutil::strcopy(value.pointer, value.before, value.before_count);
	}

	template <class ValueType>
	constexpr void new_space (alloc_t&   alloc,
		                      ValueType& value,
		                      size_t     size)
		noexcept (
			noexcept(allocator().allocate(0ull)) &&
			noexcept(allocator().deallocate(nullptr, 0ull))
		)
	{
		pointer_t old_ptr = value.pointer;
		value.pointer     = alloc.allocate(size);
		strutil::strcopy(value.pointer, old_ptr, value.count);
		alloc.deallocate(old_ptr, value.count);
	}

	template <bool init_heap = false>
	constexpr void respace(size_t size)
		noexcept (
			noexcept(allocator().allocate(0ull)) &&
			noexcept(allocator().deallocate(nullptr, 0ull))
		)
	{
		box_value_t& value = core_t::value;
		alloc_t& alloc     = allocator();
		if constexpr (trait_is_advanced_mode()) {
			return reserve_space(alloc, value, size);
		}
		if constexpr (init_heap) {
			heapify_cache(alloc, value, size);
		}
		else {
			new_space(alloc, value, size);
		}
		value.alloc_size = size;
	}

	constexpr void set_length(size_t size) noexcept {
		if (is_large_mode()) {
			core_t::value.count = size;
			return;
		}
		core_t::cache.specs = size;
	}

	constexpr size_t sublen(size_t size) noexcept {
		if (is_large_mode()) {
			core_t::value.count -= size;
			return core_t::value.count;
		}
		core_t::cache.specs -= size;
		return core_t::cache.specs;
	}

	constexpr void copy_cache(box_cache_t& object, box_cache_t& self) noexcept {
		[[assume(object.count <= bitinfo::size<7>)]]
		strutil::strcopy (
			self.pointer,
			object.pointer,
			object.count
		);
		self.count = object.count;
	}

	constexpr void copy_cache (box_cache_t&    cache,
		                       size_t          buf_size,
		                       const_pointer_t pointer,
		                       size_t          size)
		noexcept
	{
		[[assume(size <= bitinfo::size<7>)]]
		strutil::strcopy (
			cache.pointer + buf_size,
			pointer, size
		);
		cache.specs += size;
	}

	constexpr static void reset_value (box_value_t& value,
		                               char_t       char_value,
		                               size_t       size,
		                               size_t       strlen)
		noexcept
	{
		strutil::strset(value.pointer + strlen, char_value, size - strlen);
		value.count                    = size;
		value.pointer[value.count - 1] = char_t();
	}

	constexpr void swap_cache (alloc_t&     alloc,
		                       box_cache_t& self_cache,
		                       box_cache_t& object_cache)
		noexcept (
			noexcept(allocator().allocate(0ull)) &&
			noexcept(allocator().deallocate(nullptr, 0ull))
		)
	{
		size_t cache_size = self_cache.count;
		pointer_t cache   = alloc.allocate(cache_size + 1);
		strutil::strcopy(cache, self_cache.pointer, cache_size);
		cache[cache_size] = char_t();
		strutil::strcopy (
			self_cache.pointer,
			object_cache.pointer,
			object_cache.count
		);
		self_cache.count                     = object_cache.count;
		self_cache.pointer[self_cache.count] = char_t();
		strutil::strcopy(object_cache.pointer, cache, cache_size + 1);
		object_cache.count = cache_size;
		alloc.deallocate(cache, cache_size);
	}

	constexpr void swap_value (box_value_t& self_value,
		                       box_value_t& object_value)
		noexcept
	{
		pointer_t cache      = self_value.pointer;
		size_t cache_size    = self_value.count;
		self_value.pointer   = object_value.pointer;
		self_value.count     = object_value.count;
		object_value.pointer = cache;
		object_value.count   = cache_size;
	}

	constexpr void cache_swap_value (box_cache_t& self_cache,
		                             box_cache_t& object_cache,
		                             box_value_t& self_value,
		                             box_value_t& object_value)
		noexcept
	{
		pointer_t old_ptr = object_value.pointer;
		size_t old_size   = object_value.count;
		strutil::strcopy (
			object_cache.pointer,
			self_cache.pointer,
			self_cache.count
		);
		object_cache.count = self_cache.count;
		object_cache.pointer[object_cache.count] = char_t();
		object_cache.cache = true;
		self_value.pointer = old_ptr;
		self_value.count   = old_size;
		self_cache.cache   = false;
	}

private:

	constexpr void assign_init (basic_string&   object,
		                        const_pointer_t pointer,
		                        size_t          size)
		noexcept
	{
		box_cache_t& cache = core_t::cache;
		size_t obj_size    = object.string_length();
		size_t sumlen      = obj_size + size;
		if (sumlen < core_t::buffer_size) {
			strutil::strcopy(cache.pointer, object.pointer(), obj_size);
			strutil::strcopy(cache.pointer + obj_size, pointer, size);
			cache.specs = sumlen;
			return;
		}
		box_value_t& value = core_t::value;
		value.count        = sumlen;
		size_t alloc_size  = value.count * 1.5;
		value.pointer      = allocator().allocate(alloc_size);
		value.alloc_size   = alloc_size;
		strutil::strcopy(value.pointer, object.pointer(), obj_size);
		strutil::strcopy(value.pointer + obj_size, pointer, size);
		value.pointer[value.count] = char_t();
	}

	constexpr void assign_init(const_pointer_t str) noexcept {
		if (is_cache_mode()) {
			box_cache_t& cache = core_t::cache;
			size_t buf_size    = cache.specs;
			[[assume(buf_size <= bitinfo::size<7>)]]
			strutil::strcopy(cache.pointer, str, buf_size);
			return;
		}
		box_value_t& value = core_t::value;
		size_t size = value.count;
		size_t alloc_size = size * 1.5;
		value.pointer = allocator().allocate(alloc_size);
		value.alloc_size = alloc_size;
		strutil::strcopy(value.pointer, str, size);
		value.pointer[size] = char_t();
	}

	constexpr void assign_init(const std::initializer_list<const char_t*>& list, char_t fill)
		noexcept
	{
		box_cache_t& cache = core_t::cache;
		pointer_t data     = cache.pointer;
		if (is_large_mode()) {
			box_value_t& value   = core_t::value;
			size_t alloc_size    = value.count * 1.5;
			data = value.pointer = allocator().allocate(alloc_size);
			value.alloc_size     = alloc_size;
		}
		const auto* begin = list.begin();
		const auto* end   = list.end() - 1;
		for (; begin != end; ++begin) {
			size_t strlen = strutil::strlenof(*begin);
			strutil::strcopy(data, *begin, strlen);
			data += strlen;
			*data = fill;
			data += 1;
		}
		size_t strlen = strutil::strlenof(*begin);
		strutil::strcopy(data, *begin, strlen);
	}

	constexpr void assign_init(char_t char_value) noexcept {
		if (is_cache_mode()) {
			box_cache_t& cache = core_t::cache;
			size_t buf_size    = cache.specs;
			[[assume(buf_size <= bitinfo::size<7>)]]
			strutil::strset (
				cache.pointer,
				char_value,
				buf_size
			);
			return;
		}
		box_value_t& value = core_t::value;
		size_t size        = value.count;
		size_t alloc_size  = size * 1.5;
		value.pointer      = allocator().allocate(alloc_size);
		value.alloc_size   = alloc_size;
		strutil::strset(value.pointer, char_value, size);
		value.pointer[size] = char_t();
	}

	constexpr void shift(basic_string& object, char_action act) noexcept {
		if (act == char_action::upper) {
			for (auto& value : object) {
				if (value >= 'a' && value <= 'z') {
					value -= 32;
				}
			}
			return;
		}
		else {
			for (auto& value : object) {
				if (value >= 'A' && value <= 'Z') {
					value += 32;
				}
			}
		}
	}

	constexpr void assign_before (alloc_t&     alloc,
		                          box_value_t& self_value,
		                          box_value_t& object_value)
		noexcept
	{
		if constexpr (!trait_is_advanced_mode()) {
			return;
		}
		if (!object_value.before) {
			return;
		}
		self_value.before = alloc.allocate (
			object_value.before_alloc_size + 1
		);
		strutil::strcopy (
			self_value.before,
			object_value.before,
			object_value.before_count
		);
		self_value.before_count = object_value.before_count;
		self_value.before[self_value.before_count] = char_t();
	}

	constexpr void assign_data(box_value_t&  self_value, box_value_t&  object_value) noexcept {
		alloc_t& alloc            = allocator();
		size_t object_size        = object_value.count;
		size_t object_alloc_size  = object_value.alloc_size;
		self_value.pointer        = alloc.allocate (
			object_alloc_size
		);
		strutil::strcopy (
			self_value.pointer,
			object_value.pointer,
			object_size
		);
		self_value.count                     = object_size;
		self_value.alloc_size                = object_alloc_size;
		self_value.pointer[self_value.count] = char_t();
		assign_before(alloc, self_value, object_value);
	}

	constexpr void assign_init(basic_string& object) noexcept {
		if (object.is_cache_mode()) {
			return copy_cache(object.cache, core_t::cache);
		}

		assign_data(core_t::value, object.value);
	}

private:

	constexpr basic_string& assignment(const_pointer_t pointer, size_t size)
		noexcept (
			noexcept(respace<false>(0ull))
		)
	{
		if (size + 1 >= core_t::buffer_size) {
			respace<false>(size + 1);
		}
		if (is_large_mode()) {
			box_value_t& value = core_t::value;
			strutil::strcopy(value.pointer, pointer, size);
			value.count = size;
			value.pointer[value.count] = char_t();
		}
		else {
			box_cache_t& cache = core_t::cache;
			strutil::strcopy(cache.pointer, pointer, size);
			cache.specs = size;
			cache.pointer[cache.specs] = char_t();
		}
		return *this;
	}

	constexpr basic_string& assignment(char_t char_value) noexcept {
		return assignment(&char_value, 1);
	}

	constexpr basic_string& assignment(const_pointer_t pointer) noexcept {
		if (pointer == this->pointer()) {
			return *this;
		}
		return assignment(pointer, strutil::strlenof(pointer));
	}

	constexpr basic_string& assignment(basic_string& object) noexcept {
		if (std::addressof(object) == this) {
			return *this;
		}
		return assignment(object.pointer(), object.string_length());
	}

	constexpr basic_string& assignment(basic_string&& object) noexcept {
		if (std::addressof(object) == this) {
			return *this;
		}
		if constexpr (trait_is_enhance_mode()) {
			if (!object.is_empty() && !is_empty()) {
				exchange_string(object);
				return *this;
			}
		}
		object.move_string(*this);
		return *this;
	}

private:

	constexpr void swap_before(box_value_t& self, box_value_t& object) noexcept {
		if (!self.before) {
			return;
		}
		object.before            = self.before;
		object.before_count      = self.before_count;
		object.before_alloc_size = self.before_alloc_size;
		self.before = nullptr;
	}

	constexpr void move_before (alloc_t&     alloc,
								box_value_t& self,
								box_value_t& object)
		noexcept
	{
		if constexpr (!trait_is_advanced_mode()) {
			return;
		}
		release_before(alloc, object);
		swap_before(self, object);
	}

	constexpr void move_self(box_value_t& self, basic_string& object)
		noexcept (
			noexcept(allocator().deallocate(nullptr, 0ull))
		)
	{
		alloc_t& object_alloc     = object.allocator();
		box_value_t& object_value = object.value;
		if (object.is_large_mode()) {
			object_alloc.deallocate (
				object_value.pointer,
				object_value.alloc_size
			);
		}
		object_value.pointer    = self.pointer;
		object_value.count      = self.count;
		object_value.alloc_size = self.alloc_size;
		self.pointer            = nullptr;
		move_before (
			object_alloc, self, object_value
		);
	}

	constexpr void move_string(basic_string& object)
		noexcept (
			noexcept(move_self(core_t::value, object))
		)
	{
		box_cache_t& self_cache = core_t::cache;
		if (is_cache_mode()) {
			copy_cache(self_cache, object.cache);
			return;
		}
		move_self(core_t::value, object);
		self_cache.cache = true;
	}

	constexpr void exchange_self (basic_string& object,
								  alloc_t&      alloc,
								  box_cache_t&  self_cache,
								  box_value_t&  self_value)
		noexcept (
			noexcept(swap_cache(alloc, self_cache, object.cache))
		)
	{
		if (object.is_cache_mode()) {
			return swap_cache (
				alloc,
				self_cache,
				object.cache
			);
		}
		
		return cache_swap_value (
			self_cache,
			object.cache,
			self_value,
			object.value
		);
	}

	constexpr void exchange_object (basic_string& object,
									alloc_t&      alloc,
									box_cache_t&  self_cache,
									box_value_t&  self_value)
		noexcept (
			noexcept(swap_cache(alloc, self_cache, object.cache))
		)
	{
		if (object.is_cache_mode()) {
			return swap_cache (
				alloc,
				object.cache,
				self_cache
			);
		}
		
		return swap_value (
			object.value,
			self_value
		);
	}

	constexpr void exchange_string(basic_string& object)
		noexcept (
			noexcept(swap_cache(allocator(), core_t::cache, core_t::value))
		)
	{
		if (is_cache_mode()) {
			return exchange_self (
				object, allocator(),
				core_t::cache, core_t::value
			);;
		}

		return exchange_object (
			object, allocator(),
			core_t::cache, core_t::value
		);
	}

private:

	template <class OptionType>
	constexpr bool entrusted(OptionType&& option)
		noexcept requires (
		    std::is_same_v<decltype(option(char_t())), bool>
		)
	{
		for (auto& char_value : *this) {
			if (!option(char_value)) {
				return false;
			}
		}
		return true;
	}

	template <class OptionType>
	constexpr basic_string& delivered(OptionType&& option)
		noexcept requires (
		    requires {
				option(char_t());
	        }
		)
	{
		for (auto& char_value : *this) {
			option(char_value);
		}
		return *this;
	}

private:

	constexpr size_t string_length() const noexcept {
		if (is_cache_mode()) {
			return core_t::cache.specs;
		}
		return core_t::value.count;
	}

	constexpr size_t string_capacity() const noexcept {
		if (is_cache_mode()) {
			return core_t::buffer_size - core_t::cache.specs;
		}
		box_value_t& value = core_t::value;
		return value.alloc_size - value.count;
	}

	constexpr size_t string_max_size() const noexcept {
		if (is_cache_mode()) {
			return core_t::buffer_size;
		}
		return core_t::value.alloc_size;
	}

private:

	[[nodiscard]]
	constexpr bool is_large_mode() const noexcept {
		return core_t::state.modes == string_mode::storage;
	}

	[[nodiscard]]
	constexpr bool is_cache_mode() const noexcept {
		return core_t::state.modes == string_mode::cache;
	}

private:

	constexpr string_info curr_info() const noexcept {
		return string_info {
			core_t::state.modes,
			core_t::state.is_xored
		};
	}

	constexpr pointer_t pointer() noexcept {
		pointer_t data[] = {
			core_t::cache.pointer,
			core_t::value.pointer
		};
		return data[is_large_mode()];
	}

	constexpr const_pointer_t pointer() const noexcept {
		if (is_cache_mode()) {
			return core_t::cache.pointer;
		}
		return core_t::value.pointer;
	}

private:

	[[nodiscard]]
	constexpr bool within_range(size_t string_begin, size_t end) const noexcept {
		size_t size = string_length();
		[[assume(size <= end)]]
		return string_begin < size && end - string_begin <= size;
	}

	[[nodiscard]]
	constexpr bool inrange(size_t string_begin, size_t end) const noexcept {
		if (string_begin > end) {
			size_t old = string_begin;
			string_begin = end;
			end = old;
		}
		return within_range(string_begin, end);
	}

private:

	constexpr size_t count_string (
		char_t char_value, size_t point, size_t end
	) noexcept {
		if (!within_range(point, end)) {
			return 0;
		}
		size_t cont = 0;
		const_pointer_t data = pointer();
		for (; point != end; ++point) {
			if (data[point] == char_value) {
				++cont;
			}
		}
		return cont;
	}

private:

	constexpr basic_string extract_string(size_t point, size_t end) noexcept {
		if (!within_range(point, end)) {
			return {};
		}
		return { pointer() + point, end };
	}

private:

	constexpr void align_left(pointer_t data,
							 pointer_t old,
							 size_t    fill_size,
							 size_t    str_len)
		noexcept
	{
		if (old == nullptr) {
			strutil::strmove (
				data + fill_size, data, str_len
			);
			return;
		}
		strutil::strcopy(data + fill_size, old, str_len);
	}

	constexpr void align_center(pointer_t data,
							   pointer_t old,
							   size_t    fill_size,
							   size_t    str_len)
		noexcept
	{
		if (old == nullptr) {
			strutil::strmove (
				data + fill_size, data, str_len
			);
			return;
		}
		strutil::strcopy(data + fill_size, old, str_len);
	}

	constexpr void align_right(pointer_t data,
							  pointer_t old,
							  size_t    str_len)
		noexcept
	{
		if (old == nullptr) {
			return;
		}
		strutil::strcopy(data, old, str_len);
	}

	template <fill_action fill_act>
	[[noreturn]] constexpr void single_fill (pointer_t,
											 pointer_t, char_t,
											 size_t, size_t)
		noexcept = delete;

	template <>
	constexpr void single_fill<fill_action::left> (pointer_t data,
												   pointer_t old,
												   char_t    fill,
												   size_t    fill_size,
												   size_t    str_len)
		noexcept
	{
		align_left(data, old, fill_size, str_len);
		strutil::strset(data, fill, fill_size);
	}

	template <>
	constexpr void single_fill<fill_action::center> (pointer_t data,
													 pointer_t old,
													 char_t    fill,
													 size_t    fill_size,
													 size_t    str_len)
		noexcept
	{
		align_center(data, old, fill_size, str_len);
		strutil::strset(data, fill, fill_size);
		strutil::strset(data + (fill_size + str_len), fill, fill_size);
	}

	template <>
	constexpr void single_fill<fill_action::right> (pointer_t data,
													pointer_t old,
													char_t    fill,
													size_t    fill_size,
													size_t    str_len)
		noexcept
	{
		align_right(data, old, fill_size, str_len);
		strutil::strset(data + str_len, fill, fill_size);
	}

	template <fill_action fill_act>
	[[noreturn]] constexpr void multiple_fill (pointer_t, pointer_t,
											   const_pointer_t,
											   size_t, size_t)
		noexcept = delete;

	template <>
	constexpr void multiple_fill<fill_action::left> (pointer_t		 data,
													 pointer_t		 old,
													 const_pointer_t fill,
													 size_t			 fill_size,
													 size_t			 str_len)
		noexcept
	{
		align_left(data, old, fill_size, str_len);
		strutil::strcopy(data, fill, fill_size);
	}

	template <>
	constexpr void multiple_fill<fill_action::center> (pointer_t	   data,
													   pointer_t	   old,
													   const_pointer_t fill,
													   size_t		   fill_size,
													   size_t		   str_len)
		noexcept
	{
		align_center(data, old, fill_size, str_len);
		strutil::strcopy(data, fill, fill_size);
		strutil::strcopy(data + (fill_size + str_len), fill, fill_size);
	}

	template <>
	constexpr void multiple_fill<fill_action::right> (pointer_t		  data,
													  pointer_t		  old,
													  const_pointer_t fill,
													  size_t		  fill_size,
													  size_t		  str_len)
		noexcept
	{
		align_right(data, old, fill_size, str_len);
		strutil::strcopy(data + str_len, fill, fill_size);
	}

	constexpr pointer_t align_switch_data (pointer_t&   data,
		                                   size_t&      strlen,
		                                   size_t&      sumlen,
		                                   alloc_t&     alloc,
		                                   box_value_t& value)
		noexcept
	{
		bool large_mode = is_large_mode();
		if (large_mode) {
			data    = value.pointer;
			strlen  = value.count;
			value.count = sumlen;
		}
		sumlen += strlen;
		size_t max_size = large_mode ? value.alloc_size
			: core_t::buffer_size;
		if (sumlen >= max_size) {
			pointer_t old = alloc.allocate(strlen);
			strutil::strcopy(old, data, strlen);
			if (large_mode) {
				alloc.deallocate(value.pointer, value.alloc_size);
			}
			value.alloc_size     = sumlen * 1.5;
			data = value.pointer = alloc.allocate(value.alloc_size);
			data[sumlen]         = char_t();
			return old;
		}
		return nullptr;
	}

	template <fill_action fill_act, class FillType>
	constexpr void align_impl(FillType fill, size_t fill_size)
		noexcept (
			noexcept(allocator().deallocate(nullptr, 0ull))
		)
		requires (
		    std::is_same_v<FillType, const_pointer_t>
		    ||
		    std::is_same_v<FillType, char_t>
		)
	{
		box_cache_t& cache = core_t::cache;
		box_value_t& value = core_t::value;
		size_t strlen      = cache.specs;
		pointer_t data     = cache.pointer;
		size_t sumlen      = fill_size;
		if constexpr (fill_act == fill_action::center) {
			sumlen *= 2;
		}
		alloc_t alloc     = allocator();
		pointer_t address = align_switch_data (
			data, strlen, sumlen, alloc, value
		);
		if constexpr (std::is_same_v<FillType, char_t>) {
			single_fill<fill_act> (
				data, address, fill, fill_size, strlen
			);
		}
		else {
			multiple_fill<fill_act> (
				data, address, fill, fill_size, strlen
			);
		}
		if (address != nullptr) {
			alloc.deallocate(address, strlen);
		}
		set_length(sumlen);
	}

	constexpr void center_string(char_t fill, size_t size = 1)
		noexcept (
			noexcept(align_impl<fill_action::center>(fill, size))
		)
	{
		return align_impl<fill_action::center>(fill, size);
	}

	constexpr void center_string(const_pointer_t fill)
		noexcept (
			noexcept(align_impl<fill_action::center>(fill, 0ull))
		)
	{
		return align_impl<fill_action::center>(fill, strutil::strlenof(fill));
	}

	constexpr void left_string(char_t fill, size_t size = 1)
		noexcept (
			noexcept(align_impl<fill_action::left>(fill, 0ull))
		)
	{
		return align_impl<fill_action::left>(fill, size);
	}

	constexpr void left_string(const_pointer_t fill)
		noexcept (
			noexcept(align_impl<fill_action::left>(fill, 0ull))
		)
	{
		return align_impl<fill_action::left>(fill, strutil::strlenof(fill));
	}

	constexpr void right_string(char_t fill, size_t size = 1)
		noexcept (
			noexcept(align_impl<fill_action::right>(fill, 0ull))
		)
	{
		return align_impl<fill_action::right>(fill, size);
	}

	constexpr void right_string(const_pointer_t fill)
		noexcept (
			noexcept(align_impl<fill_action::right>(fill, 0ull))
		)
	{
		return align_impl<fill_action::right>(fill, strutil::strlenof(fill));
	}

private:

	constexpr void expand_cache_prefix(const_pointer_t str, size_t size)
		noexcept (
			noexcept(respace<true>(0ull))
		)
	{
		box_cache_t& cache = core_t::cache;
		pointer_t data     = cache.pointer;
		size_t bufsize     = cache.specs;
		size_t nextsize    = bufsize + size;
		if (nextsize >= core_t::buffer_size) {
			respace<true>(nextsize * 1.2);
			box_value_t& value = core_t::value;
			data               = value.pointer;
			value.count        = nextsize;
			data[nextsize]     = char_t();
		}
		else {
			cache.specs = nextsize;
		}
		strutil::strmove (
			data + size,
			data,
			bufsize
		);
		strutil::strcopy (
			data,
			str,
			size
		);
	}

	constexpr void expand_heap_prefix(const_pointer_t str, size_t size)
		noexcept (
			noexcept(respace<false>(0ull))
		)
	{
		box_value_t& value = core_t::value;
		size_t curlen      = value.count;
		size_t nextlen     = curlen + size;
		if (nextlen >= value.alloc_size) {
			respace<false>(nextlen * 1.2);
		}
		strutil::strmove (
			value.pointer + size,
			value.pointer,
			curlen
		);
		strutil::strcopy (
			value.pointer,
			str,
			size
		);
		value.count = nextlen;
		value.pointer[nextlen] = char_t();
	}

	constexpr void expand_prefix_string(const_pointer_t str)
		noexcept (
			noexcept(allocator().allocate(0ull))
		)
	{
		auto expand_prefix = is_cache_mode() ? expand_cache_prefix
			: expand_heap_prefix;

		return expand_prefix(str, strutil::strlenof(str));
	}

private:

	constexpr bool replace_string (const_pointer_t str,
		                           size_t          point,
		                           size_t          end)
		noexcept
	{
		if (std::strlen(str) > end) {
			return false;
		}
		if (!within_range(point, end)) {
			return false;
		}
		pointer_t data = pointer();
		size_t index = 0;
		for (; point < end; ++point) {
			data[point] = str[index++];
		}
		return true;
	}

	constexpr bool replace_string (reference_t char_value,
		                           size_t      point,
		                           size_t      end)
		noexcept
	{
		if (!within_range(point, end)) {
			return false;
		}
		pointer_t data = pointer();
		for (; point < end; ++point) {
			data[point] = char_value;
		}
		return true;
	}

private:

	constexpr bool insert_cache (const_pointer_t str,
		                         size_t          strlen,
		                         size_t          position)
		noexcept (
			noexcept(respace<true>(0ull))
		)
	{
		box_cache_t& cache = core_t::cache;
		size_t buflen      = cache.specs;
		if (position > buflen) {
			return false;
		}
		size_t sumlen  = buflen + strlen;
		pointer_t data = cache.pointer;
		if (sumlen >= core_t::buffer_size) {
			respace<true>(sumlen * 1.5);
			box_value_t& value = core_t::value;
			data               = value.pointer;
			value.count        = sumlen;
			data[sumlen]       = char_t();
		}
		else {
			cache.specs = sumlen;
		}
		const size_t consize = buflen - position;
		if (!consize) {
			strutil::strcopy (
				data + position,
				str,
				strlen
			);
			return true;
		}
		size_t point = position + strlen;
		strutil::strmove (
			data + point,
			data + position,
			consize
		);
		strutil::strcopy (
			data + position,
			str, strlen
		);
		return true;
	}

	constexpr bool insert_heap (const_pointer_t str,
		                        size_t          strlen,
		                        size_t          position)
		noexcept (
			noexcept(respace<false>(0ull))
		)
	{
		box_value_t& value = core_t::value;
		size_t curlen      = value.count;
		if (position > curlen) {
			return false;
		}
		size_t sumlen = curlen + strlen;
		if (sumlen >= core_t::buffer_size) {
			respace<false>(sumlen * 1.5);
		}
		const size_t consize = curlen - position;
		if (!consize) {
			strutil::strcopy (
				value.pointer + position,
				str,
				strlen
			);
			return true;
		}
		size_t point = position + strlen;
		strutil::strmove (
			value.pointer + point,
			value.pointer + position,
			consize
		);
		strutil::strcopy (
			value.pointer + position,
			str, strlen
		);
		value.count           = sumlen;
		value.pointer[sumlen] = char_t();
		return true;
	}

	constexpr bool insert_string (const_pointer_t str,
		                          size_t          position)
	    noexcept (
			noexcept(respace(0ull))
		)
	{
		size_t strlen = strutil::strlenof(str);
		if (is_cache_mode()) {
			return insert_cache(str, strlen, position);
		}
		return insert_heap(str, strlen, position);
	}

private:

	constexpr basic_string& xor_impl (basic_string& string,
									  char_t        key)
		noexcept
	{
		std::size_t size = string.string_length();
		for (size_t i = 0; i < size; ++i) {
			string[i] ^= key;
		}
		core_t::state.is_xored = true;
		return string;
	}

	constexpr basic_string& xor_string(char_t key) noexcept {
		return xor_impl(*this, key);
	}

	constexpr basic_string xor_string(char_t key) const noexcept {
		return xor_impl(basic_string{ *this }, key);
	}

private:

	constexpr bool restore_string_cache_mode(size_t size)
		noexcept (
			noexcept(allocator().allocate(0ull)) &&
			noexcept(allocator().deallocate(nullptr, 0ull))
		)
	{
		if (is_cache_mode() || size >= core_t::buffer_size) {
			return false;
		}
		if (!index_string(size)) {
			return false;
		}
		alloc_t& alloc     = allocator();
		box_value_t& value = core_t::value;
		pointer_t buffer   = alloc.allocate(size + 1);
		strutil::strcopy(buffer, value.pointer, size);
		buffer[size] = char_t();
		alloc.deallocate(value.pointer, value.alloc_size);
		box_cache_t& cache  = core_t::cache;
		cache.specs         = size;
		cache.pointer[size] = '\0';
		cache.cache         = true;
		if constexpr (trait_is_advanced_mode()) {
			release_before(alloc, value);
		}
		strutil::strcopy(cache.pointer, buffer, size);
		alloc.deallocate(buffer, size);
		return true;
	}

	constexpr bool restore_string_cache_mode()
		noexcept (
			noexcept(restore_string_cache_mode(0ull))
		)
	{
		box_value_t& value = core_t::value;
		if (value.count < core_t::buffer_size) {
			return restore_string_cache_mode(value.count);
		}
		size_t bufsize = core_t::buffer_size;
		if (!is_large_mode()) {
			return restore_string_cache_mode(bufsize);
		}
		return restore_string_cache_mode (
			bufsize * bufsize / (string_capacity() + 1)
		);
	}

	constexpr bool toggle_string_large_mode()
		noexcept (
			noexcept(allocator().allocate(0ull)) &&
			noexcept(allocator().deallocate(nullptr, 0ull))
		)
	{
		if (is_large_mode()) {
			return false;
		}
		box_cache_t& cache = core_t::cache;
		size_t strlen      = cache.specs;
		alloc_t alloc      = allocator();
		pointer_t buffer   = alloc.allocate(strlen);
		strutil::strcopy(buffer, cache.pointer, strlen);
		box_value_t& value = core_t::value;
		size_t alloc_size  = strlen * 1.5;
		value.pointer      = alloc.allocate(alloc_size);
		strutil::strcopy(value.pointer, buffer, strlen);
		value.alloc_size      = alloc_size;
		value.count           = strlen;
		value.pointer[strlen] = char_t();
		alloc.deallocate(buffer, strlen);
		return true;
	}

	constexpr void resize_string(size_t size, char_t fill = char_t())
		noexcept (
			noexcept(respace(0ull))
		)
	{
		if (is_cache_mode()) {
			if (size < core_t::buffer_size) {
				core_t::cache.pointer[size] = fill;
				return;
			}
			box_cache_t& cache = core_t::cache;
			size_t strlen      = cache.specs;
			respace<true>(size);
			reset_value(core_t::value, fill, size, strlen);
			return;
		}
		box_value_t& value = core_t::value;
		if (size < value.alloc_size) {
			value.pointer[size] = fill;
			return;
		}
		size_t strlen = value.count;
		respace<false>(size);
		reset_value(value, fill, size, strlen);
	}

	constexpr bool reserve_string(size_t size)
		noexcept (
			noexcept(respace<false>(0ull))
		)
	{
		if (is_cache_mode()) {
			if (size < core_t::cache.specs) {
				return false;
			}
			if (size > core_t::buffer_size) {
				reserve_cache(size);
			}
		}
		else {
			if (size < core_t::value.count) {
				return false;
			}
			respace<false>(size);
		}
		return true;
	}

	template <class OptionType>
	constexpr bool resize_string(size_t size, OptionType&& option)
		noexcept (
			noexcept(respace(0ull))
		)
		requires (
			std::is_same_v <
				decltype(option(core_t::value, size_t(), size_t())),
		        size_t
		    >
		)
	{
		if (is_cache_mode()) {
			if (size < core_t::buffer_size) {
				core_t::cache.pointer[size] = char_t();
				return true;
			}
			box_cache_t cache = core_t::cache;
			size_t strlen     = cache.specs;
			respace<true>(size);
			size_t resu = option(core_t::value, size, strlen);
			cache.specs = resu;
			if (cache.pointer[resu] != char_t()) {
				cache.pointer[resu]  = char_t();
			}
		}
		else {
			box_value_t& value = core_t::value;
			if (size < value.alloc_size) {
				value.pointer[size] = char_t();
				return true;
			}
			size_t strlen = value.count;
			respace<false>(size);
			size_t resu = option(value, size, strlen);
			value.count = resu;
			if (value.pointer[resu] != char_t()) {
				value.pointer[resu]  = char_t();
			}
		}
		return true;
	}

private:

	constexpr match_t<size_t> front_index (char_t target,
		                                   size_t point,
		                                   size_t end)
		noexcept
	{
		for (const_pointer_t data = pointer(); point > 0; --point) {
			if (data[point] == target) {
				return point;
			}
		}
		return match::failed;
	};

	constexpr match_t<size_t> last_index (char_t target,
		                                  size_t point,
		                                  size_t end)
		noexcept
	{
		for (const_pointer_t data = pointer(); point < end; ++point) {
			if (data[point] == target) {
				return point;
			}
		}
		return match::failed;
	}

	constexpr bool index_string(size_t position) noexcept {
		return position < string_length();
	}

	constexpr match_t<size_t> index_string (
		char_t target, size_t point, size_t end
	) noexcept {
		if (point > end) {
			return front_index(target, point, end);
		}
		return last_index(target, point, end);
	}

	constexpr reference_t at_string(size_t position) {
		if (position >= string_length()) {
			throw "Out range";
		}
		return pointer()[position];
	}

	constexpr char_t string_element(size_t position) {
		if (position >= string_length()) {
			return '\0';
		}
		return pointer()[position];
	}

private:

	template <typename CastType>
	constexpr CastType cast_to()
		noexcept (
			noexcept(CastType{ pointer_t(), size_t() })
		)
		requires (
		    requires {
		        CastType{ pointer_t(), size_t() };
	        }
		)
	{
		return { pointer(), string_length() };
	}

	template <typename CastType>
	constexpr CastType cast_to(size_t offset)
		noexcept (
			noexcept(CastType{ pointer_t(), size_t() })
		)
		requires (
		    requires {
		        CastType{ pointer_t(), size_t() };
	        }
		)
	{
		std::size_t size = string_length();
		if (offset > size) {
			throw "The out length of offset";
		}
		return { pointer() + offset, size - offset };
	}

private:

	constexpr void append_impl(char_t char_value)
		noexcept (
			noexcept(respace(0ull))
		)
	{
		if (is_cache_mode()) {
			box_cache_t& cache = core_t::cache;
			size_t buf_size    = cache.specs;
			if (buf_size + 1 < core_t::buffer_size) {
				cache.pointer[buf_size] = char_value;
				++cache.specs;
				return;
			}
			respace<true>((buf_size + 1) * 1.2);
			box_value_t& value = core_t::value;
			value.pointer[buf_size]    = char_value;
			value.count                = buf_size + 1;
			value.pointer[value.count] = char_value;
		}
		else {
			box_value_t& value = core_t::value;
			size_t& heap_count = value.count;
			size_t next_size   = heap_count + 1;
			if (next_size >= value.alloc_size) {
				respace<false>(next_size * 1.2);
			}
			value.pointer[heap_count] = char_value;
			value.count              += 1;
			value.pointer[heap_count] = char_t();
		}
	}

	constexpr void append_value (box_value_t&    value,
								 const_pointer_t pointer,
		                         size_t&         heap_count,
								 size_t          offset,
								 size_t          size,
								 size_t          next_size)
		noexcept
	{
		strutil::strcopy (
			value.pointer + offset,
			pointer, size
		);
		heap_count                = next_size;
		value.pointer[heap_count] = char_t();
	}

	constexpr void append_cache(const_pointer_t pointer, size_t size)
		noexcept (
			noexcept(respace<true>(0ull))
		)
	{
		box_cache_t& cache = core_t::cache;
		size_t buf_size    = cache.specs;
		size_t next_size   = buf_size + size;
		if (next_size < core_t::buffer_size) {
			return copy_cache (
				cache, buf_size, pointer, size
			);
		}
		respace<true>(next_size * 1.5);
		box_value_t& value = core_t::value;
		size_t& heap_count = value.count;
		return append_value (
			value, pointer, heap_count,
			buf_size, size, next_size
		);
	}

	constexpr void append_storage(const_pointer_t pointer, size_t size)
		noexcept (
			noexcept(respace<false>(0ull))
		)
	{
		box_value_t& value = core_t::value;
		size_t& heap_count = value.count;
		size_t next_size   = heap_count + size;
		if (next_size >= value.alloc_size) {
			respace<false>(next_size * 1.5);
		}
		return append_value (
			value, pointer,
			heap_count, heap_count,
			size, next_size
		);
	}

	constexpr void append_impl(const_pointer_t pointer, size_t size) noexcept {
		if (is_cache_mode()) {
			return append_cache(pointer, size);
		}
		return append_storage(pointer, size);
	}

	constexpr void append_impl(const_pointer_t pointer) noexcept {
		return append_impl(pointer, strutil::strlenof(pointer));
	}

	constexpr void append_impl(basic_string& object) noexcept {
		return append_impl(object.pointer(), object.string_length());
	}

	constexpr std::conditional_t <
		trait_is_enhance_mode(),
		adder<basic_string&>, basic_string&
	> string_append(char_t char_value) noexcept {
		append_impl(char_value);
		return *this;
	}

	constexpr std::conditional_t <
		trait_is_enhance_mode(),
		adder<basic_string&>, basic_string&
	> string_append(const_pointer_t pointer) noexcept {
		append_impl(pointer);
		return *this;
	}

	constexpr std::conditional_t <
		trait_is_enhance_mode(),
		adder<basic_string&>, basic_string&
	> string_append(basic_string& string) noexcept {
		append_impl(string);
		return *this;
	}

private:

	constexpr basic_string lower_string() noexcept {
		return { *this, char_action::lower };
	}

	constexpr basic_string upper_string() noexcept {
		return { *this, char_action::upper };
	}

private:

	constexpr pointer_t string_begin() noexcept {
		return pointer();
	}

	constexpr const_pointer_t string_begin() const noexcept {
		return pointer();
	}

	constexpr pointer_t string_end() noexcept {
		return pointer() + string_length();
	}

	constexpr const_pointer_t string_end() const noexcept {
		return pointer() + string_length();
	}

	constexpr reverse_iterator reverse_string() noexcept {
		return { this };
	}

private:

	constexpr size_t strcut(char_t char_value) noexcept {
		pointer_t data = pointer();
		size_t strlen  = string_length();
		if (!strlen) {
			return 0;
		}
		size_t count = 0;
		for (size_t i = 0; i < strlen; ++i) {
			if (data[i] != char_value) {
				break;
			}
			++count;
		}
		for (size_t i = strlen - 1; i > 0; --i) {
			if (data[i] != char_value) {
				break;
			}
			++count;
		}
		return count;
	}

	constexpr auto strcut_info (pointer_t data,
		                        size_t    strlen,
		                        char_t    char_value)
	    const noexcept
	{
		struct cut_info_t {
			size_t count, string_begin, end;
		} result{};
		for (size_t i = 0; i < strlen; ++i) {
			if (data[i] != char_value) {
				result.string_begin = i;
				break;
			}
			++result.count;
		}
		if (result.count != strlen) {
			size_t i = strlen - 1;
			for (; i > 0; --i) {
				if (data[i] != char_value) {
					result.end = i;
					break;
				}
				++result.count;
			}
		}
		return result;
	}

	constexpr size_t trimmed_string(char_t char_value = ' ') noexcept {
		size_t strlen = string_length();
		if (!strlen) {
			return 0;
		}
		pointer_t data = pointer();
		auto [count, string_begin, end] = strcut_info (
			data, strlen, char_value
		);
		data[sublen(count)] = char_t();
		if (size_t next = strlen - count; next) {
			strutil::strmove (
				data, data + string_begin, next
			);
		}
		return count;
	}

private:

	[[nodiscard]]
	constexpr bool compare(char_t char_value) const noexcept {
		if (string_length() != 1) {
			return false;
		}
		return pointer()[0] == char_value;
	}

	[[nodiscard]]
	constexpr bool compare(const_pointer_t str) const noexcept {
		size_t strlen = strutil::strlenof(str);
		size_t size   = string_length();
		if (strlen != size) {
			return false;
		}
		const_pointer_t data = pointer();
		if (data[size - 1] != str[size - 1]) {
			return false;
		}
		return strutil::strcmp(data, str, size - 1);
	}

	[[nodiscard]]
	constexpr bool is_empty() const noexcept {
		return string_length() == 0;
	}

public:

	[[nodiscard]]
	constexpr bool operator!() const noexcept {
		return !is_empty();
	}

	[[nodiscard]]
	constexpr operator const_pointer_t() const noexcept {
		return pointer();
	}

public:

	constexpr reference_t operator[](size_t position) noexcept {
		return pointer()[position];
	}

	constexpr char_t operator[](size_t position) const noexcept {
		return pointer()[position];
	}

public:

	template <class... ArgsType>
	constexpr basic_string operator+(ArgsType&&... args)
		noexcept requires (
		    requires {
		        basic_string { *this, std::forward<ArgsType>(args)... };
	        }
		)
	{
		return { *this, std::forward<ArgsType>(args)... };
	}

	template <class... ArgsType>
	constexpr basic_string& operator=(ArgsType&&... args)
		noexcept requires (
		    requires {
		        assignment(std::forward<ArgsType>(args)...);
	        }
		)
	{
		return assignment(std::forward<ArgsType>(args)...);
	}

	template <class... ArgsType>
	constexpr decltype(auto) operator+=(ArgsType&&... args)
		noexcept requires (
		    requires {
		        string_append(std::forward<ArgsType>(args)...);
	        }
		)
	{
		return string_append(std::forward<ArgsType>(args)...);
	}

	template <class... ArgsType>
	constexpr basic_string& operator^=(ArgsType&&... args)
		noexcept requires (
			requires {
				xor_string(std::forward<ArgsType>(args)...);
			}
		)
	{
		return xor_string(std::forward<ArgsType>(args)...);
	}

	template <class... ArgsType>
	constexpr bool operator==(ArgsType&&... args)
		const noexcept requires (
		    requires {
		        compare(std::forward<ArgsType>(args)...);
	        }
		)
	{
		return compare(std::forward<ArgsType>(args)...);
	}

private:

	constexpr void release_before(alloc_t& alloc, box_value_t& value) noexcept {
		if (!value.before) {
			return;
		}

		alloc.deallocate (
			value.before,
			value.before_alloc_size
		);
	}

	constexpr void release(alloc_t& alloc, box_value_t& value) noexcept {
		if constexpr (trait_is_advanced_mode()) {
			release_before(alloc, value);
		}
		alloc.deallocate(value.pointer, value.alloc_size);
	}

public:

	constexpr ~basic_string() noexcept {
		if (!is_large_mode()) {
			return;
		}

		release(allocator(), core_t::value);
	}
};