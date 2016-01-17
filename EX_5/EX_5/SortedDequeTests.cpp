#include <iostream>
#include <string>
#include <algorithm>
#include <gtest/gtest.h>

#include "SortedDeque.h"
#include "SortedDeque.cpp"

using namespace std;
using namespace ::testing;


class SortedDequeTests : public ::testing::Test
{
	SortedDeque<int> sortedDeque;

public:
	template<typename T>
	unsigned CeilOfDivision(T divident, T divisor)
	{
		double dividentAsDouble = divident;
		double divisorAsDouble = divisor;

		return static_cast<unsigned>(ceil(dividentAsDouble / divisorAsDouble));
	}
};


TEST_F(SortedDequeTests, testConstructionOfEmptyContainer)
{
	SortedDeque<int> sortedDeque;
	ASSERT_TRUE(sortedDeque.empty());
	ASSERT_EQ(0, sortedDeque.size_total());
	ASSERT_EQ(0, sortedDeque.size_unique());
	ASSERT_EQ(0, sortedDeque.capacity());
}

TEST_F(SortedDequeTests, testReserve)
{
	const unsigned SIZE_OF_BUCKET = 3;
	SortedDeque<int, SIZE_OF_BUCKET> sortedDeque;

	const unsigned capacityWhichIWant = SIZE_OF_BUCKET * 10 + 1;
	sortedDeque.reserve(capacityWhichIWant);

	const unsigned expectedNumberOfBuckets = CeilOfDivision(capacityWhichIWant, SIZE_OF_BUCKET);
	const unsigned expectedCapacity = expectedNumberOfBuckets * SIZE_OF_BUCKET;

	ASSERT_TRUE(sortedDeque.empty());
	ASSERT_EQ(0, sortedDeque.size_total());
	ASSERT_EQ(0, sortedDeque.size_unique());
}

TEST_F(SortedDequeTests, testReserveWithSmallerNewCapacity)
{
	const unsigned SIZE_OF_BUCKET = 3;
	SortedDeque<int, SIZE_OF_BUCKET> sortedDeque;

	const unsigned capacityWhichIWant = SIZE_OF_BUCKET * 10;
	sortedDeque.reserve(capacityWhichIWant);

	const unsigned capacityWhichIWant_new = SIZE_OF_BUCKET * 5;
	sortedDeque.reserve(capacityWhichIWant);

	ASSERT_EQ(0, sortedDeque.capacity());

	ASSERT_TRUE(sortedDeque.empty());
	ASSERT_EQ(0, sortedDeque.size_total());
	ASSERT_EQ(0, sortedDeque.size_unique());
}

TEST_F(SortedDequeTests, testSimpleAddingIntoSingleBucket)
{
	vector<int> valuesToAdd = { 2, 0, 9, 1 };

	const unsigned SIZE_OF_BUCKET = 10;
	SortedDeque<int, SIZE_OF_BUCKET> sortedDeque;
	for (unsigned i = 0; i < valuesToAdd.size(); ++i)
		sortedDeque.insert(valuesToAdd[i]);

	sort(valuesToAdd.begin(), valuesToAdd.end());

	for (unsigned i = 0; i < valuesToAdd.size(); ++i)
	{
		SCOPED_TRACE(i);
		ASSERT_EQ(valuesToAdd.at(i), sortedDeque.at(i));
	}

	ASSERT_FALSE(sortedDeque.empty());
	ASSERT_EQ(valuesToAdd.size(), sortedDeque.size_total());
	ASSERT_EQ(valuesToAdd.size(), sortedDeque.size_unique());

	ASSERT_EQ(1u, sortedDeque.number_of_buckets());
}

