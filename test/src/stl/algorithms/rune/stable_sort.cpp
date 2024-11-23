//
// Created by wuzhengqi on 24-11-23.
//

#include <algorithm>
#include <array>
#include <iostream>
#include <string>
#include <vector>

// Sorts the elements in the range [first, last) in non-descending order. The order of equivalent elements is guaranteed to be preserved.
struct Employee
{
    int age;
    std::string name; // Does not participate in comparisons
};

bool operator<(const Employee& lhs, const Employee& rhs)
{
    return lhs.age < rhs.age;
}

#if __cpp_lib_constexpr_algorithms >= 202306L
consteval auto get_sorted()
{
    auto v = std::array{3, 1, 4, 1, 5, 9};
    std::stable_sort(v.begin(), v.end());
    return v;
}
static_assert(std::ranges::is_sorted(get_sorted()));
#endif

int main()
{
    std::vector<Employee> v{{108, "Zaphod"}, {32, "Arthur"}, {108, "Ford"}};

    std::stable_sort(v.begin(), v.end());

    for (const Employee& e : v)
        std::cout << e.age << ", " << e.name << '\n';

    std::stable_sort(v.begin(), v.end(), [](const Employee& lhs, const Employee& rhs)
    {
      return lhs.age > rhs.age;
    });

    for (const Employee& e : v)
        std::cout << e.age << ", " << e.name << '\n';
}