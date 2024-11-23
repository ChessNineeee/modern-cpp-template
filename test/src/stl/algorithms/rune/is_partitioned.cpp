//
// Created by wuzhengqi on 24-11-23.
//
#include <algorithm>
#include <array>
#include <iostream>

// 1) Checks whether [first, last) is partitioned by the predicate p: all elements satisfy p appear before all elements that do not.
int main()
{
    std::array<int, 9> v {1, 2, 3, 4, 5, 6, 7, 8, 9};

    auto is_even = [](int i) { return i % 2 == 0; };
    std::cout.setf(std::ios_base::boolalpha);
    std::cout << std::is_partitioned(v.begin(), v.end(), is_even) << ' ';

    std::partition(v.begin(), v.end(), is_even);
    std::cout << std::is_partitioned(v.begin(), v.end(), is_even) << ' ';

    std::reverse(v.begin(), v.end());
    std::cout << std::is_partitioned(v.cbegin(), v.cend(), is_even) << ' ';
    std::cout << std::is_partitioned(v.crbegin(), v.crend(), is_even) << '\n';
}