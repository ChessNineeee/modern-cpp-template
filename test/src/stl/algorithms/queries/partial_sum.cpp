//
// Created by wuzhengqi on 24-11-23.
//
#include <functional>
#include <iostream>
#include <iterator>
#include <numeric>
#include <vector>
// 1) If [first, last) is empty, does nothing.
//  Otherwise, performs the following operations in order:
// Creates an accumulator acc, whose type is the value type of InputIt, and initializes it with *first.
// Assigns acc to *d_first.
// For each integer i in [1, std::distance(first, last)), performs the following operations in order:
// a) Computes acc + *iter(until C++20)std::move(acc) + *iter(since C++20), where iter is the next ith iterator of first.
// b) Assigns the result to acc.
// c) Assigns acc[1] to *dest, where dest is the next ith iterator of d_first.
// 2) Same as (1), but computes op(acc, *iter)(until C++20)op(std::move(acc), *iter)(since C++20) instead.
int main()
{
    std::vector<int> v(10, 2); // v = {2, 2, 2, 2, 2, 2, 2, 2, 2, 2}

    std::cout << "The first " << v.size() << " even numbers are: ";
    // write the result to the cout stream
    std::partial_sum(v.cbegin(), v.cend(),
                     std::ostream_iterator<int>(std::cout, " "));
    std::cout << '\n';

    // write the result back to the vector v
    std::partial_sum(v.cbegin(), v.cend(),
                     v.begin(), std::multiplies<int>());

    std::cout << "The first " << v.size() << " powers of 2 are: ";
    for (int n : v)
        std::cout << n << ' ';
    std::cout << '\n';
}