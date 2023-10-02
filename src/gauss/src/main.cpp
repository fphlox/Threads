#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>

#include "gauss_lib.hpp"

auto promptGauss() {
  std::cout << "Enter matrix file path: ";

  std::string path;
  std::cin >> path;

  return s21::Gauss(path);
}

auto promptN() {
  std::cout << "Enter iteration count: ";

  int n;
  std::cin >> n;

  if (n < 1) throw std::runtime_error("Invalid iteration count, should be >=1");

  return n;
}

void cli() {
  auto gauss = promptGauss();
  auto n = promptN();

  {
    std::cout << "Running serial gauss solver..." << std::endl;

    auto start = std::chrono::high_resolution_clock::now();

    auto result = gauss.solutionSale(n);

    auto end = std::chrono::high_resolution_clock::now();

    auto duration =
        std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    std::cout << "Serial gauss solver x" << n << " took:   " << duration.count()
              << "us" << std::endl;

    std::cout << "Result: ";

    std::copy(result.begin(), result.end(),
              std::ostream_iterator<double>(std::cout, " "));

    std::cout << std::endl;
  }

  {
    std::cout << "Running parallel gauss solver..." << std::endl;

    auto start = std::chrono::high_resolution_clock::now();

    auto result = gauss.solutionSaleParallels(n);

    auto end = std::chrono::high_resolution_clock::now();

    auto duration =
        std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    std::cout << "Parallel gauss solver x" << n << " took: " << duration.count()
              << "us" << std::endl;

    std::cout << "Result: ";

    std::copy(result.begin(), result.end(),
              std::ostream_iterator<double>(std::cout, " "));

    std::cout << std::endl;
  }
}

int main() {
  try {
    cli();
  } catch (std::exception &e) {
    std::cout << "Fatal error: " << e.what() << std::endl;
  }

  return (0);
}
