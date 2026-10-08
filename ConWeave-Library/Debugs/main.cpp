import std;
import cwlib;

int main() {
	dast::block_allocator<dast::plmem_holder> alloc(20);
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
	std::cout << *alloc.retreat<char>() << '\n';
}