#include <k_map.h>

int main(int argc, char** argv)
{
	try {
		Truth_table<uint1024_t> table;
		table.generate(4, static_cast<uint1024_t>(0xABCD));
		table.print();

		K_map<uint1024_t> map;
		map.make(table);
		map.print();

		return 0;
	}
	catch(const std::exception& ex) {
		std::cout << "ERROR:\t" << ex.what() << std::endl;
		return 1;
	}
}

