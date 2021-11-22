#include "Vector.h"

template<typename T>
void Vector<T>::expand() {
    if (_size < _capacity) return;
    if (_capacity < DEFAULT_CAPACITY)
        _capacity = DEFAULT_CAPACITY;

    T* oldElem = _elem;
    _elem = new T[_capacity <<= 1];
    for (int i = 0; i < _size; ++i)
        _elem[i] = oldElem[i];

    delete[] oldElem;
}

template<typename T>
void Vector<T>::copyFrom(const T *A, Rank lo, Rank hi) {
    _elem = new T[_capacity = 2 * (hi - lo)];   // 分配空间
    _size = 0;                                  // 规模清零
    while (lo < hi)                             // A[lo, hi)内的元素逐一
        _elem[_size++] = A[lo++];               // 复制至_elem[0, hi - lo)
}


template<typename T>
Vector<T>& Vector<T>::operator=(const Vector<T>& V) {
    if (_elem)
        delete[] _elem;
    copyFrom(V._elem, 0, _size);
    return *this;
}