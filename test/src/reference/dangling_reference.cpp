//
// Created by wuzhengqi on 24-12-1.
//
#include <iostream>
#include <string>

class Example {
public:
  std::string_view view;

  void setView(const std::string& str) {
    view = str; // 保存了对 str 的引用
  }

  void print() const {
    std::cout << view << std::endl; // 尝试使用已经销毁的引用
  }
};

int main() {
  Example ex;

  // 临时字符串对象
  ex.setView(std::string("Temporary")); // 临时对象会在函数调用后销毁

  // 悬空引用，访问销毁的对象
  ex.print(); // 未定义行为：视图指向的内存已释放

  return 0;
}
