#pragma once

#include <cmath>
#include <fstream>
#include <iostream>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

namespace s21 {

class Gauss {
 public:
  Gauss();
  Gauss(std::string filename);

  std::vector<double> solutionSale(int iters) const;
  std::vector<double> solutionSaleParallels(int iters) const;

 private:
  std::vector<std::vector<double>> _matrix;
};

}  // namespace s21
