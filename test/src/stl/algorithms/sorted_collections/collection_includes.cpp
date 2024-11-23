//
// Created by wuzhengqi on 24-11-23.
//
#include <algorithm>
#include <cctype>
#include <iostream>

// Returns true if the sorted range [first2, last2) is a subsequence of the sorted range [first1, last1) (a subsequence need not be contiguous).
//
// 1) If [first1, last1) or [first2, last2) is not sorted with respect to operator<(until C++20)std::less{}(since C++20), the behavior is undefined.
// 3) If [first1, last1) or [first2, last2) is not sorted with respect to comp, the behavior is undefined.
template<class Os, class Co>
Os& operator<<(Os& os, const Co& v)
{
    for (const auto& i : v)
        os << i << ' ';
    return os << '\t';
}

int main()
{
    const auto
        v1 = {'a', 'b', 'c', 'f', 'h', 'x'},
        v2 = {'a', 'b', 'c'},
        v3 = {'a', 'c'},
        v4 = {'a', 'a', 'b'},
        v5 = {'g'},
        v6 = {'a', 'c', 'g'},
        v7 = {'A', 'B', 'C'};

    auto no_case = [](char a, char b) { return std::tolower(a) < std::tolower(b); };

    std::cout
    << v1 << "\nincludes:\n" << std::boolalpha
    << v2 << ": " << std::includes(v1.begin(), v1.end(), v2.begin(), v2.end()) << '\n'
    << v3 << ": " << std::includes(v1.begin(), v1.end(), v3.begin(), v3.end()) << '\n'
    << v4 << ": " << std::includes(v1.begin(), v1.end(), v4.begin(), v4.end()) << '\n'
    << v5 << ": " << std::includes(v1.begin(), v1.end(), v5.begin(), v5.end()) << '\n'
    << v6 << ": " << std::includes(v1.begin(), v1.end(), v6.begin(), v6.end()) << '\n'
    << v7 << ": " << std::includes(v1.begin(), v1.end(), v7.begin(), v7.end(), no_case)
          << " (case-insensitive)\n";
}