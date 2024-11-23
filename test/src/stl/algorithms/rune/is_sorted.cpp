//
// Created by wuzhengqi on 24-11-23.
//
#include <algorithm>
#include <cassert>
#include <functional>
#include <iterator>
#include <vector>

// Checks if the elements in range [first, last) are sorted in non-descending order.
//
// 1) Checks if the elements are sorted with respect to operator<(until C++20)std::less{}(since C++20).
// 3) Checks if the elements are sorted with respect to comp.
int main()
{
    std::vector<int> v;
    assert(std::is_sorted(v.cbegin(), v.cend()) && "an empty range is always sorted");
    v.push_back(42);
    assert(std::is_sorted(v.cbegin(), v.cend()) && "a range of size 1 is always sorted");

    int data[] = {3, 1, 4, 1, 5};
    assert(not std::is_sorted(std::begin(data), std::end(data)));

    std::sort(std::begin(data), std::end(data));
    assert(std::is_sorted(std::begin(data), std::end(data)));
    assert(not std::is_sorted(std::begin(data), std::end(data), std::greater<>{}));
}