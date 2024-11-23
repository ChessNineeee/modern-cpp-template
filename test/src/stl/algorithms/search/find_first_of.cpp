//
// Created by wuzhengqi on 24-11-23.
//
#include <algorithm>
#include <iostream>
#include <vector>
// Searches the range [first, last) for any of the elements in the range [s_first, s_last).
//
// 1) Elements are compared using operator==.
// 3) Elements are compared using the given binary predicate p.
auto print_sequence = [](const auto id, const auto& seq, int pos = -1)
{
    std::cout << id << "{ ";
    for (int i{}; auto const& e : seq)
    {
        const bool mark{i == pos};
        std::cout << (i++ ? ", " : "");
        std::cout << (mark ? "[ " : "") << e << (mark ? " ]" : "");
    }
    std::cout << " }\n";
};

int main()
{
    const std::vector<int> v{0, 2, 3, 25, 5};
    const auto t1 = {19, 10, 3, 4};
    const auto t2 = {1, 6, 7, 9};

    auto find_any_of = [](const auto& v, const auto& t)
    {
        const auto result = std::find_first_of(v.begin(), v.end(),
                                               t.begin(), t.end());
        if (result == v.end())
        {
            std::cout << "No elements of v are equal to any element of ";
            print_sequence("t = ", t);
            print_sequence("v = ", v);
        }
        else
        {
            const auto pos = std::distance(v.begin(), result);
            std::cout << "Found a match (" << *result << ") at position " << pos;
            print_sequence(", where t = ", t);
            print_sequence("v = ", v, pos);
        }
    };

    find_any_of(v, t1);
    find_any_of(v, t2);
}