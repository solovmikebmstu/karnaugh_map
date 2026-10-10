#include <k_map.h> 
#include <gtest/gtest.h>

TEST(Pow2, val)
{
	auto pow {pow2<int>(10)};
	EXPECT_EQ(pow, 1024);
}

TEST(GrayCode, ThreeBit)
{
	std::vector<Boolnum<uint1024_t>> reference {{0b000},
						    {0b001},
						    {0b011},
						    {0b010},
						    {0b110},
						    {0b111},
						    {0b101},
						    {0b100}};
	K_map<uint1024_t> map;
	auto codes { map.make_gray_code(3) };
	EXPECT_EQ(codes.size(), reference.size());

	size_t sz {codes.size()};

	for(size_t i = 0; i < sz; ++i) {
		EXPECT_EQ(codes[i].getnum(), reference[i].getnum());
	}
}

TEST(GrayCode, FourBit)
{

	Boolnum<int> ref {0b1010};
	K_map<int> map;
	auto codes {map.make_gray_code(4)};
	auto code {codes[12]};
	EXPECT_EQ(code.getnum(), ref.getnum());
}

TEST(Cout, HelloWorld)
{
	std::cout << "hello world!" << std::endl;
}

TEST(SplitToHalfs, val)
{
	K_map<uint8_t> map;
	auto [higher, lower] = map.split_to_halfs(7);
	EXPECT_EQ(higher, 4);
	EXPECT_EQ(lower, 3);
	auto [higher2, lower2] {map.split_to_halfs(12)};
	EXPECT_EQ(higher2, 6);
	EXPECT_EQ(lower2, 6);
}

TEST(MakeMapHead, Simple)
{
	K_map<uint1024_t> map;
	map.make_map_head(5);
	EXPECT_EQ(map.gray_code_row.size(), 8);
	EXPECT_EQ(map.gray_code_column.size(), 4);
	
}

TEST(MakeMap, Visualise)
{
	Truth_table<int> table;
	table.generate(4, 0b01010101);
	table.result_vector.print_bits();

	K_map<int> map;
	map.make_map(table);
	map.print();
}

TEST(CombineBoolnums, Simple)
{
	Boolnum<int> msb {0b01, 2};
	Boolnum<int> lsb {0b101, 3};
	Boolnum<int> res = combine_Boolnums(msb, lsb);
	res.print_bits();
	Boolnum<int> ref {0b01101, 5};
	EXPECT_EQ(res.getnum(), ref.getnum());
	EXPECT_EQ(res.bit_count, ref.bit_count);
}

