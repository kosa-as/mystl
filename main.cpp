#include <iostream>
#include <thread>
#include <future>
#include "function.hpp"
#include <functional>

int add(int x, int y) {
    return x + y;
}

class A {
    public:
    int add(int x, int y) {
        return x + y;
    }
};
int main() {
    A a;  // 创建 A 的实例
    Function<int(int, int)> f = add;
    Function<int(int, int)> f2 = [](int x, int y) { return x + y; };
    Function<int(int, int)> f3 = std::bind(&A::add, &a, std::placeholders::_1, std::placeholders::_2);


    std::cout << "The result of add is: " << f(1, 2) << std::endl;
    std::cout << "The result of add is: " << f2(1, 3) << std::endl;
    std::cout << "The result of add is: " << f3(1, 4) << std::endl;
    return 0;
}