#include "vector.h"
// 扩容函数实现
template <typename T>
void Vector<T>::reserve(size_t new_capacity) {
    if (new_capacity <= vec_capacity) return;  // 只在需要扩容时执行

    T* new_data = allocator.allocate(new_capacity);
    // 移动现有元素
    for (size_t i = 0; i < vec_size; i++) {
        allocator.construct(&new_data[i], std::move(data[i]));
        allocator.destroy(&data[i]);
    }
    // 释放旧内存
    if (data) {
        allocator.deallocate(data, vec_capacity);
    }
    data = new_data;
    vec_capacity = new_capacity;
}

// 快速排序函数实现
template <typename T>
template <typename Compare>
void Vector<T>::quick_sort(iterator first, iterator last, Compare cmp) {
    if (first >= last) return;
    // 选择基准元素（此处选择中间元素）
    T pivot = *(first + (last - first) / 2);
    iterator left = first;
    iterator right = last - 1;

    // 分区操作
    while (left <= right) {
        while (cmp(*left, pivot)) ++left;
        while (cmp(pivot, *right)) --right;
        if (left <= right) {
            std::swap(*left, *right);
            ++left;
            --right;
        }
    }
    // 递归排序
    quick_sort(first, right + 1, cmp);
    quick_sort(left, last, cmp);
}

// 默认构造函数实现
template <typename T>
Vector<T>::Vector() noexcept : data(nullptr), vec_size(0), vec_capacity(0) {}

// 显式构造函数实现
template <typename T>
Vector<T>::Vector(size_t n) : data(nullptr), vec_size(0), vec_capacity(0) {
    reserve(n);
    vec_size = n;
    for (size_t i = 0; i < n; ++i) {
        allocator.construct(&data[i]);
    }
}

// 直接构造函数实现
template <typename T>
Vector<T>::Vector(size_t n, const T& val) : data(nullptr), vec_size(0), vec_capacity(0) {
    reserve(n);
    vec_size = n;
    std::uninitialized_fill_n(data, n, val);
}

// 初始化列表构造函数实现
template <typename T>
Vector<T>::Vector(std::initializer_list<T> init) : data(nullptr), vec_size(0), vec_capacity(0) {
    if (init.size() > 0) {
        reserve(init.size());
        for (size_t i = 0; i < init.size(); ++i) {
            allocator.construct(&data[i], *(init.begin() + i));
        }
        vec_size = init.size();
    }
}

// 迭代器构造函数实现
template <typename T>
Vector<T>::Vector(iterator begin, iterator end) : data(nullptr), vec_size(0), vec_capacity(0) {
    size_t count = end - begin;
    reserve(count);
    for (size_t i = 0; i < count; ++i) {
        allocator.construct(&data[i], *(begin + i));
    }
    vec_size = count;
}

// 拷贝构造函数实现
template <typename T>
Vector<T>::Vector(const Vector<T>& other) : data(nullptr), vec_size(0), vec_capacity(0) {
    reserve(other.vec_size);
    for (size_t i = 0; i < other.vec_size; ++i) {
        allocator.construct(&data[i], other.data[i]);
    }
    vec_size = other.vec_size;
}

// 移动构造函数实现
template <typename T>
Vector<T>::Vector(Vector<T>&& other) noexcept : 
    data(other.data),
    vec_size(other.vec_size),
    vec_capacity(other.vec_capacity),
    allocator(std::move(other.allocator)) {
    // 防止 other 析构时释放我们刚"偷"来的内存
    other.data = nullptr;
    other.vec_size = 0;
    other.vec_capacity = 0;
}

// 拷贝赋值运算符实现
template <typename T>
Vector<T>& Vector<T>::operator=(const Vector<T>& other) {
    if (this != &other) {
        // 如果现有容量足够，直接复用内存
        if (vec_capacity >= other.vec_size) {
            // 复制共同部分
            for (size_t i = 0; i < std::min(vec_size, other.vec_size); ++i) {
                data[i] = other.data[i];
            }
            
            // 如果新大小更大，构造额外元素
            for (size_t i = vec_size; i < other.vec_size; ++i) {
                allocator.construct(&data[i], other.data[i]);
            }
            
            // 如果新大小更小，销毁多余元素
            for (size_t i = other.vec_size; i < vec_size; ++i) {
                allocator.destroy(&data[i]);
            }
        } else {
            // 需要重新分配内存
            clear();
            reserve(other.vec_size);
            for (size_t i = 0; i < other.vec_size; ++i) {
                allocator.construct(&data[i], other.data[i]);
            }
        }
        
        vec_size = other.vec_size;
    }
    return *this;
}

