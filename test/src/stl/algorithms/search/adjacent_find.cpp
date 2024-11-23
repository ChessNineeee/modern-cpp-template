//
// Created by wuzhengqi on 24-11-23.
//

#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>
// Searches the range [first, last) for two consecutive equal elements.
//
// 1) Elements are compared using operator==.
// 3) Elements are compared using the given binary predicate p.
int main()
{
    std::vector<int> v1{0, 1, 2, 3, 40, 40, 41, 41, 5};

    auto i1 = std::adjacent_find(v1.begin(), v1.end());

    if (i1 == v1.end())
        std::cout << "No matching adjacent elements\n";
    else
        std::cout << "The first adjacent pair of equal elements is at "
                  << std::distance(v1.begin(), i1) << ", *i1 = "
                  << *i1 << '\n';

    auto i2 = std::adjacent_find(v1.begin(), v1.end(), std::greater<int>());
    if (i2 == v1.end())
        std::cout << "The entire vector is sorted in ascending order\n";
    else
        std::cout << "The last element in the non-decreasing subsequence is at "
                  << std::distance(v1.begin(), i2) << ", *i2 = " << *i2 << '\n';
}