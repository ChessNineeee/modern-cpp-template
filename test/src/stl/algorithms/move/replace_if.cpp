//
// Created by wuzhengqi on 24-11-23.
//
#include <algorithm>
#include <array>
#include <complex>
#include <functional>
#include <iostream>
// Replaces all elements in the range [first, last) with new_value if they satisfy specific criteria.
//
// 1) Replaces all elements that are equal to old_value (using operator==).
// 3) Replaces all elements for which predicate p returns true.
void println(const auto& seq)
{
    for (const auto& e : seq)
        std::cout << e << ' ';
    std::cout << '\n';
}

int main()
{
    std::array<int, 10> s{5, 7, 4, 2, 8, 6, 1, 9, 0, 3};

    // Replace all occurrences of 8 with 88.
    std::replace(s.begin(), s.end(), 8, 88);
    println(s);

    // Replace all values less than 5 with 55.
    std::replace_if(s.begin(), s.end(),
                    std::bind(std::less<int>(), std::placeholders::_1, 5), 55);
    println(s);

    std::array<std::complex<double>, 2> nums{{{1, 3}, {1, 3}}};
    #ifdef __cpp_lib_algorithm_default_value_type
        std::replace(nums.begin(), nums.end(), {1, 3}, {4, 2});
    #else
        std::replace(nums.begin(), nums.end(), std::complex<double>{1, 3},
                                               std::complex<double>{4, 2});
    #endif
    println(nums);
}