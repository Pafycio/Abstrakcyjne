#pragma once
#include <algorithm>
#include <iostream>

// Bubble Sort 

template<int N> 
inline void loop(int *data) {
	if (data[0]>data[1]) std::swap(data[0], data[1]);
	loop<N - 1>(++data);
}

template<> 
inline void loop<0>(int *data) {};

template<int N>  
inline void bubble_sort_template(int * data) {
	loop<N - 1>(data);
	bubble_sort_template<N - 1>(data);
}

template<>  
inline void bubble_sort_template<2>(int * data) {
	loop<1>(data);
};

template<>  
inline void bubble_sort_template<1>(int * data) {
};

// Silnia

template < int N >
struct silnia {
	static long long int const value = N*silnia<N - 1>::value;
};

template < >
struct silnia<0> {
	static long long int const value = 1;
};


// Ciag fibonacciego

template <int N>
struct fibbonaci {
	enum { val = fibbonaci<N-1>::val+fibbonaci<N-2>::val };
};

template <>
struct fibbonaci<0> {
	enum { val = 0 };
};

template <>
struct fibbonaci<1>{
	enum { val = 1};
};

template <>
struct fibbonaci<2> {
	enum { val = 1 };
};

// Liczba Pi rekurencyjnie

template <unsigned long N>
struct CalculatePi
{
	constexpr static double pi = CalculatePi<N - 1>::pi + (1.0 / ((4.0*N - 1.0)*(4.0*N - 3.0)));
};

template <>
struct CalculatePi<1>
{
	constexpr static double pi = (1.0/3.0);
};

// Symbol Newton

template <unsigned N, unsigned K>
struct Newton
{
	const static int value = (Newton<N - 1, K - 1>::value + Newton<N - 1, K>::value);
};

template <>
struct Newton<0, 0>
{
	const static int value = 1;
};

template <unsigned N>
struct Newton<N, 0>
{
	const static int value = 1;
};

template <unsigned N>
struct Newton<N , N>
{
	const static int value = 1;
};

template<int N, int K>
inline int newton()
{
	return Newton<N, K>::value;
}

// Najwiekszy wspolny dzielnik

template <unsigned A, unsigned B>
struct NWD
{
	const static int value = NWD<B, A%B>::value;
};

template<unsigned A>
struct NWD<A, 0>
{
	const static int value = A;
};