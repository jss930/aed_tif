#ifndef __SORT_H__
#define __SORT_H__

#include "myvector.h"

template <typename T>
void merge(Vector<T> &arr, int left, int mid, int right, bool (*compare)(const T &, const T &))
{
    int n1 = mid - left + 1;
    int n2 = right - mid;

    Vector<T> L(n1), R(n2);
    for (int i = 0; i < n1; ++i)
        L.push_back(arr[left + i]);
    for (int j = 0; j < n2; ++j)
        R.push_back(arr[mid + 1 + j]);

    int i = 0, j = 0, k = left;
    while (i < L.getSize() && j < R.getSize())
    {
        if (compare(L[i], R[j]))
        {
            arr[k++] = L[i++];
        }
        else
        {
            arr[k++] = R[j++];
        }
    }

    while (i < L.getSize())
        arr[k++] = L[i++];
    while (j < R.getSize())
        arr[k++] = R[j++];
}

template <typename T>
void mergeSort(Vector<T> &arr, int left, int right, bool (*compare)(const T &, const T &))
{
    if (left < right)
    {
        int mid = (left + right) / 2;
        mergeSort(arr, left, mid, compare);
        mergeSort(arr, mid + 1, right, compare);
        merge(arr, left, mid, right, compare);
    }
}

template <typename T>
void sort(Vector<T> &arr, bool (*compare)(const T &, const T &))
{
    if (arr.getSize() > 1)
        mergeSort(arr, 0, arr.getSize() - 1, compare);
}

#endif