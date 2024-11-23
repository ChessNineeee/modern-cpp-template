//
// Created by wuzhengqi on 24-11-23.
//
#include <functional>
#include <iostream>
#include <iterator>
#include <numeric>
#include <vector>

// 1) Equivalent to inclusive_scan(first, last, d_first, std::plus<>().
// 3) Computes the inclusive prefix sum using op.
//  For each integer i in [​0​, std::distance(first, last)), performs the following operations in order:
// Creates a sequence which is formed by the elements of [first, iter] in order, where iter is the next ith iterator of first.
// Computes the generalized noncommutative sum of the sequence over op.
// Assigns the result to *dest, where dest is the next ith iterator of d_first.
// 5) Same as (3), but each sequence created is formed by init followed by the elements of [first, iter] in order.
int main()
{
    std::vector data{3, 1, 4, 1, 5, 9, 2, 6};

    std::cout << "Exclusive sum: ";
    std::exclusive_scan(data.begin(), data.end(),
                        std::ostream_iterator<int>(std::cout, " "),
                        0);

    std::cout << "\nInclusive sum: ";
    std::inclusive_scan(data.begin(), data.end(),
                        std::ostream_iterator<int>(std::cout, " "));

    std::cout << "\n\nExclusive product: ";
    std::exclusive_scan(data.begin(), data.end(),
                        std::ostream_iterator<int>(std::cout, " "),
                        1, std::multiplies<>{});

    std::cout << "\nInclusive product: ";
    std::inclusive_scan(data.begin(), data.end(),
                        std::ostream_iterator<int>(std::cout, " "),
                        std::multiplies<>{});
}