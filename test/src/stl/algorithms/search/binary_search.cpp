//
// Created by wuzhengqi on 24-11-23.
//
#include <algorithm>
#include <cassert>
#include <complex>
#include <iostream>
#include <vector>
// Checks if an element equivalent to value appears within the partitioned range [first, last).
//
// 1) The equivalence is checked using operator<:
// If !bool(*iter < value) && !bool(value < *iter) is true for some iterator iter in [first, last), returns true. Otherwise returns false.
//
// If any of the following conditions is satisfied, the behavior is undefined:
//
// For any element elem of [first, last), bool(elem < value) does not imply !bool(value < elem).
// The elements elem of [first, last) are not partitioned with respect to expressions bool(elem < value) and !bool(value < elem).
// (until C++20)
// Equivalent to std::binary_search(first, last, value, std::less{}).
//
// (since C++20)
// 2) The equivalence is checked using comp:
//  If !bool(comp(*iter, value)) && !bool(comp(value, *iter)) is true for some iterator iter in [first, last), returns true. Otherwise returns false.
//  If any of the following conditions is satisfied, the behavior is undefined:
// For any element elem of [first, last), bool(comp(elem, value)) does not imply !bool(comp(value, elem)).
// The elements elem of [first, last) are not partitioned with respect to expressions bool(comp(elem, value)) and !bool(comp(value, elem)).
int main()
{
    const auto haystack = {1, 3, 4, 5, 9};

    for (const auto needle : {1, 2, 3})
    {
        std::cout << "Searching for " << needle << '\n';
        if (std::binary_search(haystack.begin(), haystack.end(), needle))
            std::cout << "Found " << needle << '\n';
        else
            std::cout << "No dice!\n";
    }

    using CD = std::complex<double>;
    std::vector<CD> nums{{1, 1}, {2, 3}, {4, 2}, {4, 3}};
    auto cmpz = [](CD x, CD y){ return abs(x) < abs(y); };
    #ifdef __cpp_lib_algorithm_default_value_type
        assert(std::binary_search(nums.cbegin(), nums.cend(), {4, 2}, cmpz));
    #else
        assert(std::binary_search(nums.cbegin(), nums.cend(), CD{4, 2}, cmpz));
    #endif
}