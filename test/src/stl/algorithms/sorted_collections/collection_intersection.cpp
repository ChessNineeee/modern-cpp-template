//
// Created by wuzhengqi on 24-11-23.
//
#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>
// Constructs a sorted range beginning at d_first consisting of elements that are found in both sorted ranges [first1, last1) and [first2, last2).
//
// If [first1, last1) contains m elements that are equivalent to each other and [first2, last2) contains n elements that are equivalent to them, the first std::min(m, n) elements will be copied from [first1, last1) to the output range, preserving order.
//
// 1) If [first1, last1) or [first2, last2) is not sorted with respect to operator<(until C++20)std::less{}(since C++20), the behavior is undefined.
// 3) If [first1, last1) or [first2, last2) is not sorted with respect to comp, the behavior is undefined.

int main()
{
    std::vector<int> v1{7, 2, 3, 4, 5, 6, 7, 8};
    std::vector<int> v2{5, 7, 9, 7};
    std::sort(v1.begin(), v1.end());
    std::sort(v2.begin(), v2.end());

    std::vector<int> v_intersection;
    std::set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(),
                          std::back_inserter(v_intersection));

    for (int n : v_intersection)
        std::cout << n << ' ';
    std::cout << '\n';
}