//
// Created by wuzhengqi on 24-11-23.
//
#include <algorithm>
#include <iostream>
#include <vector>
// Examines the range [first, last) and finds the largest range beginning at first which is a heap.
//
// 1) The heap property to be checked is with respect to operator<(until C++20)std::less{}(since C++20).
// 3) The heap property to be checked is with respect to comp.
int main()
{
    std::vector<int> v{3, 1, 4, 1, 5, 9};

    std::make_heap(v.begin(), v.end());

    // probably mess up the heap
    v.push_back(2);
    v.push_back(6);

    auto heap_end = std::is_heap_until(v.begin(), v.end());

    std::cout << "all of v:  ";
    for (const auto& i : v)
        std::cout << i << ' ';
    std::cout << '\n';

    std::cout << "only heap: ";
    for (auto i = v.begin(); i != heap_end; ++i)
        std::cout << *i << ' ';
    std::cout << '\n';
}