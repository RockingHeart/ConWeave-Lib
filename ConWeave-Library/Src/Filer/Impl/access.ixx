export module sys.access;

import <cstddef>;

export enum class comaccess : std::size_t {
	read		 = 1 << 0,
	write		 = 1 << 1,
	execute		 = 1 << 2,
	read_share   = 1 << 3,
	write_share  = 1 << 4,
	delete_share = 1 << 5,
	create		 = 1 << 6,
	make		 = 1 << 7,
	clear		 = 1 << 8
};

export constexpr std::size_t operator|(comaccess left, comaccess right) noexcept {
	return static_cast<std::size_t>(left) | static_cast<std::size_t>(right);
}

export constexpr std::size_t operator|(std::size_t left, comaccess right) noexcept {
	return left | static_cast<std::size_t>(right);
}

export constexpr std::size_t operator&(comaccess left, comaccess right) noexcept {
	return static_cast<std::size_t>(left) & static_cast<std::size_t>(right);
}

export constexpr bool operator&(std::size_t left, comaccess right) noexcept {
	return left & static_cast<std::size_t>(right);
}

export enum class permission : std::size_t {
	read_only	  = static_cast<std::size_t>(comaccess::read),
	write_only	  = static_cast<std::size_t>(comaccess::write),
	read_write	  = comaccess::read | comaccess::write,
	execute_only  = static_cast<std::size_t>(comaccess::execute),
	read_execute  = comaccess::execute | comaccess::read,
	sound_execute = comaccess::execute | comaccess::read | comaccess::write,
};

export constexpr bool operator&(permission per, comaccess comaces) noexcept {
	return static_cast<std::size_t>(per) & static_cast<std::size_t>(comaces);
}

export constexpr bool operator&(permission per, std::size_t comaces) noexcept {
	return static_cast<std::size_t>(per) & comaces;
}