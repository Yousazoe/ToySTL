#include <algorithm>
#include "Vector.h"

namespace Vector {

    template <typename T>
    void Vector<T>::expand() {
        if (_size < _capacity) return;
        _capacity = std::max(_capacity, DEFAULT_CAPACITY);

        T* oldElem = _elem;
        _elem = new T[_capacity <<= 1];

        for (int i = 0; i < _size; ++i)
            _elem[i] = oldElem[i];
        delete[] oldElem;
    }

    template <typename T>
    void Vector<T>::copyFrom(const T *A, Rank lo, Rank hi) {
        _elem = new T[_capacity = 2*(hi - lo)];
        _size = 0;

        while (lo < hi)
            _elem[_size++] = A[lo++];
    }


}