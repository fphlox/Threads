#include <algorithm>
#include <chrono>
#include <exception>
#include <functional>
#include <iostream>
#include <iterator>
#include <string>

#include "winograd_lib.hpp"

template <typename T>
auto prompt(const std::string &message) {
  std::cout << message;

  T input;
  std::cin >> input;

  return input;
}

s21::Winograd createRandomWinograd() {
  auto width1 = prompt<std::size_t>("Enter matrix 1 width: ");
  auto height1 = prompt<std::size_t>("Enter matrix 1 height: ");
  auto width2 = prompt<std::size_t>("Enter matrix 2 width: ");
  auto height2 = prompt<std::size_t>("Enter matrix 2 height: ");

  return s21::Winograd(width1, height1, width2, height2);
}
auto createWinograd(bool isRandom) {
  if (isRandom) {
    return createRandomWinograd();
  }

  auto path1 = prompt<std::string>("Enter matrix 1 path: ");
  auto path2 = prompt<std::string>("Enter matrix 2 path: ");

  return s21::Winograd(path1, path2);
}

void printMatrix(const s21::Winograd::t_matrix &result) {
  for (auto &row : result) {
    std::copy(row.begin(), row.end(),
              std::ostream_iterator<long long>(std::cerr, " "));
    std::cerr << "\n";
  }
}
void cli(bool isRandom) {
  auto winograd = createWinograd(isRandom);

  auto n = prompt<int>("Enter the number of executions: ");

  if (isRandom) {
    std::cerr << "Matrix 1:\n";
    printMatrix(winograd.matrix1());

    std::cerr << "Matrix 2:\n";
    printMatrix(winograd.matrix2());
  }

  {
    std::cout << "Running serial winograd . . . " << std::endl;

    auto start = std::chrono::high_resolution_clock::now();

    auto result = winograd.multiplyStart(n);

    auto end = std::chrono::high_resolution_clock::now();

    auto duration =
        std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    printMatrix(result);

    std::cout << "Finished in " << duration.count() << "us" << std::endl;
  }

  {
    int availableProcessors = std::thread::hardware_concurrency();
    int threadCount = 1;

    std::cout << "Detected " << availableProcessors << " available processors"
              << std::endl;

    do {
      threadCount <<= 1;

      auto currentThreadCount =
          std::clamp(threadCount, 2, availableProcessors * 4);

      std::cout << "Running parallel winograd on " << currentThreadCount
                << " threads . . . " << std::endl;

      auto start = std::chrono::high_resolution_clock::now();

      auto result = winograd.multiplyParallelsStart(n, currentThreadCount);

      auto end = std::chrono::high_resolution_clock::now();

      auto duration =
          std::chrono::duration_cast<std::chrono::microseconds>(end - start);

      printMatrix(result);

      std::cout << "Finished in " << duration.count() << "us" << std::endl;
    } while (threadCount < availableProcessors * 4);
  }

  {
    std::cout << "Running conveyor winograd . . . " << std::endl;

    auto start = std::chrono::high_resolution_clock::now();

    auto result = winograd.multiplyConveyorStart(n);

    auto end = std::chrono::high_resolution_clock::now();

    auto duration =
        std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    printMatrix(result);

    std::cout << "Finished in " << duration.count() << "us" << std::endl;
  }
}

int main(int argc, char **argv) {
  try {
    auto isRandom = argc > 1 && std::string(argv[1]) == "random";

    cli(isRandom);
  } catch (std::exception &e) {
    std::cout << "Fatal error: " << e.what() << std::endl;
  }
}
