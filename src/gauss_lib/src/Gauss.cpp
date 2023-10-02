#include "gauss_lib/Gauss.hpp"

#include <mutex>

static void readMatrix(std::istream &input,
                       std::vector<std::vector<double>> &matrix) {
  std::string str;
  std::size_t matrix_size;
  double num;

  std::getline(input, str);
  matrix_size = static_cast<std::size_t>(std::stoi(str));
  for (auto j = 0ul; j < matrix_size; j++) {
    matrix.push_back(std::vector<double>());
    std::getline(input, str);
    std::istringstream iss(str);
    while ((iss >> num) && (matrix[j].size() < matrix_size + 1)) {
      matrix[j].push_back(num);
    }
    if (matrix[j].size() != matrix_size + 1) {
      throw std::runtime_error("Строка матрицы не полна!");
    }
  }
}

s21::Gauss::Gauss() { readMatrix(std::cin, _matrix); }

s21::Gauss::Gauss(std::string filename) {
  std::ifstream file(filename);

  if (!file)
    throw std::runtime_error(
        "Указанный файл не существует или недоступен для чтения!");
  readMatrix(file, _matrix);
}

static std::vector<double> gauss(std::vector<std::vector<double>> &matrix) {
  const int rows = matrix.size();

  // Создание списка "нулевости" строк
  std::vector<std::size_t> zeros(rows, 0);
  for (int j = 0; j < rows; j++) {
    int i = 0;
    while ((i < rows) && (matrix[j][i] == 0)) {
      zeros[j]++;
      i++;
    }
  }

  // Сортировка матрицы по имеющимся нулям
  for (int j = 0; j < rows - 1; j++) {
    for (int i = 0; i < rows - j - 1; i++) {
      if (zeros[i] > zeros[i + 1]) {
        std::swap(matrix[i], matrix[i + 1]);
        std::swap(zeros[i], zeros[i + 1]);
      }
    }
  }

  // Преображение матрицы в полу-нулевую
  for (int k = 0; k < rows - 1; k++) {
    for (int j = k + 1; j < rows; j++) {
      double factor = matrix[j][k] / matrix[k][k];
      for (int i = k; i < rows + 1; i++) matrix[j][i] -= factor * matrix[k][i];
    }
  }

  //Вычисление ответов
  std::vector<double> ansvers(rows, 0);
  double sum = 0;
  for (int j = rows - 1; j >= 0; j--) {
    sum = matrix[j][rows];
    for (int i = j; i < rows; i++) {
      sum -= matrix[j][i] * ansvers[i];
    }
    ansvers[j] = sum / matrix[j][j];
  }

  for (auto ansver : ansvers) {
    if (std::isinf(ansver) || std::isnan(ansver))
      throw std::runtime_error(
          "Ошибка! Данная система уравнений не имеет полноценного решения!");
  }

  return (ansvers);
}

std::vector<double> s21::Gauss::solutionSale(int iters) const {
  std::vector<double> ansvers;
  std::vector<std::vector<double>> matrix;

  if (iters <= 0)
    throw std::runtime_error(
        "Ошибка! Введено отрицательное или нулевое число итераций алгоритмa");

  for (int i = 0; i < iters; i++) {
    matrix = _matrix;
    ansvers = gauss(matrix);
  }
  return (ansvers);
}

static void gaussParallels(std::vector<std::vector<double>> &matrix,
                           std::mutex &mtx, std::vector<double> &ansvers,
                           uint32_t iters) {
  for (auto i = 0ul; i < iters; i++) {
    std::vector<std::vector<double>> matrix_cp = matrix;
    std::vector<double> ansver = gauss(matrix_cp);
    mtx.lock();
    ansvers = ansver;
    mtx.unlock();
  }
}

std::vector<double> s21::Gauss::solutionSaleParallels(int iters) const {
  if (iters <= 0)
    throw std::runtime_error(
        "Ошибка! Введено отрицательное или нулевое число итераций алгоритма");

  std::vector<double> ansvers;
  std::vector<std::vector<double>> matrix = _matrix;
  std::mutex mtx;

  int threads_num = 20;
  if (iters < threads_num) threads_num = iters;

  int rep_per_thread = iters / threads_num;
  ;
  std::vector<std::thread> threads;
  int i = 0;
  if (iters % threads_num != 0) {
    threads.emplace_back(gaussParallels, std::ref(matrix), std::ref(mtx),
                         std::ref(ansvers),
                         rep_per_thread + iters % threads_num);
    ++i;
  }
  for (; i < threads_num; ++i)
    threads.emplace_back(gaussParallels, std::ref(matrix), std::ref(mtx),
                         std::ref(ansvers), rep_per_thread);
  for (auto &thread : threads) thread.join();
  return (ansvers);
}