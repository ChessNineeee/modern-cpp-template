//
// Created by wuzhengqi on 24-11-23.
//
#include <complex>
#include <iostream>
#include <numeric>
#include <string_view>
#include <vector>
// 1) Erases all elements that compare equal to value from the container. Equivalent to
// auto it = std::remove(c.begin(), c.end(), value);
// auto r = c.end() - it;
// c.erase(it, c.end());
// return r;
// 2) Erases all elements that satisfy the predicate pred from the container. Equivalent to
// auto it = std::remove_if(c.begin(), c.end(), pred);
// auto r = c.end() - it;
// c.erase(it, c.end());
// return r;
void println(std::string_view comment, const auto& c)
{
    std::cout << comment << '[';
    bool first{true};
    for (const auto& x : c)
        std::cout << (first ? first = false, "" : ", ") << x;
    std::cout << "]\n";
}

int main()
{
    std::vector<char> cnt(10);
    std::iota(cnt.begin(), cnt.end(), '0');
    println("Initially, cnt = ", cnt);

    std::erase(cnt, '3');
    println("After erase '3', cnt = ", cnt);

    auto erased = std::erase_if(cnt, [](char x) { return (x - '0') % 2 == 0; });
    println("After erase all even numbers, cnt = ", cnt);
    std::cout << "Erased even numbers: " << erased << '\n';

    std::vector<std::complex<double>> nums{{2, 2}, {4, 2}, {4, 8}, {4, 2}};
    #ifdef __cpp_lib_algorithm_default_value_type
        std::erase(nums, {4, 2});
    #else
        std::erase(nums, std::complex<double>{4, 2});
    #endif
    println("After erase {4, 2}, nums = ", nums);
}