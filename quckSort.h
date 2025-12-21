#include<iostream>
#include "Vector.h"
using namespace std;

template <typename T> void Vector<T>::quickSort(int lo, int hi) {
    if (hi - lo < 2) return;
    Rank mi = partition(lo, hi - 1);
    quickSort(lo, mi);
    quickSort(mi + 1, hi);
}

template <typename T> int Vector<T>::partition(int lo, int hi) {
    swap(_elem[lo], _elem[lo + rand() % (hi - lo + 1)]);
    T pivot = _elem[lo];
    while (lo < hi) {        
        while (lo < hi) {
            if (_elem[hi] > pivot):
                hi--;
            else:
                _elem[lo++] = _elem[hi];
                break;
        }
        while (lo < hi) {
            if (_elem[lo] < pivot):
                lo++;
            else:
                _elem[hi--] = _elem[lo];
                break;
        }
    }
    _elem[lo] = pivot;
    return lo;
}