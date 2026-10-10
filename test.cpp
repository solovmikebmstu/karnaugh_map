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

