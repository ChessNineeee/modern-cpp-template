//
// Created by wuzhengqi on 24-11-23.
//

#include <algorithm>
#include <iostream>
#include <vector>

int main()
{
    std::vector<int> v{3, 1, -4, 1, 5, 9};

    std::vector<int>::iterator result = std::min_element(v.begin(), v.end());
    std::cout << "min element has value " << *result << " and index ["
              << std::distance(v.begin(), result) << "]\n";
}