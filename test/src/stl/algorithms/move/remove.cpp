//
// Created by wuzhengqi on 24-11-23.
//
// Removes all elements satisfying specific criteria from the range [first, last) and returns a past-the-end iterator for the new end of the range.
//
// 1) Removes all elements that are equal to value (using operator==).
// 3) Removes all elements for which predicate p returns true.

#include <algorithm>
#include <cassert>
#include <cctype>
#include <complex>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

int main()
{
    std::string str1{"Text with some   spaces"};

    auto noSpaceEnd = std::remove(str1.begin(), str1.end(), ' ');

    // The spaces are removed from the string only logically.
    // Note, we use view, the original string is still not shrunk:
    std::cout << std::string_view(str1.begin(), noSpaceEnd)
              << " size: " << str1.size() << '\n';

    str1.erase(noSpaceEnd, str1.end());

    // The spaces are removed from the string physically.
    std::cout << str1 << " size: " << str1.size() << '\n';

    std::string str2 = "Text\n with\tsome \t  whitespaces\n\n";
    str2.erase(std::remove_if(str2.begin(),
                              str2.end(),
                              [](unsigned char x) { return std::isspace(x); }),
               str2.end());
    std::cout << str2 << '\n';

    std::vector<std::complex<double>> nums{{2, 2}, {1, 3}, {4, 8}};
    #ifdef __cpp_lib_algorithm_default_value_type
        nums.erase(std::remove(nums.begin(), nums.end(), {1, 3}), nums.end());
    #else
        nums.erase(std::remove(nums.begin(), nums.end(), std::complex<double>{1, 3}),
                   nums.end());
    #endif
    assert((nums == std::vector<std::complex<double>>{{2, 2}, {4, 8}}));
}