#include <algorithm>
#include <chrono>
#include <exception>
#include <iostream>
#include <iterator>
#include <numeric>
#include <stdexcept>
#include <string>

#include "ant_lib.hpp"

s21::Graph promptGraph() {
  std::cout << "Enter graph file path: ";

  std::string path;
  std::cin >> path;

  s21::Graph graph;

  graph.loadGraphFromFile(path);

  return graph;
}

int promptN() {
  std::cout << "Enter iteration count: ";

  int n;
  std::cin >> n;

  if (n < 1) throw std::runtime_error("Invalid iteration count, should be >=1");

  return n;
}

void cli() {
  auto graph = promptGraph();

  std::cout << "Graph:\n" << graph << std::endl;

  auto n = promptN();

  {
    auto result = s21::GraphAlgorithms::solveTravelingSalesmanProblem(graph);

    std::cout << "Serial TSP solution:   ";
    std::copy(result.vertices.begin(), result.vertices.end(),
              std::ostream_iterator<int>(std::cout, " "));

    std::cout << "(" << result.distance << ")" << std::endl;
  }

  {
    auto result =
        s21::GraphAlgorithms::solveTravelingSalesmanProblemParallels(graph);

    std::cout << "Parallel TSP solution: ";
    std::copy(result.vertices.begin(), result.vertices.end(),
              std::ostream_iterator<int>(std::cout, " "));

    std::cout << "(" << result.distance << ")" << std::endl;
  }

  {
    std::cout << "Running serial benchmark ..." << std::endl;

    auto start = std::chrono::high_resolution_clock::now();

    for (auto i = 0; i < n; i++)
      s21::GraphAlgorithms::solveTravelingSalesmanProblem(graph);

    auto end = std::chrono::high_resolution_clock::now();

    auto duration =
        std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    std::cout << "Serial ant algorithm x" << n
              << " took:   " << duration.count() << "us" << std::endl;
  }

  {
    std::cout << "Running parallel benchmark ..." << std::endl;

    auto start = std::chrono::high_resolution_clock::now();

    for (auto i = 0; i < n; i++)
      s21::GraphAlgorithms::solveTravelingSalesmanProblemParallels(graph);

    auto end = std::chrono::high_resolution_clock::now();

    auto duration =
        std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    std::cout << "Parallel ant algorithm x" << n
              << " took: " << duration.count() << "us" << std::endl;
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
