#include "winograd_lib/Winograd.hpp"

typedef std::vector<std::vector<long long>> t_matrix;
typedef std::vector<long long> t_line;

static void readMatrix(std::istream &input, t_matrix &matrix) {
  std::string str;
  long long num;

  matrix.push_back(t_line());
  std::getline(input, str);
  std::istringstream iss(str);
  while (iss >> num) {
    matrix[0].push_back(num);
  }

  std::size_t columns = matrix[0].size();
  std::size_t j = 0;
  while (std::getline(input, str)) {
    j++;
    matrix.push_back(t_line());
    std::istringstream iss(str);
    while ((iss >> num) && (matrix[j].size() < columns)) {
      matrix[j].push_back(num);
    }
    if (matrix[j].size() != columns)
      throw std::runtime_error("Матрица имеет некорректный размер!");
  }
}

s21::Winograd::Winograd(std::string filename1, std::string filename2) {
  std::ifstream file(filename1);

  if (!file)
    throw std::runtime_error(
        "Указанный файл не существует или недоступен для чтения!");
  readMatrix(file, _matrix1);
  file.close();

  file.open(filename2);
  if (!file)
    throw std::runtime_error(
        "Указанный файл не существует или недоступен для чтения!");
  readMatrix(file, _matrix2);

  if (intersection() != mat2_rows())
    throw std::runtime_error("Матрицы не могут быть перемножены!");
}

s21::Winograd::Winograd(std::size_t rows1, std::size_t columns1,
                        std::size_t rows2, std::size_t columns2) {
  std::random_device gen;
  std::uniform_int_distribution<long long> dist(-10000, 10000);

  _matrix1.resize(rows1, t_line(columns1));
  for (std::size_t i = 0; i < rows1; i++) {
    for (std::size_t j = 0; j < columns1; j++) {
      _matrix1[i][j] = dist(gen);
    }
  }

  _matrix2.resize(rows2, t_line(columns2));
  for (std::size_t i = 0; i < rows2; i++) {
    for (std::size_t j = 0; j < columns2; j++) {
      _matrix2[i][j] = dist(gen);
    }
  }
}

s21::Winograd::Winograd(Winograd const &src) { *this = src; }

s21::Winograd &s21::Winograd::operator=(Winograd const &rhs) {
  if (this != &rhs) {
    this->_matrix1 = rhs._matrix1;
    this->_matrix2 = rhs._matrix2;
  }
  return (*this);
}

std::size_t s21::Winograd::rows() const { return _matrix1.size(); }

std::size_t s21::Winograd::mat2_rows() const { return _matrix2.size(); }

std::size_t s21::Winograd::intersection() const { return _matrix1[0].size(); }

std::size_t s21::Winograd::columns() const { return _matrix2[0].size(); }

// вычисление rowFactors для matrix1
t_line s21::Winograd::rowFactor() const {
  t_line row_factor(rows(), 0);
  for (std::size_t j = 0; j < rows(); j++) {
    for (std::size_t i = 0; i < intersection() / 2; i++) {
      row_factor[j] += _matrix1[j][2 * i] * _matrix1[j][2 * i + 1];
    }
  }
  return (row_factor);
}

// вычисление columnFactors для matrix2
t_line s21::Winograd::columnFactor() const {
  t_line column_factor(columns(), 0);
  for (std::size_t j = 0; j < columns(); j++) {
    for (std::size_t i = 0; i < intersection() / 2; i++) {
      column_factor[j] += _matrix2[2 * i][j] * _matrix2[2 * i + 1][j];
    }
  }
  return (column_factor);
}

// вычисление матрицы matrix_ret
t_matrix s21::Winograd::multiplyWork(t_line row_factor,
                                     t_line column_factor) const {
  t_matrix matrix_ret(rows(), t_line(columns(), 0));
  for (std::size_t k = 0; k < rows(); k++) {
    for (std::size_t j = 0; j < columns(); j++) {
      long long temp = -row_factor[k] - column_factor[j];
      for (std::size_t i = 0; i < intersection() / 2; i++) {
        temp += (_matrix1[k][2 * i] + _matrix2[2 * i + 1][j]) *
                (_matrix1[k][2 * i + 1] + _matrix2[2 * i][j]);
      }
      matrix_ret[k][j] = temp;
    }
  }
  return (matrix_ret);
}

// прибавление членов в случае нечетной общей размерности
void s21::Winograd::addition(t_matrix &matrix_ret) const {
  if (intersection() % 2 == 1) {
    for (std::size_t j = 0; j < rows(); j++) {
      for (std::size_t i = 0; i < columns(); i++) {
        matrix_ret[j][i] +=
            _matrix1[j][intersection() - 1] * _matrix2[intersection() - 1][i];
      }
    }
  }
}

t_matrix s21::Winograd::multiply() const {
  t_line row_factor = rowFactor();
  t_line column_factor = columnFactor();
  t_matrix matrix_ret = multiplyWork(row_factor, column_factor);
  addition(std::ref(matrix_ret));
  return matrix_ret;
}