TEST_F(SortedDequeTests, testAddingSmallestValueWhenFirstBucketIsFull)
{
	const unsigned SIZE_OF_BUCKET = 3;
	SortedDeque<int, SIZE_OF_BUCKET> sortedDeque;

	vector<int> valuesToAdd = { 2, 1, 9 };

	for (unsigned i = 0; i < valuesToAdd.size(); ++i)
		sortedDeque.insert(valuesToAdd[i]);

	sort(valuesToAdd.begin(), valuesToAdd.end());

	for (unsigned i = 0; i < valuesToAdd.size(); ++i)
	{
		SCOPED_TRACE(i);
		ASSERT_EQ(valuesToAdd.at(i), sortedDeque.at(i));
	}

	ASSERT_FALSE(sortedDeque.empty());
	ASSERT_EQ(valuesToAdd.size(), sortedDeque.size_total());
	ASSERT_EQ(valuesToAdd.size(), sortedDeque.size_unique());

	ASSERT_EQ(SIZE_OF_BUCKET, sortedDeque.capacity());
	ASSERT_EQ(1u, sortedDeque.number_of_buckets());

	///////////////////////////////////////////////////////
	const int valueSmallerThanPreviousValuesToAdd = 0;
	sortedDeque.insert(valueSmallerThanPreviousValuesToAdd);

	ASSERT_EQ(valueSmallerThanPreviousValuesToAdd, sortedDeque.at(0));

	ASSERT_FALSE(sortedDeque.empty());
	ASSERT_EQ(valuesToAdd.size() + 1, sortedDeque.size_total());
	ASSERT_EQ(valuesToAdd.size() + 1, sortedDeque.size_unique());

	ASSERT_EQ(2 * SIZE_OF_BUCKET, sortedDeque.capacity());
	ASSERT_EQ(2u, sortedDeque.number_of_buckets());
}

TEST_F(SortedDequeTests, testAddingGreaterValueWhenFirstBucketIsFull)
{
	const unsigned SIZE_OF_BUCKET = 3;
	SortedDeque<int, SIZE_OF_BUCKET> sortedDeque;

	vector<int> valuesToAdd = { 2, 1, 9 };

	for (unsigned i = 0; i < valuesToAdd.size(); ++i)
		sortedDeque.insert(valuesToAdd[i]);

	sort(valuesToAdd.begin(), valuesToAdd.end());

	for (unsigned i = 0; i < valuesToAdd.size(); ++i)
	{
		SCOPED_TRACE(i);
		ASSERT_EQ(valuesToAdd.at(i), sortedDeque.at(i));
	}

	ASSERT_FALSE(sortedDeque.empty());
	ASSERT_EQ(valuesToAdd.size(), sortedDeque.size_total());
	ASSERT_EQ(valuesToAdd.size(), sortedDeque.size_unique());

	ASSERT_EQ(SIZE_OF_BUCKET, sortedDeque.capacity());
	ASSERT_EQ(1u, sortedDeque.number_of_buckets());

	///////////////////////////////////////////////////////
	const int valueGreaterThanPreviousValuesToAdd = 100;
	sortedDeque.insert(valueGreaterThanPreviousValuesToAdd);
	ASSERT_TRUE(sortedDeque.back() == valueGreaterThanPreviousValuesToAdd);

	ASSERT_FALSE(sortedDeque.empty());
	ASSERT_EQ(valuesToAdd.size() + 1, sortedDeque.size_total());
	ASSERT_EQ(valuesToAdd.size() + 1, sortedDeque.size_unique());

	ASSERT_EQ(2 * SIZE_OF_BUCKET, sortedDeque.capacity());
	ASSERT_EQ(2u, sortedDeque.number_of_buckets());
}

TEST_F(SortedDequeTests, testAddingValueWhichShouldBeAddedBetweenTwoFullBuckets)
{
	const unsigned SIZE_OF_BUCKET = 3;
	SortedDeque<int, SIZE_OF_BUCKET> sortedDeque;

	vector<int> valuesToAddToFirstBucket = { 1, 2, 3 };
	for (unsigned i = 0; i < valuesToAddToFirstBucket.size(); ++i)
		sortedDeque.insert(valuesToAddToFirstBucket[i]);

	vector<int> valuesToAddToSecondBucket = { 5, 6, 7 };
	for (unsigned i = 0; i < valuesToAddToSecondBucket.size(); ++i)
		sortedDeque.insert(valuesToAddToSecondBucket[i]);

	int valueWhichShouldBeAddedBetweenTwoFullBuckets = 4;
	sortedDeque.insert(valueWhichShouldBeAddedBetweenTwoFullBuckets);

	///////////////////////////////////////////////////////

	const unsigned expectedTotalNumberOfElements = valuesToAddToFirstBucket.size() + 1 + valuesToAddToSecondBucket.size();
	ASSERT_EQ(expectedTotalNumberOfElements, sortedDeque.size_total());
	ASSERT_EQ(expectedTotalNumberOfElements, sortedDeque.size_unique());

	ASSERT_EQ(3 * SIZE_OF_BUCKET, sortedDeque.capacity());
	ASSERT_EQ(3u, sortedDeque.number_of_buckets());
}

