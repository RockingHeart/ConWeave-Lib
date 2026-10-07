import std;
import cwlib;

int main() {
	std::cout << sizeof(dast::cstring) << '\n';
	dast::block_allocator<dast::plmem_holder> alloc(20);
	std::string_view obj = "Hello";
	alloc.allocate<std::string_view>(obj);
	std::cout << *alloc.unchecked_retreat<std::string_view>(0) << '\n';
}