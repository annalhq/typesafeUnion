#include "../include/variant.hpp"
#include <cassert>
#include <iostream>
#include <string>

void test_default_construction() {
  variant<int, std::string, double> v;
  assert(v.get_index() == 0);
  assert(v.get<0>() == 0);
  std::cout << "test_default_construction passed\n";
}

void test_int_construction() {
  variant<int, std::string, double> v(42);
  assert(v.get_index() == 0);
  assert(v.get<0>() == 42);
  std::cout << "test_int_construction passed\n";
}

void test_string_construction() {
  std::string s = "hello";
  variant<int, std::string, double> v(s);
  assert(v.get_index() == 1);
  assert(v.get<1>() == "hello");
  std::cout << "test_string_construction passed\n";
}

void test_double_construction() {
  variant<int, std::string, double> v(3.14);
  assert(v.get_index() == 2);
  assert(v.get<2>() == 3.14);
  std::cout << "test_double_construction passed\n";
}

void test_assignment_int() {
  variant<int, std::string, double> v;
  v = 100;
  assert(v.get_index() == 0);
  assert(v.get<0>() == 100);
  std::cout << "test_assignment_int passed\n";
}

void test_assignment_string() {
  variant<int, std::string, double> v;
  std::string s = "world";
  v = s;
  assert(v.get_index() == 1);
  assert(v.get<1>() == "world");
  std::cout << "test_assignment_string passed\n";
}

void test_assignment_double() {
  variant<int, std::string, double> v;
  v = 2.718;
  assert(v.get_index() == 2);
  assert(v.get<2>() == 2.718);
  std::cout << "test_assignment_double passed\n";
}

void test_reassignment() {
  variant<int, std::string, double> v(42);
  assert(v.get_index() == 0);

  std::string s = "changed";
  v = s;
  assert(v.get_index() == 1);
  assert(v.get<1>() == "changed");

  v = 99.9;
  assert(v.get_index() == 2);
  assert(v.get<2>() == 99.9);

  std::cout << "test_reassignment passed\n";
}

void test_get_first_type() {
  static_assert(std::is_same_v<get_first_type<int, double, char>::type, int>);
  static_assert(
      std::is_same_v<get_first_type<std::string, int>::type, std::string>);
  std::cout << "test_get_first_type passed\n";
}

void test_get_index_type() {
  static_assert(get_index_type<int, int, double, char>::value == 0);
  static_assert(get_index_type<double, int, double, char>::value == 1);
  static_assert(get_index_type<char, int, double, char>::value == 2);
  std::cout << "test_get_index_type passed\n";
}

int main() {
  std::cout << "Running variant tests...\n\n";

  test_default_construction();
  test_int_construction();
  test_string_construction();
  test_double_construction();
  test_assignment_int();
  test_assignment_string();
  test_assignment_double();
  test_reassignment();
  test_get_first_type();
  test_get_index_type();

  std::cout << "\nAll tests passed!\n";
}
