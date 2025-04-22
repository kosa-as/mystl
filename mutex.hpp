#pragma once
#include <stdexcept>
template <typename T>
class lock_guard {
    T* ptr_;
    public:
        lock_guard() = delete;

        template <typename U = T, 
                  typename = std::enable_if_t<
                      std::is_member_function_pointer<decltype(&U::lock)>::value 
                      && std::is_member_function_pointer<decltype(&U::unlock)>::value,
                      void
                  >>
        explicit lock_guard(T* ptr) : ptr_(ptr) {
            ptr_->lock();
        }
        
        ~lock_guard() {
            ptr_->unlock();    
        }
        lock_guard(const lock_guard&) = delete;
        lock_guard& operator=(const lock_guard&) = delete;
        lock_guard(lock_guard&& other) noexcept : ptr_(other.ptr_) {
            other.ptr_ = nullptr;
        }
        lock_guard& operator=(lock_guard&& other) noexcept {
            ptr_ = other.ptr_;
            other.ptr_ = nullptr;
            return *this;
        }
};

template <typename T>
class unique_lock {
    T* ptr_;
    public:
        unique_lock() = delete;

        template <typename  T, 
                  typename = std::enable_if_t<
                      std::is_member_function_pointer<decltype(&T::lock)>::value 
                      && std::is_member_function_pointer<decltype(&T::unlock)>::value
                      && std::is_member_function_pointer<decltype(&T::try_lock)>::value,
                      void
                  >>
        explicit unique_lock(T* ptr) : ptr_(ptr) {
            ptr_->lock();
        }
        ~unique_lock() {
            ptr_->unlock();
        }
        void lock() {
            if (!ptr_) {
                throw std::runtime_error("尝试对空互斥锁进行加锁操作");
            }
            // 尝试加锁，如果已经被锁定则抛出异常
            if (!ptr_->try_lock()) {
                throw std::runtime_error("互斥锁已被锁定，无法再次加锁");
            }
            ptr_->lock();
        }
        void unlock() {
            if (!ptr_) {
                throw std::runtime_error("尝试对空互斥锁进行解锁操作");
            }
            ptr_->unlock();
        }
        unique_lock(const unique_lock&) = delete;
        unique_lock& operator=(const unique_lock&) = delete;
        unique_lock(unique_lock&& other) noexcept : ptr_(other.ptr_) {
            other.ptr_ = nullptr;
        }
        unique_lock& operator=(unique_lock&& other) noexcept {
            ptr_ = other.ptr_;
            other.ptr_ = nullptr;
            return *this;
        }
};