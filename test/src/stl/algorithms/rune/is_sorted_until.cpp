//
// Created by wuzhengqi on 24-11-23.
//
#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>
#include <random>
#include <string>
// Examines the range [first, last) and finds the largest range beginning at first in which the elements are sorted in non-descending order.
//
// 1) Finds the largest range whether elements are sorted with respect to operator<(until C++20)std::less{}(since C++20).
// 3) Finds the largest range whether elements are sorted with respect to comp.
int main()
{
    std::random_device rd;
    std::mt19937 g(rd());
    const int N = 6;
    int nums[N] = {3, 1, 4, 1, 5, 9};

    const int min_sorted_size = 4;

    for (int sorted_size = 0; sorted_size < min_sorted_size;)
    {
        std::shuffle(nums, nums + N, g);
        int *const sorted_end = std::is_sorted_until(nums, nums + N);
        sorted_size = std::distance(nums, sorted_end);
        assert(sorted_size >= 1);

        for (const auto i : nums)
            std::cout << i << ' ';
        std::cout << ": " << sorted_size << " initial sorted elements\n"
                  << std::string(sorted_size * 2 - 1, '^') << '\n';
    }
}