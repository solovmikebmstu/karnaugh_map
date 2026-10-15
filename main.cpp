#include <k_map.h>

void example1(int argc, const BoolTable& data)
{
	K_map<> map {argc, data}; 
       	//first argument is count of bool function arguments

	map.print();
	map.print_conjunctions();
}

void example2(int argc, Boolnum<>& vector)
{
	Truth_table table {argc, vector};
	//first argument is the same as in example1,
	//second is a bool function result vector,
	//in this vector least significant bit is a result of first row in truth table

	K_map<> map {table};
	map.print();
	map.print_conjunctions();
}

int main()
{
	try {
#if 1
		BoolTable data {
			{0,0,1,0},
			{0,1,1,1},
			{1,1,1,1},
			{0,1,1,1}
		};
		example1(4, data);
#endif
#if 1
		Boolnum<> vector {"0xDDDDDD"};
		example2(5, vector);
#endif
#if 1
		Boolnum<> another_vector {0b011100101111001};
		example2(4, another_vector);
#endif
		return 0;
	}
	catch(const std::exception& ex) {
		std::cout << "ERROR:\t" << ex.what() << std::endl;
		return 1;
	}
}

