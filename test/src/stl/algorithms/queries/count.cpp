//
// Created by wuzhengqi on 24-11-23.
//
#include <algorithm>
#include <array>
#include <cassert>
#include <complex>
#include <iostream>
#include <iterator>
// Returns the number of elements in the range [first, last) satisfying specific criteria.
//
// 1) Counts the elements that are equal to value (using operator==).
// 3) Counts elements for which predicate p returns true.
int main()
{
    constexpr std::array v{1, 2, 3, 4, 4, 3, 7, 8, 9, 10};
    std::cout << "v: ";
    std::copy(v.cbegin(), v.cend(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << '\n';

    // Determine how many integers match a target value.
    for (int const target : {3, 4, 5})
    {
        auto const num_items = std::count(v.cbegin(), v.cend(), target);
        std::cout << "number: " << target << ", count: " << num_items << '\n';
    }

    // Use a lambda expression to count elements divisible by 4.
    auto const count_div4 = std::count_if(v.begin(), v.end(), [](int i) { return i % 4 == 0; });
    std::cout << "numbers divisible by four: " << count_div4 << '\n';

    // A simplified version of `distance` with O(N) complexity:
    auto distance = [](auto first, auto last)
    {
        return std::count_if(first, last, [](auto) { return true; });
    };
    static_assert(distance(v.begin(), v.end()) == 10);

    std::array<std::complex<double>, 3> nums{{{4, 2}, {1, 3}, {4, 2}}};
    #ifdef __cpp_lib_algorithm_default_value_type
        // T gets deduced making list-initialization possible
        auto c = std::count(nums.cbegin(), nums.cend(), {4, 2});
    #else
        auto c = std::count(nums.cbegin(), nums.cend(), std::complex<double>{4, 2});
    #endif
    assert(c == 2);
}