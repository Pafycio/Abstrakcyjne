#include "MetaFunctions.h"

MetaFunctions::MethaFunctions()
{
}


MetaFunctions::~MethaFunctions()
{
}

void loop(int *data, int N) {
	if (N>0) {
		if (data[0]>data[1]) swap(data[0], data[1]);
		loop(++data, N - 1);
	}
}

template<int N>  inline void bubble_sort_template(int * data) {
	loop<N - 1>(data);
	bubble_sort_template<N - 1>(data);
}
template<>  inline void bubble_sort_template<2>(int * data) {
	loop<1>(data);
};