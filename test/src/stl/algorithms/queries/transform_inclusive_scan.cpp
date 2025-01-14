//
// Created by wuzhengqi on 24-11-23.
//
#include <functional>
#include <iostream>
#include <iterator>
#include <numeric>
#include <vector>

int main()
{
    std::vector data{3, 1, 4, 1, 5, 9, 2, 6};

    auto times_10 = [](int x) { return x * 10; };

    std::cout << "10 times exclusive sum: ";
    std::transform_exclusive_scan(data.begin(), data.end(),
                                  std::ostream_iterator<int>(std::cout, " "),
                                  0, std::plus<int>{}, times_10);
    std::cout << "\n10 times inclusive sum: ";
    std::transform_inclusive_scan(data.begin(), data.end(),
                                  std::ostream_iterator<int>(std::cout, " "),
                                  std::plus<int>{}, times_10);
    std::cout << '\n';
}