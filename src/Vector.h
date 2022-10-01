#ifndef SRC_VECTOR_H
#define SRC_VECTOR_H

#define DEFAULT_CAPACITY 3   // 默认初始容量

namespace Vector {
    using Rank = int;       // 秩

    template<typename T> class Vector {
    protected:
        T* _elem;           // 数据
        Rank _size;         // 规模
        Rank _capacity;     // 容量

        void expand();                                      // 空间不足扩容
        void shrink();                                      // 装填过小压缩
        void copyFrom(T const* A, Rank lo, Rank hi);        // 复制数组区间

        Rank maxItem(Rank lo, Rank hi);                     // 选取最大元素
        Rank partition(Rank lo, Rank hi);                   // 轴点构造算法

        void selectionSort(Rank lo, Rank hi);               // 选择排序

        bool bubble(Rank lo, Rank hi);                      // 扫描交换
        void bubbleSort(Rank lo, Rank hi);                  // 起泡排序

        void merge(Rank lo, Rank mid, Rank hi);             // 归并算法
        void mergeSort(Rank lo, Rank hi);                   // 归并排序

        void heapSort(Rank lo, Rank hi);                    // 堆排序
        void quickSort(Rank lo, Rank hi);                   // 快速排序
        void shellSort(Rank lo, Rank hi);                   // 希尔排序

    public:
        /* 构造函数 */
        Vector(int c = DEFAULT_CAPACITY, Rank s = 0, T v = 0) {
            _elem = new T[_capacity = c];
            for (_size = 0; _size < s; ++_size)
                _elem[_size] = v;
        }

        Vector(T const* A, Rank n) { copyFrom(A, 0, n); }
        Vector(T const* A, Rank lo, Rank hi) { copyFrom(A, lo, hi); }
        Vector(Vector<T> const& V) { copyFrom(V._elem, 0, V._size); }
        Vector(Vector<T> const& V, Rank lo, Rank hi) { copyFrom(V._elem, lo, hi); }

        /* 析构函数 */
        ~Vector() { delete[] _elem; }

        /* 只读接口 */
        Rank size() const { return _size; }                           // 规模
        bool empty() const { return !_size; }                         // 判空

        Rank find(T const& e) const { return find(e, 0, _size); }       // 无序向量整体查找
        Rank find(T const& e, Rank lo, Rank hi);                       // 无序向量区间查找

        Rank search(T const& e) const {                               // 有序向量整体查找
            return (_size <= 0)? -1: search(e, 0, _size);
        }
        Rank search(T const& e, Rank lo, Rank hi) const;              // 有序向量区间查找

        /* 可写接口 */
        T& operator[] (Rank r);                                       // 重载下标操作符
        const T& operator[] (Rank r) const;
        Vector<T>& operator= (Vector<T> const&);                      // 重载赋值操作符

        T remove(Rank r);                                             // 删除单一元素
        int remove(Rank lo, Rank hi);                                 // 删除区间元素

        Rank insert(Rank r, T const& e);                              // 插入元素
        Rank insert(T const& e) { return insert(_size, e); }          // 插入末元素

        void sort(Rank lo, Rank hi);                                  // 区间排序
        void sort() { sort(0, _size); }                               // 整体排序

        void unsort(Rank lo, Rank hi);                                // 区间置乱
        void unsort() { unsort(0, _size); }                           // 整体置乱

        Rank deduplicate();                                           // 无序去重
        Rank uniquify();                                              // 有序去重

        /* 遍历接口 */
        void traverse(void(*) (T&));                                  // 函数指针遍历
        template<typename VST> void traverse(VST&);                   // 函数对象遍历
    };
}

#endif //SRC_VECTOR_H