#pragma once

#include <cmath>
#include <condition_variable>
#include <fstream>
#include <iostream>
#include <mutex>
#include <queue>
#include <random>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

namespace s21 {
class Winograd {
 public:
  typedef std::vector<std::vector<long long>> t_matrix;
  typedef std::vector<long long> t_line;

 public:
  Winograd(std::string filename1, std::string filename2);
  Winograd(std::size_t rows1, std::size_t columns1, std::size_t rows2,
           std::size_t columns2);
  Winograd(Winograd const &src);

  Winograd &operator=(s21::Winograd const &rhs);

  std::size_t rows() const;
  std::size_t mat2_rows() const;
  std::size_t intersection() const;
  std::size_t columns() const;

  const std::vector<std::vector<long long>> &matrix1() const;
  const std::vector<std::vector<long long>> &matrix2() const;

  t_matrix multiplyStart(int iters) const;
  t_matrix multiplyParallelsStart(int iters, int threads_num) const;
  t_matrix multiplyConveyorStart(int iters) const;

 private:
  std::vector<std::vector<long long>> _matrix1;
  std::vector<std::vector<long long>> _matrix2;

  t_line rowFactor() const;
  t_line columnFactor() const;
  t_matrix multiplyWork(t_line row_factor, t_line column_factor) const;
  void addition(t_matrix &matrix_ret) const;

  t_matrix multiply() const;
  static void multiplyParallels(t_matrix &matrix_ret, std::mutex &mtx,
                                const Winograd &win, int iters);
};
}  // namespace s21
