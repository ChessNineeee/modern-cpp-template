//
// Created by wuzhengqi on 24-11-23.
//
#include <algorithm>
#include <iostream>
// Checks whether [first1, last1) is a permutation of a range starting from first2:
//
// For overloads (1,2), the second range has std::distance(first1, last1) elements.
// For overloads (3,4), the second range is [first2, last2).
// 1,3) Elements are compared using operator==.
// 2,4) Elements are compared using the given binary predicate p.
template<typename Os, typename V>
Os& operator<<(Os& os, const V& v)
{
    os << "{ ";
    for (const auto& e : v)
        os << e << ' ';
    return os << '}';
}

int main()
{
    static constexpr auto v1 = {1, 2, 3, 4, 5};
    static constexpr auto v2 = {3, 5, 4, 1, 2};
    static constexpr auto v3 = {3, 5, 4, 1, 1};

    std::cout << v2 << " is a permutation of " << v1 << ": " << std::boolalpha
              << std::is_permutation(v1.begin(), v1.end(), v2.begin()) << '\n'
              << v3 << " is a permutation of " << v1 << ": "
              << std::is_permutation(v1.begin(), v1.end(), v3.begin()) << '\n';
}