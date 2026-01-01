#include "../include/variant.hpp"
#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>
#include <variant>

constexpr size_t ITERATIONS = 10'000'000;
constexpr size_t WARMUP_ITERATIONS = 100'000;

template <typename F> double measure_ns(F &&func) {
  auto start = std::chrono::high_resolution_clock::now();
  func();
  auto end = std::chrono::high_resolution_clock::now();
  return std::chrono::duration<double, std::nano>(end - start).count();
}

// Prevent compiler from optimizing away the result
template <typename T> void do_not_optimize(T const &value) {
  asm volatile("" : : "r,m"(value) : "memory");
}

void print_result(const std::string &test_name, double custom_ns,
                  double std_ns) {
  double custom_per_op = custom_ns / ITERATIONS;
  double std_per_op = std_ns / ITERATIONS;
  double ratio = std_per_op / custom_per_op;

  std::cout << std::left << std::setw(30) << test_name << std::right
            << std::setw(12) << std::fixed << std::setprecision(2)
            << custom_per_op << " ns" << std::setw(12) << std_per_op << " ns"
            << std::setw(10) << std::setprecision(2) << ratio << "x\n";
}

void benchmark_construction() {
  // Warmup
  for (size_t i = 0; i < WARMUP_ITERATIONS; ++i) {
    variant<int, double, std::string> v(42);
    do_not_optimize(v);
  }

  double custom_time = measure_ns([&]() {
    for (size_t i = 0; i < ITERATIONS; ++i) {
      variant<int, double, std::string> v(static_cast<int>(i));
      do_not_optimize(v);
    }
  });

  double std_time = measure_ns([&]() {
    for (size_t i = 0; i < ITERATIONS; ++i) {
      std::variant<int, double, std::string> v(static_cast<int>(i));
      do_not_optimize(v);
    }
  });

  print_result("Construction (int)", custom_time, std_time);
}

void benchmark_construction_string() {
  std::string test_str = "hello world";

  // Warmup
  for (size_t i = 0; i < WARMUP_ITERATIONS; ++i) {
    variant<int, double, std::string> v(test_str);
    do_not_optimize(v);
  }

  double custom_time = measure_ns([&]() {
    for (size_t i = 0; i < ITERATIONS; ++i) {
      variant<int, double, std::string> v(test_str);
      do_not_optimize(v);
    }
  });

  double std_time = measure_ns([&]() {
    for (size_t i = 0; i < ITERATIONS; ++i) {
      std::variant<int, double, std::string> v(test_str);
      do_not_optimize(v);
    }
  });

  print_result("Construction (string)", custom_time, std_time);
}

void benchmark_get() {
  variant<int, double, std::string> custom_v(42);
  std::variant<int, double, std::string> std_v(42);

  // Warmup
  for (size_t i = 0; i < WARMUP_ITERATIONS; ++i) {
    auto &val = custom_v.get<0>();
    do_not_optimize(val);
  }

  double custom_time = measure_ns([&]() {
    for (size_t i = 0; i < ITERATIONS; ++i) {
      auto &val = custom_v.get<0>();
      do_not_optimize(val);
    }
  });

  double std_time = measure_ns([&]() {
    for (size_t i = 0; i < ITERATIONS; ++i) {
      auto &val = std::get<0>(std_v);
      do_not_optimize(val);
    }
  });

  print_result("Get (index 0)", custom_time, std_time);
}

void benchmark_get_deep() {
  variant<int, double, std::string, float, long> custom_v(100L);
  std::variant<int, double, std::string, float, long> std_v(100L);

  // Warmup
  for (size_t i = 0; i < WARMUP_ITERATIONS; ++i) {
    auto &val = custom_v.get<4>();
    do_not_optimize(val);
  }

  double custom_time = measure_ns([&]() {
    for (size_t i = 0; i < ITERATIONS; ++i) {
      auto &val = custom_v.get<4>();
      do_not_optimize(val);
    }
  });

  double std_time = measure_ns([&]() {
    for (size_t i = 0; i < ITERATIONS; ++i) {
      auto &val = std::get<4>(std_v);
      do_not_optimize(val);
    }
  });

  print_result("Get (index 4, deep)", custom_time, std_time);
}

void benchmark_assignment() {
  variant<int, double, std::string> custom_v(0);
  std::variant<int, double, std::string> std_v(0);

  // Warmup
  for (size_t i = 0; i < WARMUP_ITERATIONS; ++i) {
    custom_v = static_cast<int>(i);
    do_not_optimize(custom_v);
  }

  double custom_time = measure_ns([&]() {
    for (size_t i = 0; i < ITERATIONS; ++i) {
      custom_v = static_cast<int>(i);
      do_not_optimize(custom_v);
    }
  });

  double std_time = measure_ns([&]() {
    for (size_t i = 0; i < ITERATIONS; ++i) {
      std_v = static_cast<int>(i);
      do_not_optimize(std_v);
    }
  });

  print_result("Assignment (same type)", custom_time, std_time);
}

void benchmark_assignment_type_change() {
  variant<int, double, std::string> custom_v(0);
  std::variant<int, double, std::string> std_v(0);

  // Warmup
  for (size_t i = 0; i < WARMUP_ITERATIONS; ++i) {
    if (i % 2 == 0)
      custom_v = static_cast<int>(i);
    else
      custom_v = static_cast<double>(i);
    do_not_optimize(custom_v);
  }

  double custom_time = measure_ns([&]() {
    for (size_t i = 0; i < ITERATIONS; ++i) {
      if (i % 2 == 0)
        custom_v = static_cast<int>(i);
      else
        custom_v = static_cast<double>(i);
      do_not_optimize(custom_v);
    }
  });

  double std_time = measure_ns([&]() {
    for (size_t i = 0; i < ITERATIONS; ++i) {
      if (i % 2 == 0)
        std_v = static_cast<int>(i);
      else
        std_v = static_cast<double>(i);
      do_not_optimize(std_v);
    }
  });

  print_result("Assignment (type change)", custom_time, std_time);
}

void benchmark_index() {
  variant<int, double, std::string> custom_v(3.14);
  std::variant<int, double, std::string> std_v(3.14);

  // Warmup
  for (size_t i = 0; i < WARMUP_ITERATIONS; ++i) {
    auto idx = custom_v.get_index();
    do_not_optimize(idx);
  }

  double custom_time = measure_ns([&]() {
    for (size_t i = 0; i < ITERATIONS; ++i) {
      auto idx = custom_v.get_index();
      do_not_optimize(idx);
    }
  });

  double std_time = measure_ns([&]() {
    for (size_t i = 0; i < ITERATIONS; ++i) {
      auto idx = std_v.index();
      do_not_optimize(idx);
    }
  });

  print_result("Index query", custom_time, std_time);
}

int main() {
  std::cout << "==========================================================\n";
  std::cout << "        variant vs std::variant Benchmark\n";
  std::cout << "        Iterations: " << ITERATIONS << "\n";
  std::cout << "==========================================================\n\n";

  std::cout << std::left << std::setw(30) << "Test" << std::right
            << std::setw(15) << "Custom" << std::setw(15) << "std::variant"
            << std::setw(10) << "Ratio\n";
  std::cout << std::string(70, '-') << "\n";

  benchmark_construction();
  benchmark_construction_string();
  benchmark_get();
  benchmark_get_deep();
  benchmark_assignment();
  benchmark_assignment_type_change();
  benchmark_index();

  std::cout << "\n";
  std::cout << "Note: Ratio > 1.0 means custom variant is faster\n";
  std::cout << "      Ratio < 1.0 means std::variant is faster\n";

  return 0;
}
