//
// Created by wuzhengqi on 24-11-23.
//

#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

int main()
{
    std::vector<int> v{3, 1, -14, 1, 5, 9, -14, 9};

    auto result = std::max_element(v.begin(), v.end());
    std::cout << "Max element found at index "
              << std::distance(v.begin(), result)
              << " has value " << *result << '\n';

    result = std::max_element(v.begin(), v.end(), [](int a, int b)
    {
        return std::abs(a) < std::abs(b);
    });
    std::cout << "Absolute max element found at index "
              << std::distance(v.begin(), result)
              << " has value " << *result << '\n';
}