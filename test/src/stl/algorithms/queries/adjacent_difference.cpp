//
// Created by wuzhengqi on 24-11-23.
//
#include <array>
#include <functional>
#include <iostream>
#include <iterator>
#include <numeric>
#include <vector>
// Let T be the value type of decltype(first).
//
// 1) If [first, last) is empty, does nothing.
//  Otherwise, performs the following operations in order:
// Creates an accumulator acc of type T, and initializes it with *first.
// Assigns acc to *d_first.
// For each iterator iter in [++first, last) in order, performs the following operations in order:
// a) Creates an object val of type T, and initializes it with *iter.
// b) Computes val - acc(until C++20)val - std::move(acc)(since C++20).
// c) Assigns the result to *++d_first.
// d) Copy(until C++20)Move(since C++20) assigns from val to acc.
// 2) If [first, last) is empty, does nothing.
//  Otherwise, performs the following operations in order:
// Assigns *first to *d_first.
// For each integer i in [1, std::distance(first, last)), performs the following operations in order:
// a) Computes curr - prev, where curr is the next ith iterator of first, and prev is the next i - 1th iterator of first.
// b) Assigns the result to *dest, where dest is the next ith iterator of d_first.
// 3) Same as (1), but computes op(val, acc)(until C++20)op(val, std::move(acc))(since C++20) instead.
// 4) Same as (2), but computes op(curr, prev) instead.
void println(auto comment, const auto& sequence)
{
    std::cout << comment;
    for (const auto& n : sequence)
        std::cout << n << ' ';
    std::cout << '\n';
};

int main()
{
    // Default implementation - the difference between two adjacent items
    std::vector v{4, 6, 9, 13, 18, 19, 19, 15, 10};
    println("Initially, v = ", v);
    std::adjacent_difference(v.begin(), v.end(), v.begin());
    println("Modified v = ", v);

    // Fibonacci
    std::array<int, 10> a {1};
    std::adjacent_difference(std::begin(a), std::prev(std::end(a)),
                             std::next(std::begin(a)), std::plus<>{});
    println("Fibonacci, a = ", a);
}
