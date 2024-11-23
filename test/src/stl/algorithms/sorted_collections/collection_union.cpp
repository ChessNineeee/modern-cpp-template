//
// Created by wuzhengqi on 24-11-23.
//
#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>
// Constructs a sorted union beginning at d_first consisting of the set of elements present in one or both sorted ranges [first1, last1) and [first2, last2).
//
// If [first1, last1) contains m elements that are equivalent to each other and [first2, last2) contains n elements that are equivalent to them, then all m elements will be copied from [first1, last1) to the output range, preserving order, and then the final std::max(n - m, 0) elements will be copied from [first2, last2) to the output range, also preserving order.
//
// 1) If [first1, last1) or [first2, last2) is not sorted with respect to operator<(until C++20)std::less{}(since C++20), the behavior is undefined.
// 3) If [first1, last1) or [first2, last2) is not sorted with respect to comp, the behavior is undefined.
void println(const std::vector<int>& v)
{
    for (int i : v)
        std::cout << i << ' ';
    std::cout << '\n';
}

int main()
{
    std::vector<int> v1, v2, dest;

    v1 = {1, 2, 3, 4, 5};
    v2 = {3, 4, 5, 6, 7};

    std::set_union(v1.cbegin(), v1.cend(),
                   v2.cbegin(), v2.cend(),
                   std::back_inserter(dest));
    println(dest);

    dest.clear();

    v1 = {1, 2, 3, 4, 5, 5, 5};
    v2 = {3, 4, 5, 6, 7};

    std::set_union(v1.cbegin(), v1.cend(),
                   v2.cbegin(), v2.cend(),
                   std::back_inserter(dest));
    println(dest);
}