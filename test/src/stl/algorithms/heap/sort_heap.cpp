//
// Created by wuzhengqi on 24-11-23.
//

#include <algorithm>
#include <iostream>
#include <string_view>
#include <vector>
void println(std::string_view fmt, const auto& v)
{
    for (std::cout << fmt; const auto &i : v)
        std::cout << i << ' ';
    std::cout << '\n';
}

// Converts the heap [first, last) into a sorted range. The heap property is no longer maintained.
int main()
{
    std::vector<int> v{3, 1, 4, 1, 5, 9};

    std::make_heap(v.begin(), v.end());
    println("after make_heap, v: ", v);

    std::sort_heap(v.begin(), v.end());
    println("after sort_heap, v: ", v);

    std::make_heap(v.begin(), v.end(), std::greater<int>{});
    println("after make_heap, v: ", v);

    std::sort_heap(v.begin(), v.end(), std::greater<int>{});
    println("after sort_heap, v: ", v);
}