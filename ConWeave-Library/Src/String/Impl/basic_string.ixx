export module string.impl.basic;

import string.impl.traits;
import string.impl.context;
import char_wrap;

import dast.vector;

import utility;
import std;

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
	using box_value_t = typename core_t::box_value_t;
	using box_cache_t = typename core_t::box_cache_t;
	using box_specs_t = typename core_t::box_specs_t;

private:
	using string_info   = typename string_info<::string_mode>;

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

private:

	template <assoptions AssignOption>
	using option_t = std::conditional_t <
		AssignOption == assoptions::pull,
		char_t,
		void
	>;

	template <assoptions AssignOption>
	using assign_option_t = assign_operation_t<AssignOption, option_t<AssignOption>>;

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
		std::forward<basic_string&&>(object).move_string(*this);
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

	struct string_range {
		const_pointer_t data;
		size_t			size;
	};

private:

	constexpr alloc_t& allocator() noexcept {
		return core_t::memalloc;
	}

	constexpr std::size_t alloc_size() const noexcept {
		const box_value_t& value = core_t::value;
		return value.count + value.concord.left;
	}

	constexpr std::size_t alloc_size(box_value_t& value) const noexcept {
		return value.count + value.concord.left;
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

	template <size_t Expand>
	constexpr void heapify_cache (alloc_t&     alloc,
								  box_value_t& value,
		                          size_t       size)
		noexcept (
			noexcept(allocator().allocate(0ull)) &&
			noexcept(allocator().deallocate(nullptr, 0ull))
		)
	{
		box_cache_t& cache = core_t::cache;
		box_specs_t& specs = cache.specs;
		size_t buf_size    = specs.size;
		size			   = std::max(size, buf_size);
		size_t alloc_size  = size * Expand + 1;
		pointer_t buffer   = alloc.allocate(alloc_size);
		strutil::strcopy(buffer, cache.pointer, buf_size);
		buffer[buf_size]	= char_t();
		value.pointer		= buffer;
		specs.mode			= string_mode::storage;
		if constexpr (trait_is_advanced_mode()) {
			value.before = nullptr;
		}
		value.count			= buf_size;
		value.concord.left  = alloc_size - size - 1;
	}

	constexpr void reserve_cache(size_t size) noexcept {
		box_value_t& value = core_t::value;
		alloc_t& alloc	   = allocator();
		return heapify_cache(alloc, value, size);
	}

	constexpr void reserve_before (alloc_t&     alloc,
								   box_value_t& value,
		                           size_t	    size)
		noexcept (
			noexcept(allocator().allocate(0ull)) &&
			noexcept(allocator().deallocate(nullptr, 0ull))
		)
	{
		alloc.deallocate(value.before, value.before_count + value.before_left);
		value.before            = value.pointer;
		value.before_count      = value.count;
		value.before_left		= value.concord.left;
		value.pointer           = alloc.allocate(size);
		strutil::strcopy(value.pointer, value.before, value.before_count);
	}

	template <size_t Expand>
	constexpr void new_space (alloc_t&     alloc,
							  box_value_t& value,
		                      size_t       size)
		noexcept (
			noexcept(allocator().allocate(0ull)) &&
			noexcept(allocator().deallocate(nullptr, 0ull))
		)
	{
		pointer_t old_ptr	  = value.pointer;
		size_t old_count	  = value.count;
		size_t old_left		  = value.concord.left;
		size_t old_alloc_size = old_count + old_left + 1;

		size_t alloc_size  = size * Expand + 1;
		pointer_t new_ptr  = alloc.allocate(alloc_size);
		strutil::strcopy(new_ptr, old_ptr, old_count);
		new_ptr[old_count] = char_t();
		if (old_alloc_size > 1) {
			alloc.deallocate(old_ptr, old_alloc_size);
		}

		value.pointer	   = new_ptr;
		value.count		   = old_count;
		value.concord.left = alloc_size - old_count - 1;
	}

	template <bool InitHeap = false, size_t Expand = 2>
	constexpr void respace(size_t size)
		noexcept (
			noexcept (
				respace<InitHeap, Expand> (
					allocator(), core_t::value, size
				)
			)
		)
	{
		box_value_t& value = core_t::value;
		alloc_t& alloc     = allocator();
		return respace<InitHeap, Expand> (
			alloc, value, size
		);
	}

	template <bool InitHeap = false, size_t Expand = 2>
	constexpr void respace (alloc_t&	 alloc,
							box_value_t& value,
							size_t		 size)
		noexcept (
			noexcept(allocator().allocate(0ull)) &&
			noexcept(allocator().deallocate(nullptr, 0ull))
		)
	{
		if constexpr (trait_is_advanced_mode()) {
			return reserve_before(alloc, value, size);
		}
		if constexpr (InitHeap) {
			return heapify_cache<Expand>(alloc, value, size);
		}
		else {
			return new_space<Expand>(alloc, value, size);
		}
	}

	constexpr string_range info() const noexcept {
		const auto& cache = core_t::cache;
		const auto* data  = cache.pointer;
		size_t strlen	  = cache.specs.size;
		if (is_large_mode()) {
			const auto& value = core_t::value;
			data			  = value.pointer;
			strlen			  = value.count;
		}

		return { data, strlen };
	}

	constexpr void set_length(size_t size) noexcept {
		if (is_large_mode()) {
			core_t::value.count = size;
			return;
		}
		core_t::cache.specs.size = size;
	}

	constexpr size_t sublen(size_t size) noexcept {
		if (is_large_mode()) {
			return core_t::value.count -= size;
		}
		box_specs_t& specs = core_t::cache.specs;
		size_t result	   = specs.size - size;
		specs.size		   = result;
		return result;
	}

	constexpr void copy_cache(box_cache_t& object, box_cache_t& self) noexcept {
		strutil::strcopy (
			self.pointer,
			object.pointer,
			object.specs.size
		);
		self.specs = object.specs;
	}

	constexpr void move_cache(box_cache_t& object, basic_string& self) noexcept {
		box_cache_t& self_cache = self.cache;
		box_specs_t& self_specs = self_cache.specs;
		box_specs_t& obje_specs = object.specs;
		if (self.is_large_mode()) {
			alloc_t alloc	   = self.allocator();
			box_value_t& value = self.value;
			alloc.deallocate(value.pointer, self.alloc_size());
			self_specs.mode = string_mode::cache;
			self_cache.pointer[obje_specs.size] = char_t();
		}
		strutil::strcopy (
			self_cache.pointer,
			object.pointer,
			obje_specs.size
		);
		self_specs.size   = obje_specs.size;
		object.pointer[0] = char_t();
		obje_specs.size   = 0;
	}

	constexpr void copy_cache (box_cache_t&    cache,
		                       size_t          buf_size,
		                       const_pointer_t pointer,
		                       size_t          size)
		noexcept
	{
		strutil::strcopy (
			cache.pointer + buf_size,
			pointer, size
		);
		box_specs_t& specs = core_t::cache.specs;
		size_t next_size   = specs.size + size;
		specs.size		   = next_size;
	}

	constexpr static void reset_value (box_value_t& value,
		                               char_t       char_value,
		                               size_t       size,
		                               size_t       strlen)
		noexcept
	{
		strutil::strset (
			value.pointer + strlen,
			char_value, size - strlen
		);
		value.count = size;
		value.pointer[value.count] = char_t();
	}

	constexpr void swap_cache (alloc_t&     alloc,
		                       box_cache_t& self_cache,
		                       box_cache_t& object_cache)
		noexcept (
			noexcept(allocator().allocate(0ull)) &&
			noexcept(allocator().deallocate(nullptr, 0ull))
		)
	{
		size_t cache_size = self_cache.specs.size;
		size_t next_size  = cache_size + 1;
		pointer_t cache   = alloc.allocate(next_size);
		strutil::strcopy(cache, self_cache.pointer, cache_size);
		cache[cache_size] = char_t();
		strutil::strcopy(
			self_cache.pointer,
			object_cache.pointer,
			object_cache.specs.size
		);
		self_cache.specs.size = object_cache.specs.size;
		cache_size			  = self_cache.specs.size;
		self_cache.pointer[cache_size] = char_t();
		strutil::strcopy(object_cache.pointer, cache, next_size);
		object_cache.specs.size = cache_size;
		alloc.deallocate(cache, next_size);
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
			self_cache.specs.size
		);
		object_cache.specs.size = self_cache.specs.size;
		object_cache.pointer[object_cache.specs.size] = char_t();
		object_cache.specs.mode = string_mode::storage;
		self_value.pointer		= old_ptr;
		self_value.count		= old_size;
		self_cache.specs.mode	= string_mode::cache;
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
			cache.specs.size = sumlen;
			return;
		}
		box_value_t& value = core_t::value;
		value.count        = sumlen;
		size_t count	   = sumlen;
		size_t alloc_size  = count * 2;
		alloc_size		  += 1;
		value.pointer      = allocator().allocate(alloc_size);
		size_t new_left    = alloc_size - size;
		value.concord.left = new_left ? new_left -= 1 : new_left;
		strutil::strcopy(value.pointer, object.pointer(), obj_size);
		strutil::strcopy(value.pointer + obj_size, pointer, size);
		value.pointer[count] = char_t();
	}

	constexpr void assign_init(const_pointer_t str) noexcept {
		if (is_cache_mode()) {
			box_cache_t& cache = core_t::cache;
			size_t buf_size    = core_t::cache.specs.size;
			strutil::strcopy(cache.pointer, str, buf_size);
			return;
		}
		box_value_t& value = core_t::value;
		size_t size		   = value.count;
		size_t alloc_size  = size * 2 + 1;
		value.pointer	   = allocator().allocate(alloc_size);
		size_t new_left    = alloc_size - size;
		value.concord.left = new_left ? new_left -= 1 : new_left;
		strutil::strcopy(value.pointer, str, size);
		value.pointer[size] = char_t();
	}

	constexpr void assign_init (
		const std::initializer_list<const char_t*>& list,
				   char_t							fill)
		noexcept
	{
		pointer_t data     = core_t::cache.pointer;
		if (is_large_mode()) {
			box_value_t& value   = core_t::value;
			size_t size			 = value.count;
			size_t alloc_size    = size * 2;
			alloc_size			+= 1;
			data = value.pointer = allocator().allocate(alloc_size);
			value.concord.left   = alloc_size - size - 1;
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
			size_t buf_size = core_t::cache.specs.size;
			strutil::strset (
				core_t::cache.pointer,
				char_value,
				buf_size
			);
			return;
		}
		box_value_t& value = core_t::value;
		size_t size        = value.count;
		size_t alloc_size  = size * 2;
		alloc_size		  += 1;
		value.pointer      = allocator().allocate(alloc_size);
		value.concord.left = alloc_size - size - 1;
		strutil::strset(value.pointer, char_value, size);
		value.pointer[size] = char_t();
	}

	template <assoptions Option>
	assign_option_t<Option> assign_string(const char_t* pointer)
		noexcept (
			noexcept(append_impl(char_t()))
		)
	{
		std::size_t size = strutil::strlenof(pointer);
		for (std::size_t i = 0; i < size; i++) {
			char_t current = pointer[i];
			append_impl(current);
			if constexpr (Option == assoptions::pull) {
				co_yield current;
			}
		}
	}

	constexpr void assign_before (alloc_t&     alloc,
		                          box_value_t& self_value,
		                          box_value_t& object_value)
		noexcept
	{
		if constexpr (trait_is_advanced_mode()) {
			if (!object_value.before) {
				return;
			}

			self_value.before = alloc.allocate (
				object_value.before_count + object_value.before_left + 1
			);

			strutil::strcopy(
				self_value.before,
				object_value.before,
				object_value.before_count
			);

			self_value.before_count = object_value.before_count;
			self_value.before[self_value.before_count] = char_t();
		}
	}

	constexpr void assign_data(box_value_t& self_value, box_value_t& object_value) noexcept {
		alloc_t& alloc     = allocator();
		size_t object_size = object_value.count;
		size_t object_left = object_value.concord.left;

		self_value.pointer = alloc.allocate (
			object_size + object_left
		);

		strutil::strcopy (
			self_value.pointer,
			object_value.pointer,
			object_size
		);

		self_value.count                     = object_size;
		self_value.concord.left				 = object_left;
		self_value.pointer[self_value.count] = char_t();


		assign_before(alloc, self_value, object_value);
	}

	constexpr void assign_init(basic_string& object) noexcept {
		if (object.is_cache_mode()) {
			return copy_cache(object.cache, core_t::cache);
		}

		assign_data(core_t::value, object.value);
		core_t::cache.specs.mode = string_mode::storage;
	}

