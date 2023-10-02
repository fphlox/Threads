#pragma once

#include <ctime>
#include <iostream>
#include <limits>
#include <mutex>
#include <stdexcept>
#include <thread>
// #include <chrono>
#include "Graph.hpp"
#include "Queue.hpp"

namespace s21 {

class GraphAlgorithms {
 private:
  struct t_deikstra {
    bool visited = false;
    unsigned long int mark = 4294967295;
    unsigned int number;
  };

 public:
  struct t_ant {
    std::vector<int> way;
    double distance = 0;
    unsigned int way_back = 0;
    bool ant_can_go = true;
    double feromon = 0;
  };

  struct TsmResult {
    std::vector<int>
        vertices;  // массив с искомым маршрутом (с порядком обхода вершин).
                   // Вместо int* можно использовать std::vector<int>
    double distance;  // длина этого маршрута
  };

  static int getShortestPathBetweenVertices(Graph &graph, int vertex1,
                                            int vertex2);

  static TsmResult solveTravelingSalesmanProblem(Graph &graph);

  static TsmResult solveTravelingSalesmanProblemParallels(Graph &graph);

  std::vector<time_t> сomparisonMethods(s21::Graph &graph, int iter);

 private:
  static void antMove(t_ant &ant, s21::SquareMatrix<double> &fero_matrix,
                      s21::Graph &graph, std::size_t &dead_end);
  static void deadEnd(std::size_t dead_end, s21::Graph &graph);
  static void sprayingFeromons(t_ant &ant, s21::Graph &graph,
                               s21::SquareMatrix<double> &fero_matrix);
  static void minWay(t_ant &ant, TsmResult &rezult, bool &need_retry);
  static void antZeroing(t_ant &ant, int number);

  static void antDo(t_ant &ant, s21::Graph &graph,
                    s21::SquareMatrix<double> &fero_matrix,
                    std::size_t &alg_can_work, std::mutex &mtx,
                    bool &need_retry, TsmResult &rezult, int &retry);
};

}  // namespace s21
