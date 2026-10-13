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
	map.make_head(5);
	EXPECT_EQ(map.gray_code_row.size(), 8);
	EXPECT_EQ(map.gray_code_column.size(), 4);
	
}

TEST(MakeMap, Simple)
{
	Truth_table<uint1024_t> table;
	table.generate(4, static_cast<uint1024_t>(0b1011101011101000));

	K_map<uint1024_t> map;
	map.make(table);

	std::vector<std::vector<bool>> ref {{0,0,1,0},
					    {0,1,1,1},
					    {1,1,1,0},
					    {0,1,1,0}};

	EXPECT_EQ(map.data.size(), ref.size());
	EXPECT_EQ(map.data[0].size(), ref[0].size());
#if 1
	int column_index {0};
	int row_index {0};
	for(const auto& row : map.data) {
		for(const auto& column : row) {
			EXPECT_EQ(column, ref[row_index][column_index]);
			++column_index;
		}
		column_index = 0;
		++row_index;
	}
#endif
#if 0
	table.result_vector.print_bits();
	table.result_vector.print();
	map.print();
	print_container(map.data[3]);
#endif
}

TEST(CombineBoolnums, Simple)
{
	Boolnum<int> msb {0b01, 2};
	Boolnum<int> lsb {0b101, 3};
	Boolnum<int> res = combine_Boolnums(msb, lsb);
//	res.print_bits();
	Boolnum<int> ref {0b01101, 5};
	EXPECT_EQ(res.getnum(), ref.getnum());
	EXPECT_EQ(res.bit_count, ref.bit_count);
}

TEST(MapMakeDirect, Simple)
{
	std::vector<std::vector<bool>> ref {{0,0,1,0},
					    {0,1,1,1},
					    {1,1,1,0},
					    {0,1,1,0}};

	K_map<int> map;
	map.make_direct(4, ref);
#if 0
	map.print();
#endif
	EXPECT_EQ(map.data.size(), ref.size());
	EXPECT_EQ(map.data[0].size(), ref[0].size());

}

TEST(MapMake, Visualise)
{
	Truth_table<uint1024_t> table;
	table.generate(7, static_cast<uint1024_t>(0));

	K_map<uint1024_t> map;
	map.make(table);
#if 0
	map.print();
	std::cout << "row size:\t" << map.max_size.row << std::endl
		<< "column size:\t" << map.max_size.column << std::endl;
#endif
}

TEST(GenerateConjuctionSizes, Simple)
{
	Truth_table<uint1024_t> table;
	table.generate(4, static_cast<uint1024_t>(0));

	K_map<uint1024_t> map;
	map.make(table);
//	map.generate_conjuction_sizes();

	std::vector<row_column> ref {{4,4},
				     {2,4},{4,2},
				     {1,4},{2,2},{4,1},
				     {1,2},{2,1},
				     {1,1}};
	size_t i {0};
	for(const auto& v : map.conjunctions_sizes) {
		EXPECT_EQ(v.row, ref[i].row);
		EXPECT_EQ(v.column, ref[i].column);
		++i;
	}

#if 1
	print_container(map.conjunctions_sizes);
	map.print();
	std::cout << "row size:\t" << map.max_size.row << std::endl
		<< "column size:\t" << map.max_size.column << std::endl;
#endif

}

TEST(Log2, Values)
{
	EXPECT_EQ(log2(512), 9);
	EXPECT_EQ(log2(4), 2);
	EXPECT_EQ(log2(1), 0);
}

