//
// Created by wuzhengqi on 24-11-23.
//
#include <algorithm>
#include <array>
#include <iostream>
#include <iterator>

// Examines the partitioned range [first, last) and locates the end of the first partition, that is, the first element that does not satisfy p or last if all elements satisfy p.

// If the elements elem of [first, last) are not partitioned with respect to the expression bool(p(elem)), the behavior is undefined.


auto print_seq = [](auto rem, auto first, auto last)
{
    for (std::cout << rem; first != last; std::cout << *first++ << ' ') {}
    std::cout << '\n';
};

int main()
{
    std::array v{1, 2, 3, 4, 5, 6, 7, 8, 9};

    auto is_even = [](int i) { return i % 2 == 0; };

    std::partition(v.begin(), v.end(), is_even);
    print_seq("After partitioning, v: ", v.cbegin(), v.cend());

    const auto pp = std::partition_point(v.cbegin(), v.cend(), is_even);
    const auto i = std::distance(v.cbegin(), pp);
    std::cout << "Partition point is at " << i << "; v[" << i << "] = " << *pp << '\n';

    print_seq("First partition (all even elements): ", v.cbegin(), pp);
    print_seq("Second partition (all odd elements): ", pp, v.cend());
}