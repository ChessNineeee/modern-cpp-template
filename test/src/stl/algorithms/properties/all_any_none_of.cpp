//
// Created by wuzhengqi on 24-11-23.
//
#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <numeric>
#include <vector>
// 1) Checks if unary predicate p returns true for all elements in the range [first, last).
// 3) Checks if unary predicate p returns true for at least one element in the range [first, last).
// 5) Checks if unary predicate p returns true for no elements in the range [first, last).
int main()
{
    std::vector<int> v(10, 2);
    std::partial_sum(v.cbegin(), v.cend(), v.begin());
    std::cout << "Among the numbers: ";
    std::copy(v.cbegin(), v.cend(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << '\n';

    if (std::all_of(v.cbegin(), v.cend(), [](int i) { return i % 2 == 0; }))
        std::cout << "All numbers are even\n";

    if (std::none_of(v.cbegin(), v.cend(), std::bind(std::modulus<>(),
                                                     std::placeholders::_1, 2)))
        std::cout << "None of them are odd\n";

    struct DivisibleBy
    {
        const int d;
        DivisibleBy(int n) : d(n) {}
        bool operator()(int n) const { return n % d == 0; }
    };

    if (std::any_of(v.cbegin(), v.cend(), DivisibleBy(7)))
        std::cout << "At least one number is divisible by 7\n";

    if (std::none_of(v.cbegin(), v.cend(), [](const auto &x) { return x % 100 == 0; }))
        std::cout << "None of them is divisible by 100\n";
}