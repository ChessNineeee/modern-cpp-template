//
// Created by wuzhengqi on 24-11-23.
//

#include <algorithm>
#include <iostream>
#include <vector>

// Merges two consecutive sorted ranges [first, middle) and [middle, last) into one sorted range [first, last).

// 1) If [first, middle) or [middle, last) is not sorted with respect to operator<(until C++20)std::less{}(since C++20), the behavior is undefined.
// 3) If [first, middle) or [middle, last) is not sorted with respect to comp, the behavior is undefined.
// 2,4) Same as (1,3), but executed according to policy.
//  These overloads participate in overload resolution only if
// std::is_execution_policy_v<std::decay_t<ExecutionPolicy>> is true.
//
// (until C++20)
// std::is_execution_policy_v<std::remove_cvref_t<ExecutionPolicy>> is true.
//
// (since C++20)
// This merge function is stable, which means that for equivalent elements in the original two ranges, the elements from the first range (preserving their original order) precede the elements from the second range (preserving their original order).
//
// If any of the following conditions is satisfied, the behavior is undefined:
//
// [first, middle) or [middle, last) is not a valid range.
// The output range overlaps with [first, middle) or [middle, last).
template<class Iter>
void merge_sort(Iter first, Iter last)
{
    if (last - first > 1)
    {
        Iter middle = first + (last - first) / 2;
        merge_sort(first, middle);
        merge_sort(middle, last);
        std::inplace_merge(first, middle, last);
    }
}

int main()
{
    std::vector<int> v{8, 2, -2, 0, 11, 11, 1, 7, 3};
    merge_sort(v.begin(), v.end());
    for (const auto& n : v)
        std::cout << n << ' ';
    std::cout << '\n';
}