// 移动赋值运算符实现
template <typename T>
Vector<T>& Vector<T>::operator=(Vector<T>&& other) noexcept {
    if (this != &other) {
        clear();  // 释放当前资源
        
        // 窃取资源
        data = other.data;
        vec_size = other.vec_size;
        vec_capacity = other.vec_capacity;
        allocator = std::move(other.allocator);
        
        // 重置 other
        other.data = nullptr;
        other.vec_size = 0;
        other.vec_capacity = 0;
    }
    return *this;
}

// 析构函数实现
template <typename T>
Vector<T>::~Vector() {
    clear();
}

// 迭代器函数实现
template <typename T>
typename Vector<T>::iterator Vector<T>::begin() {
    return data;
}

template <typename T>
typename Vector<T>::const_iterator Vector<T>::const_begin() const {
    return data;
}

template <typename T>
typename Vector<T>::iterator Vector<T>::end() {
    return data + vec_size;
}

template <typename T>
typename Vector<T>::const_iterator Vector<T>::const_end() const {
    return data + vec_size;
}

// 访问元素函数实现
template <typename T>
T& Vector<T>::operator[](size_t index) {
    if (index >= vec_size) {
        throw std::out_of_range("Vector::operator[]");
    }
    return *(data+index);
}

template <typename T>
T& Vector<T>::at(size_t index) {
    if (index >= vec_size) {
        throw std::out_of_range("Vector::at");
    }
    return *(data+index);
}

// 容量和大小函数实现
template <typename T>
size_t Vector<T>::capacity() const {
    return vec_capacity;
}

template <typename T>
bool Vector<T>::empty() const {
    return vec_size == 0;
}

template <typename T>
size_t Vector<T>::size() const {
    return vec_size;
}

// 清空函数实现
template <typename T>
void Vector<T>::clear() {
    for (size_t i = 0; i < vec_size; i++) {
        allocator.destroy(&data[i]);
    }
    if (data) {
        allocator.deallocate(data, vec_capacity);
    }
    data = nullptr;
    vec_size = 0;
    vec_capacity = 0;
}

// 添加元素函数实现
template <typename T>
void Vector<T>::push_back(const T& value) {
    if (vec_size == vec_capacity) {
        size_t new_capacity = (vec_capacity == 0) ? 1 : vec_capacity + (vec_capacity / 2);
        reserve(new_capacity);
    }
    allocator.construct(data+vec_size, value);
    ++vec_size;
}

// 查找元素函数实现
template <typename T>
typename Vector<T>::const_iterator Vector<T>::find(const T& value) const {
    const_iterator it = const_begin();
    for (; it != const_end(); ++it) {
        if (*it == value) {
            break;
        }
    }
    return it;
}

// 插入元素函数实现
template <typename T>
void Vector<T>::insert(iterator pos, const T& value) {
    size_t index = pos - begin();
    if (vec_size == vec_capacity) {
        size_t new_capacity = (vec_capacity == 0) ? 1 : vec_capacity + (vec_capacity / 2);
        reserve(new_capacity);
    }
    for (size_t i = vec_size; i > index; --i) {
        allocator.construct(&data[i], std::move(data[i-1]));
    }
    allocator.construct(&data[index], value);
    ++vec_size;
}

// 删除元素函数实现
template <typename T>
void Vector<T>::erase(iterator pos) {
    size_t index = pos - begin();
    for (size_t i = index; i < vec_size - 1; ++i) {
        allocator.construct(&data[i], std::move(data[i+1]));
    }
    allocator.destroy(&data[vec_size-1]);
    --vec_size;
}

// 删除最后一个元素函数实现
template <typename T>
void Vector<T>::pop_back() {
    if(vec_size == 0) {
        throw std::out_of_range("Vector::pop_back");
    }
    allocator.destroy(&data[vec_size-1]);
    --vec_size;
}

// 访问首尾元素函数实现
template <typename T>
T& Vector<T>::front() const {
    if(vec_size == 0) {
        throw std::out_of_range("Vector::front");
    }
    return data[0];
}

template <typename T>
T& Vector<T>::back() const {
    if(vec_size == 0) {
        throw std::out_of_range("Vector::back");
    }
    return data[vec_size-1];
}

// 交换函数实现
template <typename T>
void Vector<T>::swap(Vector& other) {
    std::swap(data, other.data);
    std::swap(vec_size, other.vec_size);
    std::swap(vec_capacity, other.vec_capacity);
}

// 输出流运算符实现
template <typename T>
std::ostream& operator<<(std::ostream& os, const Vector<T>& vec) {
    for (size_t i = 0; i < vec.vec_size; ++i) {
        os << vec.data[i] << " ";
    }
    return os;
}

// 快速排序函数实现
template <typename T>
template <typename Compare>
void Vector<T>::sort(iterator first, iterator last, Compare cmp) {
    quick_sort(first, last, cmp);
}

template <typename T>
template <typename Compare>
void Vector<T>::sort(Compare cmp) {
    quick_sort(begin(), end(), cmp);
}