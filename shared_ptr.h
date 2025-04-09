//
// Created by 51275 on 25-4-9.
//

#ifndef SHARED_PTR_H
#define SHARED_PTR_H
#pragma once
#include <iostream>
#include <functional>
#include <memory>
#include <stdexcept>

// Forward declaration
template <typename T>
class shared_ptr;


struct ControlBlock {
    size_t ref_count;
    void* ptr;
    std::function<void(void*)> deleter;

    ControlBlock() : ref_count(0), ptr(nullptr),
                    deleter([](void*){}) {}

    ControlBlock(size_t ref_count, void* ptr)
        : ref_count(ref_count), ptr(ptr),
          deleter([](void* p) {}) {}

    ControlBlock(size_t ref_count, void* ptr, std::function<void(void*)> d)
        : ref_count(ref_count), ptr(ptr), deleter(std::move(d)) {}

    ~ControlBlock() = default;
};

template <typename T>
class shared_ptr {
    ControlBlock* control_block;
    T* ptr;
    void release();
public:
    // Constructors
    shared_ptr() noexcept : control_block(nullptr), ptr(nullptr) {}
    explicit shared_ptr(T* p);
    shared_ptr(const shared_ptr& other) noexcept;
    shared_ptr(shared_ptr&& other) noexcept;

    template <typename... Args>
    static shared_ptr<T> make_shared(Args&&... args);
    // Destructor
    ~shared_ptr();

    // Assignment operators
    shared_ptr& operator=(const shared_ptr& other) noexcept;
    shared_ptr& operator=(shared_ptr&& other) noexcept;

    // Pointer operations
    T& operator*() const;
    T* operator->() const;
    T* get() const noexcept;
    void reset(T* p = nullptr);
    [[nodiscard]] size_t use_count() const noexcept;

    // Type conversion
    explicit operator bool() const noexcept;

    // Swap two smart pointers
    void swap(shared_ptr& other) noexcept;

    // Comparison operations
    bool operator==(const shared_ptr& other) const noexcept;
    bool operator!=(const shared_ptr& other) const noexcept;

};

template <typename T>
shared_ptr<T>::shared_ptr(T* p) : control_block(new ControlBlock(1, p)), ptr(p) {}

template <typename T>
shared_ptr<T>::shared_ptr(const shared_ptr& other) noexcept : control_block(other.control_block), ptr(other.ptr) {
    if (control_block) {
        control_block->ref_count++;
    }
}
template <typename T>
shared_ptr<T>::shared_ptr(shared_ptr&& other) noexcept : control_block(other.control_block), ptr(other.ptr) {
    other.control_block = nullptr;
    other.ptr = nullptr;
}

template <typename T>
template <typename... Args>
shared_ptr<T> shared_ptr<T>::make_shared(Args&&... args) {
    return shared_ptr<T>(new T(std::forward<Args>(args)...));
}

template <typename T>
shared_ptr<T>::~shared_ptr() {
    release();
}

template <typename T>
void shared_ptr<T>::release() {
    if (control_block) {
        control_block->ref_count--;
        if (control_block->ref_count == 0) {
            if (ptr) {
                control_block->deleter(ptr);
            }
            delete control_block;
        }
    }
}


template <typename T>
shared_ptr<T>& shared_ptr<T>::operator=(const shared_ptr& other) noexcept {
    if (this != &other) {
        release();
        control_block = other.control_block;
        ptr = other.ptr;
        if (control_block) {
            control_block->ref_count++;
        }
    }
    return *this;
}

template <typename T>
shared_ptr<T>& shared_ptr<T>::operator=(shared_ptr&& other) noexcept {
    if (this != &other) {
        release();
        control_block = other.control_block;
        ptr = other.ptr;
        other.control_block = nullptr;
        other.ptr = nullptr;
    }
    return *this;
}

template <typename T>
T& shared_ptr<T>::operator*() const { // Dereference operator
    if (ptr == nullptr) {
        throw std::runtime_error("Attempting to dereference a null pointer");
    }
    return *ptr;
}

template <typename T>
T* shared_ptr<T>::operator->() const { // Arrow operator for accessing members
    return ptr;
}

template <typename T>
T* shared_ptr<T>::get() const noexcept {
    return ptr;
}

template <typename T>
void shared_ptr<T>::reset(T* p) {
    release();
    ptr = p;
    control_block = new ControlBlock(1, p,
        [](void* p) { delete static_cast<T*>(p); });
}

template <typename T>
size_t shared_ptr<T>::use_count() const noexcept {
    if(control_block) {
        return control_block->ref_count;
    }
    return 0;
}

template <typename T>
bool shared_ptr<T>::operator==(const shared_ptr& other) const noexcept {
    return ptr == other.ptr;
}

template <typename T>
bool shared_ptr<T>::operator!=(const shared_ptr& other) const noexcept {
    return ptr != other.ptr;
}

template <typename T>
shared_ptr<T>::operator bool() const noexcept {
    return ptr != nullptr;
}

template <typename T>
void shared_ptr<T>::swap(shared_ptr& other) noexcept {
    std::swap(control_block, other.control_block);
    std::swap(ptr, other.ptr);
}

#endif //SHARED_PTR_H
