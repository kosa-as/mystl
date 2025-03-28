#include <iostream>
#include "vector.h"
#include <vector>
int main() {
    Vector<int> vec3 = {22,2,63,34,66};
    std::cout << vec3 << std::endl;
    vec3.sort();
    std::cout << vec3 << std::endl;
    vec3.sort([](int a, int b) { return a > b; });
    std::cout << vec3 << std::endl;
    Vector<int> vec4 = std::move(vec3);
    std::cout << vec4 << std::endl;
    vec3.push_back(1);
    std::cout << vec3 << std::endl;
    std::vector<int> vec5 = {1,2,3,4,5};
    std::vector<int> vec6 = std::move(vec5);
    for (auto it = vec6.begin(); it != vec6.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    vec6.push_back(6);
    for (auto it = vec6.begin(); it != vec6.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    return 0;
}