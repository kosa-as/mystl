#pragma once
#include <memory>
#include <utility>
#include <type_traits>

template <typename>
class Function; 

//模板特化，R(Args...)表示一个可调用对象，R是返回类型，Args...是参数类型
template <typename R, typename... Args>
class Function<R(Args...)> {
    //ICallable是一个抽象类，定义了一个纯虚函数invoke，用于包裹可调用对象
    class ICallable {
    public:
        virtual R invoke(Args&&... args) = 0;
        virtual std::unique_ptr<ICallable> clone() const = 0;
        virtual ~ICallable() = default;
    };

    //CallableImpl是一个具体类，继承自ICallable，用于包裹可调用对象
    template <typename F>
    class CallableImpl : public ICallable {
        F func;
    public:
        explicit CallableImpl(F&& f) : func(std::forward<F>(f)) {}
        
        R invoke(Args&&... args) override {
            return func(std::forward<Args>(args)...);
        }
        
        std::unique_ptr<ICallable> clone() const override {
            return std::make_unique<CallableImpl<F>>(F(func));
        }
    };

    std::unique_ptr<ICallable> callable;

public:
    // 默认构造函数
    Function() noexcept = default;
    
    template <typename F, typename = std::enable_if_t<!std::is_same<std::decay_t<F>, Function>::value>>
    //构造函数，接受一个可调用对象F，并将其包裹在CallableImpl中
    Function(F&& f)
        : callable(std::make_unique<CallableImpl<std::decay_t<F>>>(std::forward<F>(f))) {}

    // 移动构造和赋值
    Function(Function&& other) noexcept = default;
    Function& operator=(Function&& other) noexcept = default;

    // 拷贝构造函数
    Function(const Function& other) 
        : callable(other.callable ? other.callable->clone() : nullptr) {}
    
    // 拷贝赋值运算符
    Function& operator=(const Function& other) {
        if (this != &other) {
            callable = other.callable ? other.callable->clone() : nullptr;
        }
        return *this;
    }

    R operator()(Args... args) {
        if (!callable) {
            throw std::bad_function_call();
        }
        return callable->invoke(std::forward<Args>(args)...);
    }

    explicit operator bool() const {
        return static_cast<bool>(callable);
    }
};