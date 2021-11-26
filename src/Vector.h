#ifndef SRC_VECTOR_H
#define SRC_VECTOR_H

typedef int Rank;               // 秩
const int DEFAULT_CAPACITY = 3; // 默认初始容量

template<typename T> class Vector {
protected:
    Rank _size;         // 规模
    int _capacity;      // 容量
    T* _elem;           // 数据区

    void expand();      // 空间不足时扩容
    void shrink();      // 装填因子过小时压缩
    void copyFrom(T const* A, Rank lo, Rank hi);    // 复制数组区间A[lo, hi)
    bool bubble(Rank lo, Rank hi);                  // 扫描交换
    void bubbleSort(Rank lo, Rank hi);              // 起泡排序算法
    Rank max(Rank lo, Rank hi);                     // 选取最大元素
    void selectionSort(Rank lo, Rank hi);           // 选择排序算法
    void merge(Rank lo, Rank mi, Rank hi);          // 归并算法
    void mergeSort(Rank lo, Rank hi);               // 归并排序算法
    Rank partition(Rank lo, Rank hi);               // 轴点构造算法
    void quickSort(Rank lo, Rank hi);               // 快速排序算法
    void heapSort(Rank lo, Rank hi);                // 堆排序

public:
// 构造函数
    Vector(int c = DEFAULT_CAPACITY, int s = 0, T v = 0) {
        _elem = new T[_capacity = c];
        for (_size = 0; _size < s; _elem[_size++] = v);
    }
    Vector(T const* A, Rank n) { copyFrom(A, 0, n); }                           // 数组整体复制
    Vector(T const* A, Rank lo, Rank hi) { copyFrom(A, lo, hi); }                   // 数组区间复制
    Vector(Vector<T> const& V) { copyFrom(V._elem, 0, V._size); }               // 向量整体复制
    Vector(Vector<T> const& V, Rank lo, Rank hi) { copyFrom(V._elem, lo, hi); }     // 向量区间复制

// 析构函数
    ~Vector() { delete []_elem; }

// 只读访问接口
    Rank size() const { return _size; }     // 规模
    bool empty() const { return !_size; }   // 判空
    int disoedered() const;                 // 判断向量是否已排序
    Rank find(T const& e) const { return find(e, 0, _size); }   // 无序向量整体查找
    Rank find(T const& e, Rank lo, Rank hi) const;              // 无序向量区间查找
    Rank search(T const& e) const {                             // 有序向量整体查找
        return (_size <= 0)? -1 : search(e, 0, _size);
    }
    Rank search(T const& e, Rank lo, Rank hi) const;            // 有序向量区间查找

// 可写访问接口
    T& operator[](Rank r) const;                            // 重载下标操作符
    Vector<T>& operator=(Vector<T> const&);                // 重载赋值操作符
    T remove(Rank r);                                       // 删除秩为r的元素
    int remove(Rank lo, Rank hi);                           // 删除秩在区间[lo, hi)的元素
    Rank insert(Rank r, T const& e);                        // 插入元素
    Rank insert(T const& e) { return insert(_size, e); }    // 默认作为末元素插入
    void sort(Rank lo, Rank hi);                            // 区间排序
    void sort() { sort(0, _size); }                         // 整体排序
    void unsort(Rank lo, Rank hi);                          // 区间置乱
    void unsort() { unsort(0, _size); }                     // 整体置乱
    int deduplicate();                                      // 无序去重
    int uniquify();                                         // 有序去重

// 遍历
    void traverse();                                // 使用函数指针遍历
    template<typename VST> void traverse(VST&);     // 使用函数对象遍历
};


#endif //SRC_VECTOR_H
