//
// Created by wuzhengqi on 24-11-23.
//
#include <functional>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>
// Computes the sum of the given value init and the elements in the range [first, last).
//
// 1) Initializes the accumulator acc (of type T) with the initial value init and then modifies it with acc = acc + *i(until C++20)acc = std::move(acc) + *i(since C++20) for every iterator i in the range [first, last) in order.
// 2) Initializes the accumulator acc (of type T) with the initial value init and then modifies it with acc = op(acc, *i)(until C++20)acc = op(std::move(acc), *i)(since C++20) for every iterator i in the range [first, last) in order.
int main()
{
    std::vector<int> v{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    int sum = std::accumulate(v.begin(), v.end(), 0);
    int product = std::accumulate(v.begin(), v.end(), 1, std::multiplies<int>());

    auto dash_fold = [](std::string a, int b)
    {
        return std::move(a) + '-' + std::to_string(b);
    };

    std::string s = std::accumulate(std::next(v.begin()), v.end(),
                                    std::to_string(v[0]), // start with first element
                                    dash_fold);

    // Right fold using reverse iterators
    std::string rs = std::accumulate(std::next(v.rbegin()), v.rend(),
                                     std::to_string(v.back()), // start with last element
                                     dash_fold);

    std::cout << "sum: " << sum << '\n'
              << "product: " << product << '\n'
              << "dash-separated string: " << s << '\n'
              << "dash-separated string (right-folded): " << rs << '\n';
}