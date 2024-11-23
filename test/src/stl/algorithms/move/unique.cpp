//
// Created by wuzhengqi on 24-11-23.
//
#include <algorithm>
#include <iostream>
#include <vector>
// Removes all except the first element from every consecutive group of equivalent elements from the range [first, last) and returns a past-the-end iterator for the new end of the range.
//
// 1) Elements are compared using operator==.
//  If operator== does not establish an equivalence relation, the behavior is undefined.
// 3) Elements are compared using the given binary predicate p.
//  If p does not establish an equivalence relation, the behavior is undefined.

int main()
{
    // a vector containing several duplicate elements
    std::vector<int> v{1, 2, 1, 1, 3, 3, 3, 4, 5, 4};
    auto print = [&](int id)
    {
        std::cout << "@" << id << ": ";
        for (int i : v)
            std::cout << i << ' ';
        std::cout << '\n';
    };
    print(1);

    // remove consecutive (adjacent) duplicates
    auto last = std::unique(v.begin(), v.end());
    // v now holds {1 2 1 3 4 5 4 x x x}, where 'x' is indeterminate
    v.erase(last, v.end());
    print(2);

    // sort followed by unique, to remove all duplicates
    std::sort(v.begin(), v.end()); // {1 1 2 3 4 4 5}
    print(3);

    last = std::unique(v.begin(), v.end());
    // v now holds {1 2 3 4 5 x x}, where 'x' is indeterminate
    v.erase(last, v.end());
    print(4);
}