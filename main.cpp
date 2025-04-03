#include <iostream>
#include "vector.h"
#include <vector>
#include <string>

class A {
public:
    std::string str;
    
    A() : str("Hello World") {
        std::cout << "A默认构造函数" << std::endl;
        std::cout << "  A对象地址: " << this << std::endl;
        std::cout << "  str成员地址: " << &str << std::endl;
        std::cout << "  str内部缓冲区地址: " << static_cast<const void*>(str.data()) << std::endl;
    }
    
    A(const A& other) : str(other.str) {
        std::cout << "A拷贝构造函数" << std::endl;
        std::cout << "  A对象地址: " << this << std::endl;
        std::cout << "  str成员地址: " << &str << std::endl;
        std::cout << "  str内部缓冲区地址: " << static_cast<const void*>(str.data()) << std::endl;
    }
    
    A(A&& other) noexcept : str(std::move(other.str)) {
        std::cout << "A移动构造函数" << std::endl;
        std::cout << "  A对象地址: " << this << std::endl;
        std::cout << "  str成员地址: " << &str << std::endl;
        std::cout << "  str内部缓冲区地址: " << static_cast<const void*>(str.data()) << std::endl;
        std::cout << "  other.str内部缓冲区地址: " << static_cast<const void*>(other.str.data()) << std::endl;
    }
    
    A& operator=(const A& other) {
        str = other.str;
        std::cout << "A拷贝赋值运算符" << std::endl;
        return *this;
    }
    
    A& operator=(A&& other) noexcept {
        str = std::move(other.str);
        std::cout << "A移动赋值运算符" << std::endl;
        return *this;
    }
    
    ~A() {
        std::cout << "A析构函数" << std::endl;
    }
};

int main() {
    Vector<A> vec;
    std::vector<A> vec2;
    // vec.set_capacity(10);
    A* a1 = new A();
    A* a2 = new A();
    vec.push_back(*a1);
    for(const auto& i : vec) {
        std::cout << &i << " ";
    }
    std::cout<<std::endl;
    vec.push_back(std::move(*a1));
    for(const auto& i : vec) {
        std::cout << &i << " ";
    }
    std::cout<<std::endl;
    vec.emplace_back(*a2);
    for(const auto& i : vec) {
        std::cout << &i << " ";
    }
    std::cout<<std::endl;
    vec.emplace_back(std::move(*a2));
    for(const auto& i : vec) {
        std::cout << &i << " ";
    }
    std::cout<<std::endl;
    delete a1;
    delete a2;

    A a3;
    A a4;
    vec2.push_back(a3);
    for(const auto& i : vec2) {
        std::cout << &i << " ";
    }
    std::cout<<std::endl;
    vec2.push_back(std::move(a3));
    for(const auto& i : vec2) {
        std::cout << &i << " ";
    }
    std::cout<<std::endl;
    vec2.emplace_back(a4);
    for(const auto& i : vec2) {
        std::cout << &i << " ";
    }
    std::cout<<std::endl;
    vec2.emplace_back(std::move(a4));
    for(const auto& i : vec2) {
        std::cout << &i << " ";
    }
    std::cout<<std::endl;

    std::cout << "创建 a1:" << std::endl;
    A a5;
    
    std::cout << "\n创建 a2 = std::move(a1):" << std::endl;
    A a6 = std::move(a5);
    
    std::cout << "\n移动后检查 a1:" << std::endl;
    std::cout << "  a1对象地址: " << &a5 << std::endl;
    std::cout << "  a1.str成员地址: " << &a5.str << std::endl;
    std::cout << "  a1.str内部缓冲区地址: " << static_cast<const void*>(a5.str.data()) << std::endl;
    std::cout << "  a1.str内容: \"" << a5.str << "\"" << std::endl;
    
    return 0;
}
