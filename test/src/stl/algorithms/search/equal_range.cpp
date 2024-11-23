//
// Created by wuzhengqi on 24-11-23.
//
#include <algorithm>
#include <complex>
#include <iostream>
#include <vector>
// Returns a range containing all elements equivalent to value in the partitioned range [first, last).
//
// 1) The equivalence is checked using operator<:
// Returns the results of std::lower_bound(first, last, value) and std::upper_bound(first, last, value).
//
// If any of the following conditions is satisfied, the behavior is undefined:
//
// For any element elem of [first, last), bool(elem < value) does not imply !bool(value < elem).
// The elements elem of [first, last) are not partitioned with respect to expressions bool(elem < value) and !bool(value < elem).
// (until C++20)
// Equivalent to std::equal_range(first, last, value, std::less{}).
//
// (since C++20)
// 2) The equivalence is checked using comp:
//  Returns the results of std::lower_bound(first, last, value, comp) and std::upper_bound(first, last, value, comp).
//  If any of the following conditions is satisfied, the behavior is undefined:
// For any element elem of [first, last), bool(comp(elem, value)) does not imply !bool(comp(value, elem)).
// The elements elem of [first, last) are not partitioned with respect to expressions bool(comp(elem, value)) and !bool(comp(value, elem)).
struct S
{
    int number;
    char name;
    // note: name is ignored by this comparison operator
    bool operator<(const S& s) const { return number < s.number; }
};

struct Comp
{
    bool operator()(const S& s, int i) const { return s.number < i; }
    bool operator()(int i, const S& s) const { return i < s.number; }
};

int main()
{
    // note: not ordered, only partitioned w.r.t. S defined below
    const std::vector<S> vec{{1, 'A'}, {2, 'B'}, {2, 'C'},
                             {2, 'D'}, {4, 'G'}, {3, 'F'}};
    const S value{2, '?'};

    std::cout << "Compare using S::operator<(): ";
    const auto p = std::equal_range(vec.begin(), vec.end(), value);

    for (auto it = p.first; it != p.second; ++it)
        std::cout << it->name << ' ';
    std::cout << '\n';

    std::cout << "Using heterogeneous comparison: ";
    const auto p2 = std::equal_range(vec.begin(), vec.end(), 2, Comp{});

    for (auto it = p2.first; it != p2.second; ++it)
        std::cout << it->name << ' ';
    std::cout << '\n';

    using CD = std::complex<double>;
    std::vector<CD> nums{{1, 0}, {2, 2}, {2, 1}, {3, 0}, {3, 1}};
    auto cmpz = [](CD x, CD y) { return x.real() < y.real(); };
    #ifdef __cpp_lib_algorithm_default_value_type
        auto p3 = std::equal_range(nums.cbegin(), nums.cend(), {2, 0}, cmpz);
    #else
        auto p3 = std::equal_range(nums.cbegin(), nums.cend(), CD{2, 0}, cmpz);
    #endif

    for (auto it = p3.first; it != p3.second; ++it)
        std::cout << *it << ' ';
    std::cout << '\n';
}