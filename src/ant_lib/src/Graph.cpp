#include "ant_lib/Graph.hpp"

#include <fstream>
#include <limits>
#include <stdexcept>
#include <vector>

#include "ant_lib/SquareMatrix.hpp"

using Graph = s21::Graph;

Graph::Graph(const std::string &name) : name(name), adjacencyMatrix_(0) {}

Graph::Graph(std::initializer_list<std::initializer_list<int>> adjacencyMatrix)
    : adjacencyMatrix_(adjacencyMatrix) {}

int &Graph::edge(std::size_t vertex1, std::size_t vertex2) {
  return adjacencyMatrix_(vertex1, vertex2);
}

const int &Graph::edge(std::size_t vertex1, std::size_t vertex2) const {
  return adjacencyMatrix_(vertex1, vertex2);
}

std::size_t Graph::vertexCount() const { return adjacencyMatrix_.size(); }

void Graph::importAsAdjacencyMatrix(std::istream &reader) {
  int vertexCount;

  reader >> vertexCount;

  if (vertexCount < 1) {
    throw std::runtime_error("invalid graph size");
  }

  SquareMatrix<int> adjacencyMatrix(vertexCount);

  for (auto i = 0; i < vertexCount; i++) {
    for (auto j = 0; j < vertexCount; j++) {
      reader >> adjacencyMatrix(i, j);
    }
  }

  adjacencyMatrix_ = std::move(adjacencyMatrix);
}

bool Graph::checkConnectivity() const {
  s21::SquareMatrix<long> matrix(vertexCount());

  for (std::size_t j = 0; j < vertexCount(); j++) {
    for (std::size_t i = 0; i < vertexCount(); i++) {
      matrix(j, i) = edge(j, i);
      if (matrix(j, i) == 0)
        matrix(j, i) = std::numeric_limits<unsigned>::max();
      if (i == j) matrix(j, i) = 0;
    }
  }

  for (std::size_t k = 0; k < vertexCount(); k++) {
    for (std::size_t j = 0; j < vertexCount(); j++) {
      for (std::size_t i = 0; i < vertexCount(); i++) {
        matrix(j, i) = std::min(matrix(j, i), matrix(j, k) + matrix(k, i));
      }
    }
  }

  for (std::size_t j = 0; j < vertexCount(); j++) {
    for (std::size_t i = 0; i < vertexCount(); i++) {
      if (matrix(j, i) == std::numeric_limits<unsigned>::max()) return false;
    }
  }
  return true;
}

void Graph::loadGraphFromFile(std::string filename) {
  std::ifstream reader(filename);

  if (reader.fail()) {
    throw std::runtime_error("Could not open graph file");
  }

  if (reader.eof()) {
    throw std::runtime_error("bebra");
  }

  importAsAdjacencyMatrix(reader);
  if (checkConnectivity() == false)
    throw std::runtime_error("Ошибка! Матрица не связна!");
}

std::ostream &s21::operator<<(std::ostream &os, const Graph &graph) {
  os << graph.adjacencyMatrix_;

  return os;
}
