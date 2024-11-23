//
// Created by wuzhengqi on 24-11-23.
//
#include <functional>
#include <iostream>
#include <numeric>
#include <vector>
// Computes inner product (i.e. sum of products) or performs ordered map/reduce operation on the range [first1, last1) and the range of std::distance(first1, last1) elements beginning at first2.
//
// 1) Initializes the accumulator acc (of type T) with the initial value init and then modifies it with the expression acc = acc + (*i1) * (*i2)(until C++20)acc = std::move(acc) + (*i1) * (*i2)(since C++20) for every iterator i1 in the range [first1, last1) in order and its corresponding iterator i2 in the range beginning at first2. For built-in meaning of + and *, this computes inner product of the two ranges.
// 2) Initializes the accumulator acc (of type T) with the initial value init and then modifies it with the expression acc = op1(acc, op2(*i1, *i2))(until C++20)acc = op1(std::move(acc), op2(*i1, *i2))(since C++20) for every iterator i1 in the range [first1, last1) in order and its corresponding iterator i2 in the range beginning at first2.
int main()
{
    std::vector<int> a{0, 1, 2, 3, 4};
    std::vector<int> b{5, 4, 2, 3, 1};

    int r1 = std::inner_product(a.begin(), a.end(), b.begin(), 0);
    std::cout << "Inner product of a and b: " << r1 << '\n';

    int r2 = std::inner_product(a.begin(), a.end(), b.begin(), 0,
                                std::plus<>(), std::equal_to<>());
    std::cout << "Number of pairwise matches between a and b: " <<  r2 << '\n';
}