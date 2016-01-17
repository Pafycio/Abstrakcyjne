#pragma once
#include <list>
#include <vector>

template <typename T, unsigned TSize = 1>
class SortedDeque
{
public:
	using Entity = std::pair<T, unsigned int>;
	using Bucket = std::vector<Entity>;
	using Deque = std::vector<Bucket>;

	using iterator = typename std::vector<std::pair<T, unsigned> >::iterator;

	const unsigned SIZE_OF_BUCKET = TSize;
	unsigned TOTAL_MAX_SIZE;

	unsigned currCapacity;
	unsigned maxCapacity;
	unsigned maxBucket;

	T at(unsigned);
	T back();

	SortedDeque() = default;
	~SortedDeque() = default;

	bool empty();
	unsigned size_total();
	unsigned size_unique();
	unsigned capacity();
	unsigned number_of_buckets();

	void reserve(unsigned value);
	void insert(T value);

	iterator begin();
	iterator end();

private:
	Deque deque;

	void addBucket();
};

