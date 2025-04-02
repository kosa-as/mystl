#include <iostream>
#include "vector.h"
#include <vector>
int main() {
    Vector<int> vec3 = {22,2,63,34,66};
    int a = 10;
    vec3.emplace_back(a);
    vec3.emplace_back(1);
    vec3.emplace_back(12);
    std::cout << vec3 << std::endl;

    return 0;
}