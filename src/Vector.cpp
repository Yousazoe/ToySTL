#include <stdlib.h>
#include "Vector.h"

template<typename T>
void Vector<T>::expand() {                  // 向量空间不足时扩容
    if (_size < _capacity) return;          // 尚未满员时不必扩容
    if (_capacity < DEFAULT_CAPACITY)
        _capacity = DEFAULT_CAPACITY;

    T* oldElem = _elem;
    _elem = new T[_capacity <<= 1];         // 容量加倍
    for (int i = 0; i < _size; ++i)         // 复制原向量内容
        _elem[i] = oldElem[i];

    delete[] oldElem;                       // 释放原空间
}

template<typename T>
void Vector<T>::shrink() {
    if (_capacity < DEFAULT_CAPACITY << 1) return;
    if (_size << 2 > _capacity) return;

    T* oldElem = _elem;
    _elem = new T[_capacity >>= 1];
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


// 只读访问接口
template<typename T>
Rank Vector<T>::find(const T &e, Rank lo, Rank hi) const {
    while ((lo < hi--) && (e != _elem[hi]))     // 从后向前顺序查找
    return hi;                                  // hi为命中元素的秩
}

template<typename T>
int Vector<T>::disoedered() const {             // 返回向量中逆序相邻元素对的总数
    int n = 0;
    for (int i = 0; i < _size; ++i)             // 逐一检查_size-1对相邻元素
        if (_elem[i - 1] > _elem[i])            // 逆序计数
            n++;
    return n;                                   // 向量有序当且仅当n = 0
}

template<typename T>
Rank Vector<T>::search(const T &e, Rank lo, Rank hi) const {
    return binSearch(_elem, e, lo, hi);
}

template<typename T>
static Rank binSearch(T* A, T const& e, Rank lo, Rank hi) { // 二分查找算法：在有序区间[lo, hi)内查找元素
    while (lo < hi) {
        Rank mi = (lo + hi) >> 1;               // 以中点为轴点
        if (e < A[mi])                          // [lo, mi)
            hi = mi;
        else if (e > A[mi])                     // [mi, hi)
            lo = mi + 1;
        else
            return mi;
    }
    return -1;                                  // 查找失败
}


// 可写访问接口
template<typename T>
T& Vector<T>::operator[](Rank r) const {        // 重载下标操作符
    return _elem[r];                            // assert: 0 <= r < _size
}

template<typename T>
Vector<T>& Vector<T>::operator=(const Vector<T>& V) {
    if (_elem)                                  // 释放原有内容
        delete[] _elem;
    copyFrom(V._elem, 0, _size);            // 整体复制
    return *this;                               // 返回当前对象的引用
}

template<typename T>
Rank Vector<T>::insert(Rank r, const T &e) {    // 将e作为秩为r元素插入
    expand();                                   // 如有扩容必要
    for (int i = _size; i > r; --i)             // 自后向前
        _elem[i] = _elem[i - 1];                // 后续元素依次后移

    _elem[r] = e;                               // 置入新元素
    _size++;                                    // 更新容量
    return r;                                   // 返回秩
}

template<typename T>
int Vector<T>::remove(Rank lo, Rank hi) {       // 删除区间[lo, hi)
    if (lo == hi) return 0;
    while (lo < hi)
        _elem[lo++] = _elem[hi++];              // [hi, _size)顺次后移

    _size = lo;                                 // 更新规模
    shrink();                                   // 如有必要缩容
    return hi - lo;                             // 返回被删除元素的数目
}

template<typename T>
T Vector<T>::remove(Rank r) {                   // 删除向量中秩为r的元素
    T e = _elem[r];                             // 备份被删除元素
    remove(r, r + 1);                           // 调用区间删除算法
    return e;                                   // 返回被删除元素
}

template<typename T>
void permute(Vector<T>& V) {                    // 随机置乱向量
    for (int i = V.size(); i > 0; --i)          // 自后向前
        swap(V[i - 1], V[rand() % i]);          // V[i - 1]与V[0, i)中某一元素随机交换
}

template<typename T>
void Vector<T>::unsort(Rank lo, Rank hi) {      // 随机置乱区间
    T* V = _elem + lo;                          // 将_elem[lo, hi)视为V[0, hi - lo)
    for (Rank i = hi - lo; i > 0; --i)          // 自后向前
        swap(V[i - 1], V[rand() % i]);          // V[i - 1]与V(0, i]中某一元素随机交换
}

template<typename T> static bool lt(T* a, T* b) { return lt(*a, *b); }      // less than
template<typename T> static bool lt(T& a, T& b) { return a < b; }
template<typename T> static bool eq(T* a, T* b) { return eq(*a, *b); }      // equal
template<typename T> static bool eq(T& a, T& b) { return a == b; }



template<typename T>
void Vector<T>::sort(Rank lo, Rank hi) {
    switch (rand() % 5) {
        case 1:  bubbleSort(lo, hi);    break;                              // 起泡排序
        case 2:  selectionSort(lo, hi); break;                              // 选择排序
        case 3:  mergeSort(lo, hi);     break;                              // 归并排序
        case 4:  heapSort(lo, hi);      break;                              // 堆排序
        default: quickSort(lo, hi);     break;                              // 快速排序
    }
}

template<typename T>
void Vector<T>::bubbleSort(Rank lo, Rank hi) {                              // 向量的起泡排序
    while (!bubble(lo, hi--));
}

template<typename T>
bool Vector<T>::bubble(Rank lo, Rank hi) {
    bool sorted = true;
    while (++lo < hi) {
        if (_elem[lo - 1] < _elem[lo]) {
            sorted = false;
            swap(_elem[lo - 1], _elem[lo]);
        }
    }
    return sorted;
}


template<typename T>
void Vector<T>::mergeSort(Rank lo, Rank hi) {
    if (hi - lo < 2) return;
    int mi = (lo + hi)/ 2;
    mergeSort(lo, mi);
    mergeSort(mi, hi);
    merge(lo, mi, hi);
}


template<typename T>
int Vector<T>::deduplicate() {
    int oldSize = _size;                        // 记录原规模
    Rank i = 1;
    while (i < _size) {
        (find(_elem[i], 0, i) < 0)? i++: remove(i);
    }

    return oldSize - _size;                     // 返回向量规模变化量
}

template<typename T>
int Vector<T>::uniquify() {                     // 有序向量重复元素剔除
    Rank i = 0, j = 0;
    while (++j < _size)
        if (_elem[i] != _elem[j])
            _elem[++j] = _elem[i];

    _size = ++i;
    shrink();
    return j - i;                               // 向量规模变化量
}


// 遍历
template<typename T>
void Vector<T>::traverse(void (*visit)(T&)) {
    for (int i = 0; i < _size; ++i)
        visit(_elem[i])
}

template<typename T>
template<typename VST>
void Vector<T>::traverse(VST& visit) {
    for (int i = 0; i < _size; ++i)
        visit(_elem[i]);
}

template<typename T> struct Increase {
    virtual void operator() (T& e) { e++; }
};

template<typename T> void increase(Vector<T>& V) {
    V.traverse(Increase<T>());
}