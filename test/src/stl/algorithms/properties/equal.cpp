//
// Created by wuzhengqi on 24-11-23.
//
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <string_view>
// Checks whether [first1, last1) and a range starting from first2 are equal:
//
// For overloads (1-4), the second range has std::distance(first1, last1) elements.
// For overloads (5-8), the second range is [first2, last2).
// 1,5) Elements are compared using operator==.
// 3,7) Elements are compared using the given binary predicate p.
constexpr bool is_palindrome(const std::string_view& s)
{
    return std::equal(s.cbegin(), s.cbegin() + s.size() / 2, s.crbegin());
}

void test(const std::string_view& s)
{
    std::cout << std::quoted(s)
              << (is_palindrome(s) ? " is" : " is not")
              << " a palindrome\n";
}

int main()
{
    test("radar");
    test("hello");
}