#include "SortedDeque.h"
#include <iostream>
#include <algorithm>

template<typename T, unsigned TSize>
T SortedDeque<T, TSize>::at(unsigned pos)
{
	return deque.at(0).at(pos).first;
}

template<typename T, unsigned TSize>
T SortedDeque<T, TSize>::back()
{

	return deque.at(0).back().first;
}

template <typename T, unsigned TSize>
bool SortedDeque<T, TSize>::empty()
{
	return this->deque.empty();
}

template <typename T, unsigned TSize>
unsigned SortedDeque<T, TSize>::size_total()
{
	int count = 0;
	for (unsigned i = 0; i < deque.size(); i++)
	{
		Bucket bucket = deque.at(i);
		for (unsigned j = 0; j < bucket.size(); j++)
		{
			count += bucket.at(j).second;
		}
	}
	return count;
}

template <typename T, unsigned TSize>
unsigned SortedDeque<T, TSize>::size_unique()
{
	int count = 0;
	for (auto it = deque.begin(); it != deque.end(); ++it)
	{
		count += (*it).size();
	}
	return count;
}

template <typename T, unsigned TSize>
unsigned SortedDeque<T, TSize>::capacity()
{
	return deque.size() * SIZE_OF_BUCKET;
}

template<typename T, unsigned TSize>
unsigned SortedDeque<T, TSize>::number_of_buckets()
{
	return deque.size();
}

template<typename T, unsigned TSize>
void SortedDeque<T, TSize>::reserve(unsigned value)
{
	maxCapacity = value;
	if (maxCapacity % SIZE_OF_BUCKET != 0)
		maxBucket = static_cast<unsigned>(ceil(maxCapacity / SIZE_OF_BUCKET)) + 1;
	else
		maxBucket = static_cast<unsigned>(ceil(maxCapacity / SIZE_OF_BUCKET));
}

template<typename T, unsigned TSize>
void SortedDeque<T, TSize>::insert(T value)
{
	bool added = false;
	if (deque.empty()) {
		addBucket();
	}

	for (unsigned i = 0; i < deque.size(); i++)
	{
		for (unsigned k = 0; k < deque.at(i).size(); k++)
		{
			if (value == deque.at(i).at(k).first)
			{
				deque.at(i).at(k).second++;
				added = true;
				break;
			}
			else if (value < deque.at(i).at(k).first)
			{

				deque.at(i).insert(deque.at(i).begin() + k, std::make_pair(value, 1));
				added = true;
				break;
			}
		}
		if (!added) {
			deque.at(i).push_back(std::make_pair(value, 1));
			sort(deque.at(i).begin(), deque.at(i).end());
			break;
		}

	}

	if (size_unique() > capacity()) {
		addBucket();
	}
}

template<typename T, unsigned TSize>
typename SortedDeque<T, TSize>::iterator SortedDeque<T, TSize>::begin()
{
	return deque.at(0).begin();
}

template<typename T, unsigned TSize>
typename SortedDeque<T, TSize>::iterator SortedDeque<T, TSize>::end()
{
	return deque.at(0).end();
}

template<typename T, unsigned TSize>
void SortedDeque<T, TSize>::addBucket()
{
	deque.push_back(Bucket());
}