private:

	constexpr basic_string& assignment_cache(const_pointer_t pointer, size_t size) noexcept {
		box_cache_t& cache = core_t::cache;
		box_specs_t& specs = cache.specs;
		size_t next		   = specs.size + size;
		if (next >= core_t::buffer_size) {
			respace<true>(next);
			box_value_t& value = core_t::value;
			strutil::strcopy(value.pointer, pointer, size);
			value.count = size;
			value.pointer[value.count] = char_t();
			return *this;
		}
		strutil::strcopy(cache.pointer, pointer, size);
		specs.size = size;
		cache.pointer[specs.size] = char_t();
		return *this;
	}

	constexpr basic_string& assignment_value(const_pointer_t pointer, size_t size) noexcept {
		box_value_t& value = core_t::value;
		size_t next		   = value.count + size;
		if (next >= alloc_size(value)) {
			respace<false>(next);
		}
		strutil::strcopy(value.pointer, pointer, size);
		value.count = size;
		value.pointer[value.count] = char_t();
		return *this;
	}

	constexpr basic_string& assignment(const_pointer_t pointer, size_t size)
		noexcept (
			noexcept(respace<false>(0ull))
		)
	{
		if (is_cache_mode()) {
			return assignment_cache(pointer, size);
		}
		return assignment_value(pointer, size);
	}

	constexpr basic_string& assignment(char_t char_value) noexcept {
		return assignment(&char_value, 1);
	}

	constexpr basic_string& assignment(const_pointer_t pointer) noexcept {
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
		std::forward<basic_string&&>(object).move_string(*this);
		return *this;
	}

