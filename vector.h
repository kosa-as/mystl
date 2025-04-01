//
// Created by wangwenhai on 25-3-28.
//
#ifndef VECTOR_H
#define VECTOR_H

#include <memory>
#include <stdexcept>
#include <initializer_list>
#include <algorithm>
#include <iostream>
template <typename T>
class Vector {
    using iterator = T*;             // 普通迭代器
    using const_iterator = const T*; // const 迭代器
private:
    T* data;
    size_t vec_size;
    size_t vec_capacity;
    std::allocator<T> allocator;
    // 扩容函数声明
    void reserve(size_t new_capacity);
    // 快速排序函数声明
    template<typename Compare>
    void quick_sort(iterator first, iterator last, Compare cmp);
public:
    // 构造函数声明
    Vector() noexcept;
    explicit Vector(size_t n);
    Vector(size_t n, const T& val);
    Vector(std::initializer_list<T> init);
    Vector(iterator begin, iterator end);
    Vector(const Vector<T>& other);
    Vector(Vector<T>&& other) noexcept;
    // 赋值运算符声明
    Vector& operator=(const Vector<T>& other);
    Vector& operator=(Vector<T>&& other) noexcept;
    // 析构函数声明
    ~Vector();
    // 迭代器函数声明
    iterator begin();
    const_iterator const_begin() const;
    iterator end();
    const_iterator const_end() const;
    // 访问元素函数声明
    T& operator[](size_t index);
    T& at(size_t index);
    // 容量和大小函数声明
    size_t capacity() const;
    bool empty() const;
    size_t size() const;
    // 清空函数声明
    void clear();
    // 添加元素函数声明
    void push_back(const T& value);
    // 查找元素函数声明
    const_iterator find(const T& value) const;
    // 插入元素函数声明
    void insert(iterator pos, const T& value);
    // 删除元素函数声明
    void erase(iterator pos);
    void pop_back();
    // 访问首尾元素函数声明
    T& front() const;
    T& back() const;
    // 交换函数声明
    void swap(Vector& other);
    // 友元函数声明
    template <typename U>
    friend std::ostream& operator<<(std::ostream& os, const Vector<U>& vec);
    // 快速排序函数声明
    template<typename Compare = std::less<T>>
    void sort(iterator first, iterator last, Compare cmp = Compare());
    // 函数重写
    template<typename Compare = std::less<T>>
    void sort(Compare cmp = Compare());
};
#endif // VECTOR_H
