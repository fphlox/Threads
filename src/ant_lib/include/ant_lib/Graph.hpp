#pragma once

#include <initializer_list>
#include <iostream>
#include <string>

// #include "s21_graph/SquareMatrix.hpp"
#include "SquareMatrix.hpp"

namespace s21 {

class Graph {
 public:
  std::string name;

  Graph(const std::string &name = "");
  Graph(std::initializer_list<std::initializer_list<int>> adjacencyMatrix);

  int &edge(std::size_t vertex1, std::size_t vertex2);
  const int &edge(std::size_t vertex1, std::size_t vertex2) const;

  std::size_t vertexCount() const;

  void loadGraphFromFile(std::string filename);

  bool checkConnectivity() const;

  friend std::ostream &operator<<(std::ostream &os, const Graph &graph);

 private:
  SquareMatrix<int> adjacencyMatrix_;

  void importAsAdjacencyMatrix(std::istream &reader);
};

std::ostream &operator<<(std::ostream &os, const Graph &graph);

}  // namespace s21
