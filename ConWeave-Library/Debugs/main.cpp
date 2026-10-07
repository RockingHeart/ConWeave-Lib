import std;
import cwlib;

int main() {
	std::cout << sizeof(dast::cstring) << '\n';
	dast::block_allocator<dast::plmem_holder> alloc(20);
	alloc.allocate("Hello World", 12);
	std::cout << alloc.unchecked_retreat<const char*>(0) << '\n';
}