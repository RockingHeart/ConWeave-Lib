import std;
import cwlib;

using list1 = sequence_list<ordered_source<unsigned char, 0, 10>>;
using list2 = sequence_list<unordered_source<unsigned char, ';', '+'>>;

int main() {
	std::array<std::span<const unsigned char>, 2> arr {
		list1::list(), list2::list()
	};
	for (auto& list : arr) {
		for (auto v : list) {
			std::cout << static_cast<std::size_t>(v) << '\n';
		}
	}
	/*dast::block_allocator<dast::plmem_holder> alloc(20);
	std::string_view obj1 = "Hello";
	std::string_view obj2 = "World";
	alloc.allocate<std::string_view>(obj1);
	alloc.allocate<char>(' ');
	alloc.allocate<std::string_view>(obj2);
	alloc.allocate<char>('.');
	alloc.start_over();
	alloc.resize_curr();
	std::cout << *alloc.retreat<std::string_view>();
	std::cout << *alloc.retreat<char>();
	std::cout << *alloc.retreat<std::string_view>();
	std::cout << *alloc.retreat<char>() << '\n';*/
}