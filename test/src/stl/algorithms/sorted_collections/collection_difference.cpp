//
// Created by wuzhengqi on 24-11-23.
//
#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>
// Copies the elements from the sorted range [first1, last1) which are not found in the sorted range [first2, last2) to the range beginning at d_first. The output range is also sorted.
//
// If [first1, last1) contains m elements that are equivalent to each other and [first2, last2) contains n elements that are equivalent to them, the final std::max(m - n, 0) elements will be copied from [first1, last1) to the output range, preserving order.
//
// 1) If [first1, last1) or [first2, last2) is not sorted with respect to operator<(until C++20)std::less{}(since C++20), the behavior is undefined.
// 3) If [first1, last1) or [first2, last2) is not sorted with respect to comp, the behavior is undefined.
template<typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v)
{
    os << '{';
    for (auto n{v.size()}; const auto& e : v)
        os << e << (--n ? ", " : "");
    return os << '}';
}

struct Order // a struct with very interesting data
{
    int order_id{};

    friend std::ostream& operator<<(std::ostream& os, const Order& ord)
    {
        return os << ord.order_id;
    }
};

int main()
{
    const std::vector<int> v1{1, 2, 5, 5, 5, 9};
    const std::vector<int> v2{2, 5, 7};
    std::vector<int> diff;

    std::set_difference(v1.begin(), v1.end(), v2.begin(), v2.end(),
                        std::inserter(diff, diff.begin()));

    std::cout << v1 << " ∖ " << v2 << " == " << diff << "\n\n";

    // we want to know which orders "cut" between old and new states:
    std::vector<Order> old_orders{{1}, {2}, {5}, {9}};
    std::vector<Order> new_orders{{2}, {5}, {7}};
    std::vector<Order> cut_orders;

    std::set_difference(old_orders.begin(), old_orders.end(),
                        new_orders.begin(), new_orders.end(),
                        std::back_inserter(cut_orders),
                        [](auto& a, auto& b) { return a.order_id < b.order_id; });

    std::cout << "old orders: " << old_orders << '\n'
              << "new orders: " << new_orders << '\n'
              << "cut orders: " << cut_orders << '\n';
}