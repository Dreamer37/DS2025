#include "Vector.h"
#include <cstdlib>

template <typename T>
void Vector<T>::quickSort(int lo, int hi) {
    if (hi - lo < 2) return;
    Rank mi = partition(lo, hi - 1);
    quickSort(lo, mi);
    quickSort(mi + 1, hi);
}

template <typename T>
Rank Vector<T>::partition(int lo, int hi) {
    swap(_elem[lo], _elem[lo + rand() % (hi - lo + 1)]);
    T pivot = _elem[lo];

    while (lo < hi) {
        while (lo < hi && _elem[hi] <= pivot) hi--;
        if (lo < hi) _elem[lo++] = _elem[hi];

        while (lo < hi && _elem[lo] >= pivot) lo++;
        if (lo < hi) _elem[hi--] = _elem[lo];
    }
    _elem[lo] = pivot;
    return lo;
}
