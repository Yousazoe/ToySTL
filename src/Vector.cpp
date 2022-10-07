#include <algorithm>
#include "Vector.h"

namespace Vector {

    template <typename T>
    void Vector<T>::expand() {                                  // 空间不足扩容
        if (_size < _capacity) return;                          // 尚未满员不必扩容
        _capacity = std::max(_capacity, DEFAULT_CAPACITY);

        T* oldElem = _elem;
        _elem = new T[_capacity <<= 1];                         // 容量加倍

        for (int i = 0; i < _size; ++i)
            _elem[i] = oldElem[i];                              // 复制原向量内容
        delete[] oldElem;                                       // 释放原空间
    }

    template <typename T>
    void Vector<T>::shrink() {                                  // 装填过小压缩
        if (_capacity < DEFAULT_CAPACITY << 1) return;
        if (_size << 2 > _capacity) return;

        T* oldElem = _elem;
        _elem = new T[_capacity >>= 1];                         // 容量减半

        for (int i = 0; i < _size; ++i)
            _elem[i] = oldElem[i];                              // 复制原向量内容
        delete[] _elem;                                         // 释放原空间
    }

    template <typename T>
    void Vector<T>::copyFrom(const T *A, Rank lo, Rank hi) {    // 复制数组区间
        _elem = new T[_capacity = 2*(hi - lo)];
        _size = 0;                                              // 规模清零

        while (lo < hi)
            _elem[_size++] = A[lo++];                           // A[lo, hi)元素逐一复制至_elem[0, hi - lo)
    }

    template <typename T>
    Vector<T>& Vector<T>::operator=(const Vector<T> & V) {       // 重载赋值操作符
        if (_elem)
            delete[] _elem;                                      // 释放原有内容

        copyFrom(V._elem, 0, V.size());
        return *this;                                            // 返回当前对象引用
    }

    /* 只读接口 */
    template <typename T>
    Rank Vector<T>::find(T const& e, Rank lo, Rank hi) {          // 无序向量区间查找
        while ((lo < hi--) && (_elem[hi] != e));                 // 从后向前顺序查找
        return hi;                                               // hi < lo 则意味失败；否则 hi 即命中元素的秩
    }


    /* 可写接口 */
    template <typename T>
    T& Vector<T>::operator[](Rank r) const {                     // 重载下标操作符
        return _elem[r];
    }

    template <typename T>
    T Vector<T>::remove(Rank r) {                                // 删除单一元素
        T e = _elem[r];                                          // 备份删除元素
        remove(r, r + 1);                                        // 调用区间删除
        return e;                                                // 返回删除元素
    }

    template <typename T>
    int Vector<T>::remove(Rank lo, Rank hi) {                    // 删除区间元素
        if (lo == hi) return 0;                                  // 处理退化情况

        while(hi < _size)
            _elem[lo++] = _elem[hi++];                           // [hi, _size) 顺次前移 hi-lo 位

        _size = lo;                                              // 更新规模
        shrink();                                                // 如有必要缩容
        return hi - lo;                                          // 返回被删除元素的数目
    }

    template <typename T>
    Rank Vector<T>::insert(Rank r, T const& e) {                 // 插入元素
        expand();                                                // 如有必要扩容
        for (int i = _size; i > r; --i)
            _elem[i] = _elem[i - 1];                             // 后继元素顺次后移一个单元

        _elem[r] = e;                                            // 插入元素
        _size++;                                                 // 更新规模

        return r;                                                // 返回秩
    }

    template <typename T>
    void permute(Vector<T>& V) {
        for (int i = V.size(); i > 0; --i)                       // 自后向前
            std::swap(V[i - 1], V[rand() % i]);                  // V[i - 1] 与 V[0, i) 中某一随机元素交换
    }

    template <typename T>
    void Vector<T>::unsort(Rank lo, Rank hi) {
        T* V = _elem + lo;                                       // 将子向量 _elem[lo, hi) 视作另一个向量 [0, hi - lo)

        for (int i = hi - lo; i > 0; --i)                        // 自后向前
            std::swap(V[i - 1], V[rand() % i]);                  // V[i - 1] 与 V[0, i) 中某一随机元素交换
    }

    template <typename T>
    int Vector<T>::deduplicate() {                               // 无序去重
        int oldSize = _size;                                     // 记录原规模
        Rank i = 1;
        while (i < _size) {                                      // 自前向后逐一考察各元素
            (find(_elem[i], 0, i) < 0)?                           // 在前缀中寻找雷同者
                i++:                                             // 若无雷同者继续考察其后继
                remove(i);                                       // 否则删除雷同者
        }

        return oldSize - _size;                                  // 返回被删除元素数目
    }

    /* 遍历接口 */
    template <typename T>
    void Vector<T>::traverse(void (*visit) (T&)) {
        for (int i = 0; i < _size; ++i)
            visit(_elem[i]);
    }

    template <typename T> template <typename VST>
    void Vector<T>::traverse(VST& visit) {
        for (int i = 0; i < _size; ++i)
            visit(_elem[i]);
    }
}