TEST_F(SortedDequeTests, testAddingValueWhichShouldBeAddedBetweenTwoValuesInFullBucketAfterWhichIsAnotherFullBucket)
{
	const unsigned SIZE_OF_BUCKET = 3;
	SortedDeque<double, SIZE_OF_BUCKET> sortedDeque;

	vector<int> valuesToAddToFirstBucket = { 1, 2, 3 };
	for (unsigned i = 0; i < valuesToAddToFirstBucket.size(); ++i)
		sortedDeque.insert(valuesToAddToFirstBucket[i]);

	vector<int> valuesToAddToSecondBucket = { 5, 6, 7 };
	for (unsigned i = 0; i < valuesToAddToSecondBucket.size(); ++i)
		sortedDeque.insert(valuesToAddToSecondBucket[i]);

	double valueWhichShouldBeAddedBetweenTwoFullBuckets = 2.78;
	sortedDeque.insert(valueWhichShouldBeAddedBetweenTwoFullBuckets);

	///////////////////////////////////////////////////////

	vector<double> allValues;
	allValues.insert(allValues.end(), valuesToAddToFirstBucket.begin(), valuesToAddToFirstBucket.end());
	allValues.insert(allValues.end(), valuesToAddToSecondBucket.begin(), valuesToAddToSecondBucket.end());
	allValues.insert(allValues.end(), valueWhichShouldBeAddedBetweenTwoFullBuckets);
	sort(allValues.begin(), allValues.end());

	for (unsigned i = 0; i < allValues.size(); ++i)
	{
		SCOPED_TRACE(i);
		ASSERT_EQ(allValues.at(i), sortedDeque.at(i));
	}

	///////////////////////////////////////////////////////

	const unsigned expectedTotalNumberOfElements = valuesToAddToFirstBucket.size() + 1 + valuesToAddToSecondBucket.size();
	ASSERT_EQ(expectedTotalNumberOfElements, sortedDeque.size_total());
	ASSERT_EQ(expectedTotalNumberOfElements, sortedDeque.size_unique());

	ASSERT_EQ(3 * SIZE_OF_BUCKET, sortedDeque.capacity());
	ASSERT_EQ(3u, sortedDeque.number_of_buckets());
}

TEST_F(SortedDequeTests, testAddingDuplicatedElements)
{
	const unsigned SIZE_OF_BUCKET = 4;
	SortedDeque<int, SIZE_OF_BUCKET> sortedDeque;

	vector<int> valuesToAdd = { 2, 1, 9, 1, 5, 2, 9 };

	for (unsigned i = 0; i < valuesToAdd.size(); ++i)
		sortedDeque.insert(valuesToAdd[i]);

	sort(valuesToAdd.begin(), valuesToAdd.end());

	vector<int> uniqueValues(valuesToAdd);
	vector<int>::iterator beginOfDuplicatedElements = unique(uniqueValues.begin(), uniqueValues.end());
	uniqueValues.erase(beginOfDuplicatedElements, uniqueValues.end());

	for (unsigned i = 0; i < uniqueValues.size(); ++i)
	{
		SCOPED_TRACE(i);
		ASSERT_EQ(uniqueValues.at(i), sortedDeque.at(i));
	}

	ASSERT_FALSE(sortedDeque.empty());
	ASSERT_EQ(valuesToAdd.size(), sortedDeque.size_total());
	ASSERT_EQ(uniqueValues.size(), sortedDeque.size_unique());

	ASSERT_EQ(1u, sortedDeque.number_of_buckets());
}

TEST_F(SortedDequeTests, testIterator)
{
	SortedDeque<float> sortedDeque;

	vector<float> valuesToAdd = { 1.11f, 2.22f, 3.33f, 4.44f, 5.55f };

	for (unsigned i = 0; i < valuesToAdd.size(); ++i)
		sortedDeque.insert(valuesToAdd[i]);

	sort(valuesToAdd.begin(), valuesToAdd.end());

	vector<float>::iterator vit = valuesToAdd.begin();
	for (SortedDeque<float>::iterator it = sortedDeque.begin(); it != sortedDeque.end(); ++it, ++vit)
		ASSERT_EQ((*it).first, *vit);
}

int main(int argc, char* argv[])
{
	InitGoogleTest(&argc, argv);
	int retVal = RUN_ALL_TESTS();
	cin.get();
	return retVal;
}