//
// Created by wuzhengqi on 24-11-23.
//
#include <algorithm>
#include <array>
#include <functional>
#include <iostream>
#include <string_view>

int main()
{
  std::array<int, 10> s{5, 7, 4, 2, 8, 6, 1, 9, 0, 3};

  auto print = [&s](std::string_view const rem)
  {
      for (auto a : s)
          std::cout << a << ' ';
      std::cout << ": " << rem << '\n';
  };

  std::sort(s.begin(), s.end());
  print("sorted with the default operator<");

  std::sort(s.begin(), s.end(), std::greater<int>());
  print("sorted with the standard library compare function object");

  struct
  {
    bool operator()(int a, int b) const { return a < b; }
  }
  customLess;

  std::sort(s.begin(), s.end(), customLess);
  print("sorted with a custom function object");

  std::sort(s.begin(), s.end(), [](const int& a, const int& b)
                                  {
                                      return a > b;
                                  });
  print("sorted with a lambda expression");
}