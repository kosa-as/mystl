#include <iostream>
#include "vector.h"
#include "shared_ptr.h"
int main() {
    shared_ptr<int> p1 = shared_ptr<int>::make_shared(10);
    {
        shared_ptr<int> p2 = p1;
        shared_ptr<int> p3 = p1;
        std::cout << "p1: " << *p1 << std::endl;
        std::cout << "p1_use_count: " << p1.use_count() << std::endl;
        std::cout << "p2: " << *p2 << std::endl;
        std::cout << "p2_use_count: " << p2.use_count() << std::endl;
    }
    shared_ptr<int> p2 = p1;
    std::cout << "p1: " << *p1 << std::endl;
    std::cout << "p1_use_count: " << p1.use_count() << std::endl;
    std::cout << "p2: " << *p2 << std::endl;
    std::cout << "p2_use_count: " << p2.use_count() << std::endl;
    int* ptr = new int(1);
}
