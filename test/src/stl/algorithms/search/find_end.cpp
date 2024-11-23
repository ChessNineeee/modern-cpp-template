//
// Created by wuzhengqi on 24-11-23.
//
#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
// Searches for the last occurrence of the sequence [s_first, s_last) in the range [first, last).
//
// 1) Elements are compared using operator==.
// 3) Elements are compared using the given binary predicate p.
auto print_result = [](auto result, const auto& v)
{
    result == v.end()
        ? std::cout << "Sequence not found\n"
        : std::cout << "Last occurrence is at: " << std::distance(v.begin(), result)
                    << '\n';
};

int main()
{
    const auto v = {1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4};

    for (auto const& x : {std::array{1, 2, 3}, {4, 5, 6}})
    {
        auto iter = std::find_end(v.begin(), v.end(), x.begin(), x.end()); // overload (1)
        print_result(iter, v);
    }

    for (auto const& x : {std::array{-1, -2, -3}, {-4, -5, -6}})
    {
        auto iter = std::find_end(v.begin(), v.end(), x.begin(), x.end(), // overload (3)
                                  [](int x, int y)
                                  {
                                      return std::abs(x) == std::abs(y);
                                  });
        print_result(iter, v);
    }
}