private:

	constexpr void swap_before(box_value_t& self, box_value_t& object) noexcept {
		if (!self.before) {
			return;
		}

		object.before       = self.before;
		object.before_count = self.before_count;
		object.before_left  = self.before_left;
		self.before			= nullptr;
	}

	constexpr void move_self(box_value_t&& self, basic_string& object)
		noexcept (
			noexcept(allocator().deallocate(nullptr, 0ull))
		)
	{
		alloc_t& object_alloc     = object.allocator();
		box_value_t& object_value = object.value;

		if (object.is_large_mode()) {
			object_alloc.deallocate (
				object_value.pointer,
				object.alloc_size()
			);
		}

		object_value.pointer	  = self.pointer;
		object_value.count		  = self.count;
		object_value.concord.left = self.concord.left;
		self.pointer			  = nullptr;
		self.count				  = 0;
		self.concord.left		  = 0;

		if constexpr (trait_is_advanced_mode()) {
			release_before(object_alloc, object);
			swap_before(self, object);
		}

		object.cache.specs.mode = string_mode::storage;
	}

	// Caller move to 'object' string
	constexpr void move_string(basic_string& object)
		&& noexcept (
			noexcept(move_self(std::move(core_t::value), object))
		)
	{
		if (is_cache_mode()) {
			move_cache(core_t::cache, object);
			return;
		}
		move_self(std::move(core_t::value), object);
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
			noexcept (
				exchange_self(object, allocator(), core_t::cache, core_t::value)
			) &&
			noexcept (
				exchange_object(object, allocator(), core_t::cache, core_t::value)
			)
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
				option(this->operator[](0));
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
			return core_t::cache.specs.size;
		}
		return core_t::value.count;
	}

	constexpr size_t string_capacity() const noexcept {
		if (is_cache_mode()) {
			return core_t::buffer_size - core_t::cache.specs.size;
		}
		return core_t::value.concord.left;
	}

	constexpr size_t string_max_size() const noexcept {
		if (is_cache_mode()) {
			return core_t::buffer_size;
		}

		return alloc_size();
	}