t_matrix s21::Winograd::multiplyStart(int iters) const {
  t_matrix matrix_ret;

  if (iters <= 0)
    throw std::runtime_error(
        "Ошибка! Введено отрицательное или нулевое число итераций алгоритма");

  for (int i = 0; i < iters; i++) matrix_ret = multiply();

  return matrix_ret;
}

void s21::Winograd::multiplyParallels(t_matrix &matrix_ret, std::mutex &mtx,
                                      const Winograd &win, int iters) {
  t_matrix matrix_pre_ret;

  for (int i = 0; i < iters; i++) {
    matrix_pre_ret = win.multiply();
    mtx.lock();
    matrix_ret = matrix_pre_ret;
    mtx.unlock();
  }
}

t_matrix s21::Winograd::multiplyParallelsStart(int iters,
                                               int threads_num) const {
  t_matrix matrix_ret;

  if (iters <= 0)
    throw std::runtime_error(
        "Ошибка! Введено отрицательное или нулевое число итераций алгоритма");

  if (threads_num <= 0)
    throw std::runtime_error(
        "Ошибка! Введено отрицательное или нулевое число потоков выполняющих "
        "алгоритм");

  if (iters < threads_num) threads_num = iters;
  std::vector<std::thread> threads;
  std::mutex mtx;

  int rep_per_thread = iters / threads_num;
  ;
  int i = 0;
  if (iters % threads_num != 0) {
    threads.emplace_back(multiplyParallels, std::ref(matrix_ret), std::ref(mtx),
                         std::ref(*this), rep_per_thread + iters % threads_num);
    ++i;
  }
  for (; i < threads_num; ++i)
    threads.emplace_back(multiplyParallels, std::ref(matrix_ret), std::ref(mtx),
                         std::ref(*this), rep_per_thread);
  for (auto &thread : threads) thread.join();
  return matrix_ret;
}

t_matrix s21::Winograd::multiplyConveyorStart(int iterations) const {
  if (iterations <= 0)
    throw std::runtime_error(
        "Ошибка! Введено отрицательное или нулевое число итераций алгоритма");

  std::queue<t_line> row_factors;
  std::queue<t_line> column_factors;
  std::queue<t_matrix> matrixes_ret;
  int const iters = iterations;
  std::condition_variable cv_row;
  std::condition_variable cv_column;
  std::condition_variable cv_matrix;
  s21::Winograd win = *this;
  std::mutex mtx_row;
  std::mutex mtx_column;
  std::mutex mtx_matrix;
  // вычисление rowFactors для matrix1
  auto rowFactor = [&]() {
    for (int i = 0; i < iters; i++) {
      row_factors.push(win.rowFactor());
      std::lock_guard<std::mutex> lock(mtx_row);
      cv_row.notify_one();
    }
  };

  // вычисление columnFactors для matrix2
  auto columnFactor = [&]() {
    for (int i = 0; i < iters; i++) {
      column_factors.push(win.columnFactor());
      std::lock_guard<std::mutex> lock(mtx_column);
      cv_column.notify_one();
    }
  };

  // вычисление матрицы matrix_ret
  auto matrixRet = [&]() {
    std::unique_lock<std::mutex> mrow(mtx_row);
    std::unique_lock<std::mutex> mcolumn(mtx_column);
    for (int i = 0; i < iters; i++) {
      cv_row.wait(mrow, [&row_factors]() { return !row_factors.empty(); });
      cv_column.wait(mcolumn,
                     [&column_factors]() { return !column_factors.empty(); });
      matrixes_ret.push(
          win.multiplyWork(row_factors.front(), column_factors.front()));
      row_factors.pop();
      column_factors.pop();
      std::lock_guard<std::mutex> lock(mtx_matrix);
      cv_matrix.notify_one();
    }
  };

  // прибавление членов в случае нечетной общей размерности
  auto addition = [&]() {
    std::unique_lock<std::mutex> mmatrix(mtx_matrix);
    for (int i = 0; i < iters; i++) {
      cv_matrix.wait(mmatrix,
                     [&matrixes_ret]() { return !matrixes_ret.empty(); });
      win.addition(matrixes_ret.front());
      if (i < iters - 1) matrixes_ret.pop();
    }
  };

  std::thread row_factor_thread(rowFactor);
  std::thread column_factor_thread(columnFactor);
  std::thread matrix_multiply_thread(matrixRet);
  std::thread addition_thread(addition);
  row_factor_thread.join();
  column_factor_thread.join();
  matrix_multiply_thread.join();
  addition_thread.join();

  return matrixes_ret.front();
}

const std::vector<std::vector<long long>> &s21::Winograd::matrix1() const {
  return _matrix1;
}
const std::vector<std::vector<long long>> &s21::Winograd::matrix2() const {
  return _matrix2;
}
