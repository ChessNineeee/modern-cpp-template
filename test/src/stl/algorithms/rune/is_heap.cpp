//
// Created by wuzhengqi on 24-11-23.
//
#include <algorithm>
#include <bit>
#include <iostream>
#include <vector>
// Checks whether [first, last) is a heap.
//
// 1) The heap property to be checked is with respect to operator<(until C++20)std::less{}(since C++20).
// 3) The heap property to be checked is with respect to comp.

int main()
{
    std::vector<int> v{3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5, 8, 9, 7, 9};

    std::cout << "initially, v:\n";
    for (const auto& i : v)
        std::cout << i << ' ';
    std::cout << '\n';

    if (!std::is_heap(v.begin(), v.end()))
    {
        std::cout << "making heap...\n";
        std::make_heap(v.begin(), v.end());
    }

    std::cout << "after make_heap, v:\n";
    for (auto t{1U}; const auto& i : v)
        std::cout << i << (std::has_single_bit(++t) ? " | " : " ");
    std::cout << '\n';
}