private:

	[[nodiscard]]
	constexpr bool is_large_mode() const noexcept {
		return core_t::cache.specs.mode == string_mode::storage;
	}

	[[nodiscard]]
	constexpr bool is_cache_mode() const noexcept {
		return core_t::cache.specs.mode == string_mode::cache;
	}

private:

	constexpr string_info curr_info() const noexcept {
		box_specs_t& specs = core_t::cache.specs;
		return string_info {
			specs.mode,
			specs.xored
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
			data		= value.pointer;
			strlen		= value.count;
			value.count = sumlen;
		}
		sumlen			 += strlen;
		size_t max_size   = large_mode ? alloc_size(value)
			: core_t::buffer_size;
		if (sumlen >= max_size) {
			pointer_t old = alloc.allocate(strlen);
			strutil::strcopy(old, data, strlen);
			if (large_mode) {
				alloc.deallocate(value.pointer, max_size);
			}
			size_t size			 = sumlen * 2;
			value.concord.left			 = size - sumlen;
			data = value.pointer = alloc.allocate(alloc_size(value));
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
		size_t strlen      = cache.specs.size;
		pointer_t data     = cache.pointer;
		size_t sumlen      = fill_size;
		if constexpr (fill_act == fill_action::center) {
			sumlen *= 2;
		}
		alloc_t alloc     = allocator();
		pointer_t address = align_switch_data (
			data, strlen, sumlen, alloc, core_t::value
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
		alloc.deallocate(address, strlen);
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
		box_specs_t& specs = cache.specs;
		pointer_t data     = cache.pointer;
		size_t bufsize     = specs.size;
		size_t nextsize    = bufsize + size;
		if (nextsize >= core_t::buffer_size) {
			respace<true, 2>(nextsize);
			box_value_t& value = core_t::value;
			data               = value.pointer;
			value.count        = nextsize;
			data[nextsize]     = char_t();
		}
		else {
			specs.size = nextsize;
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
			respace<false, 2>(nextlen);
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
		if (strutil::strlenof(str) > end) {
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
		box_specs_t& specs = core_t::cache.specs;
		size_t buflen      = specs.size;
		if (position > buflen) {
			return false;
		}
		size_t sumlen  = buflen + strlen;
		pointer_t data = core_t::cache.pointer;
		if (sumlen >= core_t::buffer_size) {
			respace<true, 2>(sumlen);
			box_value_t& value = core_t::value;
			data               = value.pointer;
			value.count        = sumlen;
			data[sumlen]       = char_t();
		}
		else {
			specs.size = sumlen;
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
			respace<false, 2>(sumlen);
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
		alloc.deallocate(value.pointer, alloc_size(value));
		box_cache_t& cache  = core_t::cache;
		core_t::cahce_size(size);
		cache.pointer[size] = '\0';
		core_t::mode(string_mode::cache);
		if constexpr (trait_is_advanced_mode()) {
			release_before(alloc, value);
		}
		strutil::strcopy(cache.pointer, buffer, size);
		alloc.deallocate(buffer, size + 1);
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
		size_t strlen      = cache.specs.size;
		alloc_t alloc      = allocator();
		pointer_t buffer   = alloc.allocate(strlen);
		strutil::strcopy(buffer, cache.pointer, strlen);
		box_value_t& value = core_t::value;
		size_t alloc_size  = strlen * 2 + 1;
		value.pointer      = alloc.allocate(alloc_size);
		strutil::strcopy(value.pointer, buffer, strlen);
		value.concord.left	  = alloc_size - strlen - 1;
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
			box_cache_t& cache = core_t::cache;
			if (size < core_t::buffer_size) {
				cache.pointer[size] = fill;
				return;
			}
			size_t strlen = cache.specs.size;
			respace<true>(size);
			reset_value(core_t::value, fill, size, strlen);
			return;
		}
		box_value_t& value  = core_t::value;
		std::size_t allsize = alloc_size(value);
		pointer_t data		= value.pointer;
		std::size_t count   = value.count;
		if (size < allsize) {
			if (size > count) {
				strutil::strset (
					data + count, fill, size - count
				);
			}
			data[size]		   = char_t();
			value.concord.left = allsize - size;
			value.count		   = size;
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
			if (size < core_t::cache.specs.size) {
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
			box_cache_t& cache = core_t::cache;
			if (size < core_t::buffer_size) {
				cache.pointer[size] = char_t();
				return true;
			}
			box_specs_t& specs = cache.specs;
			size_t strlen      = specs.size;
			respace<true>(size);
			size_t resu = option(core_t::value, size, strlen);
			specs.size  = resu;
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

	constexpr std::optional<size_t> front_index (char_t target,
												 size_t point,
												 size_t end)
		noexcept
	{
		const_pointer_t data = pointer();
		for (; point < end; ++point) {
			if (data[point] == target) {
				return point;
			}
		}
		return std::nullopt;
	}

	constexpr std::optional<size_t> last_index (char_t target,
												size_t point,
												size_t end)
		noexcept
	{
		const_pointer_t data = pointer();
		for (; point != static_cast<size_t>(-1) && point >= end; --point, --end) {
			if (data[point] == target) {
				return point;
			}
		}
		return std::nullopt;
	}

	constexpr bool index_string(size_t position) noexcept {
		return position < string_length();
	}

	constexpr std::optional<size_t> index_string (char_t target,
												  size_t point,
												  size_t end)
		noexcept
	{
		if (point > end) {
			return last_index(target, point, end);
		}
		return front_index(target, point, end);
	}

	constexpr dast::vector<size_t> inquiry_subscript(char_t value)
		noexcept (
			noexcept(noexcept(std::vector<size_t>{}.push_back(0)))
		)
	{
		dast::vector<size_t> result;
		auto [data, size] = info();

		for (size_t i = 0; i < size; i++) {
			if (data[i] == value) {
				result.push_back(i);
			}
		}

		return result;
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
			box_specs_t& specs = cache.specs;
			size_t buf_size    = specs.size;
			size_t next_count  = buf_size + 1;
			size_t alloc_count = buf_size + 2;
			if (alloc_count < core_t::buffer_size) {
				cache.pointer[buf_size] = char_value;
				specs.size				= buf_size + 1;
				return;
			}
			respace<true, 2>(alloc_count);
			box_value_t& value		  = core_t::value;
			value.pointer[buf_size]   = char_value;
			value.pointer[next_count] = char_t();
			value.count				  = next_count;
		}
		else {
			box_value_t& value = core_t::value;
			size_t heap_count  = value.count;
			size_t next_size   = heap_count + 1;
			size_t allocsize   = alloc_size(value);
			if (next_size + 1 >= allocsize) {
				respace<false, 2>(next_size);
			}
			else {
				value.concord.left -= 1;
			}
			value.pointer[heap_count]  = char_value;
			value.count += 1;
			value.pointer[value.count] = char_t();
		}
	}

	constexpr void append_value (box_value_t&    value,
								 const_pointer_t pointer,
		                         size_t&         heap_count,
								 size_t          offset,
								 size_t          size)
		noexcept
	{
		strutil::strcopy (
			value.pointer + offset,
			pointer, size
		);
		heap_count += size;
		value.pointer[heap_count] = char_t();
	}

	constexpr void append_cache(const_pointer_t pointer, size_t size)
		noexcept (
			noexcept(respace<true>(0ull))
		)
	{
		box_cache_t& cache = core_t::cache;
		size_t buf_size    = cache.specs.size;
		size_t next_size   = buf_size + size;
		if (next_size < core_t::buffer_size) {
			return copy_cache (
				cache, buf_size, pointer, size
			);
		}
		respace<true, 2>(next_size);
		box_value_t& value = core_t::value;
		size_t& heap_count = value.count;
		return append_value (
			value, pointer, heap_count,
			buf_size, size
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
		pointer_t old_ptr  = value.pointer;
		if (next_size >= alloc_size(value)) {
			respace<false, 2>(next_size);
		}
		if (pointer == old_ptr) {
			pointer = value.pointer;
		}
		value.concord.left -= size;
		return append_value (
			value, pointer, heap_count,
			heap_count, size
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

	constexpr basic_string& string_append(char_t char_value) noexcept {
		append_impl(char_value);
		return *this;
	}

	constexpr basic_string& string_append(const_pointer_t pointer) noexcept {
		append_impl(pointer);
		return *this;
	}

	constexpr basic_string& string_append(basic_string& string) noexcept {
		append_impl(string);
		return *this;
	}

	template <rest::string_view StringView>
	constexpr basic_string& string_append(const StringView& view) noexcept {
		append_impl(view.data(), view.size());
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

	constexpr size_t str_cut(char_t char_value) noexcept {
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

	constexpr auto corner_info (pointer_t data,
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
		auto [
			trimmed_count,
			string_begin,
			trimmed_end
		] = corner_info (
			data, strlen, char_value
		);
		if (size_t next = strlen - trimmed_count; next) {
			strutil::strmove (
				data, data + string_begin, next
			);
		}
		data[sublen(trimmed_count)] = char_t();
		return trimmed_count;
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
	constexpr bool compare(basic_string& str) const noexcept {
		size_t strlen = str.string_length();
		size_t size   = string_length();
		if (strlen != size) {
			return false;
		}
		return compare(str.pointer());
	}

	[[nodiscard]]
	constexpr bool is_empty() const noexcept {
		return string_length() == 0;
	}

public:

	template <class... ArgsType>
	constexpr basic_string& operator=(this basic_string& self, ArgsType&&... args)
		noexcept requires (
			requires {
				self.assignment(std::forward<ArgsType>(args)...);
			}
		)
	{
		return self.assignment(std::forward<ArgsType>(args)...);
	}

private:

	constexpr void release_before(alloc_t& alloc, box_value_t& value) noexcept {
		if constexpr (trait_is_advanced_mode()) {
			if (!value.before) {
				return;
			}

			alloc.deallocate (
				value.before,
				value.before_count + value.before_left
			);
		}
	}

	constexpr void release(alloc_t& alloc, box_value_t& value) noexcept {
		release_before(alloc, value);
		alloc.deallocate(value.pointer, alloc_size());
	}

public:

	constexpr ~basic_string() noexcept {
		if (!is_large_mode()) {
			return;
		}

		release(allocator(), core_t::value);
	}
};