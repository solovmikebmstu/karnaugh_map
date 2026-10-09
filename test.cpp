#include <k_map.h> 
#include <gtest/gtest.h>

TEST(Example, simple)
{
	auto pow {pow2<int>(10)};
	EXPECT_EQ(pow, 1024);
}

