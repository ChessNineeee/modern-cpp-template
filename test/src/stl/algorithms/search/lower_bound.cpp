//
// Created by wuzhengqi on 24-11-23.
//
#include <algorithm>
#include <cassert>
#include <complex>
#include <iostream>
#include <vector>
// Searches for the first element in the partitioned range [first, last) which is not ordered before value.
//
// 1) The order is determined by operator<:
// Returns the first iterator iter in [first, last) where bool(*iter < value) is false, or last if no such iter exists.
//
// If the elements elem of [first, last) are not partitioned with respect to the expression bool(elem < value), the behavior is undefined.
//
// (until C++20)
// Equivalent to std::lower_bound(first, last, value, std::less{}).
//
// (since C++20)
// 2) The order is determined by comp:
//  Returns the first iterator iter in [first, last) where bool(comp(*iter, value)) is false, or last if no such iter exists.
//  If the elements elem of [first, last) are not partitioned with respect to the expression bool(comp(elem, value)), the behavior is undefined.
struct PriceInfo { double price; };

int main()
{
    const std::vector<int> data{1, 2, 4, 5, 5, 6};

    for (int i = 0; i < 8; ++i)
    {
        // Search for first element x such that i ≤ x
        auto lower = std::lower_bound(data.begin(), data.end(), i);

        std::cout << i << " ≤ ";
        lower != data.end()
            ? std::cout << *lower << " at index " << std::distance(data.begin(), lower)
            : std::cout << "not found";
        std::cout << '\n';
    }

    std::vector<PriceInfo> prices{{100.0}, {101.5}, {102.5}, {102.5}, {107.3}};

    for (const double to_find : {102.5, 110.2})
    {
        auto prc_info = std::lower_bound(prices.begin(), prices.end(), to_find,
            [](const PriceInfo& info, double value)
            {
                return info.price < value;
            });

        prc_info != prices.end()
            ? std::cout << prc_info->price << " at index " << prc_info - prices.begin()
            : std::cout << to_find << " not found";
        std::cout << '\n';
    }

    using CD = std::complex<double>;
    std::vector<CD> nums{{1, 0}, {2, 2}, {2, 1}, {3, 0}};
    auto cmpz = [](CD x, CD y) { return x.real() < y.real(); };
    #ifdef __cpp_lib_algorithm_default_value_type
        auto it = std::lower_bound(nums.cbegin(), nums.cend(), {2, 0}, cmpz);
    #else
        auto it = std::lower_bound(nums.cbegin(), nums.cend(), CD{2, 0}, cmpz);
    #endif
    assert((*it == CD{2, 2}));
}