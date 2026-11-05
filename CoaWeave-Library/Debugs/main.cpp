import std;
import cwlib;

using list1 = sequence_list<ordered_source<unsigned char, 0, 10>>;
using list2 = sequence_list<unordered_source<unsigned char, ';', '+'>>;


int main() {
	dast::memory_mapping map(1024);
	/*dast::block_allocator<dast::memory_mapping> alloc(9);
	alloc.allocate<char>("Hello World", 12ull);
	alloc.start_over();
	std::cout << alloc.data<char>();
	if (alloc.next_block()) {
		std::cout << alloc.data<char>() << '\n';
	}*/
}