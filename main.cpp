#include <iostream>
#include "vector.h"
#include <vector>
int main() {
    Vector<int> vec3 = {22,2,63,34,66};
    Vector<int> vec4 = vec3;
    Vector<int> vec5(vec3);
    Vector<int> vec6(vec3.begin(), vec3.end());

    std::cout << vec3 << std::endl;
    vec3.sort(vec3.begin(), vec3.end());
    std::cout << vec3 << std::endl;

    std::cout << vec4 << std::endl;
    vec4.sort();
    std::cout << vec4 << std::endl;

    std::cout << vec5 << std::endl;
    vec5.sort(vec5.begin(), vec5.end(), [](int a, int b) { return a > b; });
    std::cout << vec5 << std::endl;

    std::cout << vec6 << std::endl;
    vec6.sort([](int a, int b) { return a > b; });
    std::cout << vec6 << std::endl;



    std::vector<int> v5 = {1,2,3,4,5};
    std::vector<int> v6 = std::move(v5);
    for (auto it = v6.begin(); it != v6.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    v6.push_back(6);
    for (auto it = v6.begin(); it != v6.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    return 0;
}