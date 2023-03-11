#ifndef QUICKSORT
#define QUICKSORT
#include <cstdlib>

//
// Created by Safina on 24.09.2022.
//
template< typename T >
void swap(T arr[], size_t a, size_t b)
{
	T t = arr[a];
	arr[a] = arr[b];
	arr[b] = t;
}

template< typename T, bool descending >
size_t partition(T arr[], size_t head, size_t tail)
{
	T pivot = arr[tail];	// pivot
	size_t i = head - 1;	// Index of smaller element and indicates
							// the right position of pivot found so far
	for (size_t j = head; j <= tail - 1; j++)
	{
		if (descending)
		{
			if (arr[j] > pivot)
			{
				i++;
				swap(arr, i, j);
			}
		}
		else
		{
			if (arr[j] < pivot)
			{
				i++;
				swap(arr, i, j);
			}
		}
	}
	swap(arr, i + 1, tail);
	return (i + 1);
}

template< typename T, bool descending >
void quickSort(T arr[], int head, int tail)
{
	while (head < tail)
	{
		size_t pi = partition< T, descending >(arr, head, tail);
		if (pi - head < tail - pi)
		{
			quickSort< T, descending >(arr, head, pi - 1);
			head = pi + 1;
		}
		else
		{
			quickSort< T, descending >(arr, pi + 1, tail);
			tail = pi - 1;
		}
	}
}
#endif