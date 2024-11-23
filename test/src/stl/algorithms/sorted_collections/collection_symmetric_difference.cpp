//
// Created by wuzhengqi on 24-11-23.
//
// Computes symmetric difference of two sorted ranges: the elements that are found in either of the ranges, but not in both of them are copied to the range beginning at d_first. The output range is also sorted.
//
// If [first1, last1) contains m elements that are equivalent to each other and [first2, last2) contains n elements that are equivalent to them, then std::abs(m - n) of those elements will be copied to the output range, preserving order:
//
// if m > n, the final m - n of these elements from [first1, last1).
// if m < n, the final n - m of these elements from [first2, last2).
// 1) If [first1, last1) or [first2, last2) is not sorted with respect to operator<(until C++20)std::less{}(since C++20), the behavior is undefined.
// 3) If [first1, last1) or [first2, last2) is not sorted with respect to comp, the behavior is undefined.

#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

int main()
{
    std::vector<int> v1{1, 2, 3, 4, 5, 6, 7, 8};
    std::vector<int> v2{5, 7, 9, 10};
    std::sort(v1.begin(), v1.end());
    std::sort(v2.begin(), v2.end());

    std::vector<int> v_symDifference;

    std::set_symmetric_difference(v1.begin(), v1.end(), v2.begin(), v2.end(),
                                  std::back_inserter(v_symDifference));

    for (int n : v_symDifference)
        std::cout << n << ' ';
    std::cout << '\n';
}