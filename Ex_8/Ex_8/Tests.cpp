#include "gtest\gtest.h"
#include "MetaFunctions.h"
#include <iostream>
#include <array>
#include <algorithm>



TEST(BubbleSort, sort_big_array)
{
	int tablica[12] = { 321, 54, 22, 3, -7, 2, 1, 32, 0, 11, 321, -321};
	int sorted[12] = { -321, -7, 0, 1, 2, 3, 11 , 22, 32, 54, 321, 321 };
	bubble_sort_template<12>(tablica);
	for (int i = 0; i < 12; i++)
		ASSERT_EQ(tablica[i], sorted[i]);
}

TEST(BubbleSort, sort_small_array)
{
	int tablica[4] = { 3, 2, 1, 0};
	int sorted[4] = { 0, 1, 2, 3 };
	bubble_sort_template<4>(tablica);
	for (int i = 0; i < 4; i++)
		ASSERT_EQ(tablica[i], sorted[i]);
}

TEST(BubbleSort, sort_single_num)
{
	int tablica[1] = { -321 };
	int sorted[1] = { -321 };
	bubble_sort_template<1>(tablica);
	ASSERT_EQ(tablica[0], sorted[0]);
}

TEST(Silnia, silnia)
{
	const long long int result1 = silnia<1>().value;
	ASSERT_EQ(result1, 1);

	const long long int result2 = silnia<5>().value;
	ASSERT_EQ(result2, 120);

	const long long int result3 = silnia<8>().value;
	ASSERT_EQ(result3, 40320);

	const long long int result4 = silnia<12>().value;
	ASSERT_EQ(result4, 479001600);

	const long long int result5 = silnia<15>().value;
	ASSERT_EQ(result5, 1307674368000);

	const long long int result6 = silnia<20>().value;
	ASSERT_EQ(result6, 2432902008176640000);
}

TEST(Fibbonaci, fibb)
{
	const int result0 = fibbonaci<0>().val;
	ASSERT_EQ(result0, 0);

	const int result1 = fibbonaci<3>().val;
	ASSERT_EQ(result1, 2);

	const int result2 = fibbonaci<10>().val;
	ASSERT_EQ(result2, 55);

	const int result3 = fibbonaci<15>().val;
	ASSERT_EQ(result3, 610);

	const int result4 = fibbonaci<20>().val;
	ASSERT_EQ(result4, 6765);
}

TEST(Pi, pi_generate)
{
	ASSERT_NEAR(3.14, CalculatePi<10>::pi*8, 0.1);

	ASSERT_NEAR(3.14, CalculatePi<100>::pi*8, 0.01);

	ASSERT_NEAR(3.14, CalculatePi<500>::pi*8, 0.001);
}

TEST(Newton, newton)
{

	const int result0 = newton<2, 2>();
	ASSERT_EQ(result0, 1);

	const int result1 = newton<4, 2>();
	ASSERT_EQ(result1, 6);

	const int result2 = newton<10, 2>();
	ASSERT_EQ(result2, 45);

	const int result3 = newton<10, 3>();
	ASSERT_EQ(result3, 120);
}

TEST(NWD, nwd)
{
	const int result0 = NWD<5, 5>().value;
	ASSERT_EQ(result0, 5);

	const int result1 = NWD<12, 18>().value;
	ASSERT_EQ(result1, 6);

	const int result2 = NWD<512, 124>().value;
	ASSERT_EQ(result2, 4);

	const int result3 = NWD<123, 98761>().value;
	ASSERT_EQ(result3, 1);
}

int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	int result = RUN_ALL_TESTS();
	std::cin.get();
	return result;
}