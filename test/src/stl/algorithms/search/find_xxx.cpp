//
// Created by wuzhengqi on 24-11-23.
//
#include <algorithm>
#include <array>
#include <cassert>
#include <complex>
#include <iostream>
#include <vector>
// Returns an iterator to the first element in the range [first, last) that satisfies specific criteria (or last if there is no such iterator).
//
// 1) find searches for an element equal to value (using operator==).
// 3) find_if searches for an element for which predicate p returns true.
// 5) find_if_not searches for an element for which predicate q returns false.
int main()
{
    const auto v = {1, 2, 3, 4};

    for (const int n : {3, 5})
        (std::find(v.begin(), v.end(), n) == std::end(v))
            ? std::cout << "v does not contain " << n << '\n'
            : std::cout << "v contains " << n << '\n';

    auto is_even = [](int i) { return i % 2 == 0; };

    for (const auto& w : {std::array{3, 1, 4}, {1, 3, 5}})
        if (auto it = std::find_if(begin(w), end(w), is_even); it != std::end(w))
            std::cout << "w contains an even number " << *it << '\n';
        else
            std::cout << "w does not contain even numbers\n";

    std::vector<std::complex<double>> nums{{4, 2}};
    #ifdef __cpp_lib_algorithm_default_value_type
        // T gets deduced making list-initialization possible
        const auto it = std::find(nums.begin(), nums.end(), {4, 2});
    #else
        const auto it = std::find(nums.begin(), nums.end(), std::complex<double>{4, 2});
    #endif
    assert(it == nums.begin());
}