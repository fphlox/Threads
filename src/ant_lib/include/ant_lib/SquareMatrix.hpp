#pragma once

#include <initializer_list>
#include <iostream>
#include <memory>
#include <stdexcept>

namespace s21 {

template <typename T>
class SquareMatrix {
 public:
  SquareMatrix(std::initializer_list<std::initializer_list<T>> rows);

  explicit SquareMatrix(std::size_t size);

  T &operator()(std::size_t x, std::size_t y);
  const T &operator()(std::size_t x, std::size_t y) const;

  std::size_t size() const;

  bool operator==(const SquareMatrix &matrix) const;
  bool operator!=(const SquareMatrix &matrix) const;

  template <typename U>
  friend std::ostream &operator<<(std::ostream &os,
                                  const SquareMatrix<U> &matrix);

 private:
  std::size_t size_;
  std::unique_ptr<T[]> data_;
};

template <typename T>
std::ostream &operator<<(std::ostream &os, const SquareMatrix<T> &matrix);

template <typename T>
SquareMatrix<T>::SquareMatrix(
    std::initializer_list<std::initializer_list<T>> rows) {
  std::size_t height = rows.size();

  std::size_t width = 0;

  for (auto &row : rows) {
    if (row.size() > width) {
      width = row.size();
    }
  }

  if (height != width) {
    throw std::invalid_argument("Square matrix must be square");
  }

  size_ = height;
  data_ = std::make_unique<int[]>(height * width);

  std::size_t i = 0;

  for (auto &row : rows) {
    for (auto &item : row) {
      data_[i] = item;
      i++;
    }
  }
}

template <typename T>
SquareMatrix<T>::SquareMatrix(std::size_t size)
    : size_(size), data_(std::make_unique<T[]>(size * size)) {}

template <typename T>
T &SquareMatrix<T>::operator()(std::size_t x, std::size_t y) {
  if (x >= size() || y >= size()) {
    throw std::invalid_argument("Index exceeds matrix dimensions");
  }

  return data_[x * size() + y];
}

template <typename T>
const T &SquareMatrix<T>::operator()(std::size_t x, std::size_t y) const {
  if (x >= size() || y >= size()) {
    throw std::invalid_argument("Index exceeds matrix dimensions");
  }

  return data_[x * size() + y];
}

template <typename T>
std::size_t SquareMatrix<T>::size() const {
  return size_;
}

template <typename T>
std::ostream &operator<<(std::ostream &os, const SquareMatrix<T> &matrix) {
  os << "Square matrix [" << matrix.size() << "] {\n";

  for (std::size_t i = 0; i < matrix.size(); i++) {
    os << "\t{ ";

    for (std::size_t j = 0; j < matrix.size(); j++) {
      os << matrix(i, j);

      if (j < matrix.size() - 1) {
        os << ", ";
      }
    }

    os << " }\n";
  }

  os << "}\n";

  return os;
}

template <typename T>
bool SquareMatrix<T>::operator==(const SquareMatrix<T> &matrix) const {
  int i;
  int j;

  if (this->size() != matrix.size()) {
    return false;
  }

  for (j = 0; j < matrix.size(); j++) {
    for (i = 0; i < matrix.size(); i++) {
      if (this->data_[j * size() + i] != matrix(j, i)) {
        return false;
      }
    }
  }

  return true;
}

template <typename T>
bool SquareMatrix<T>::operator!=(const SquareMatrix<T> &matrix) const {
  return !(*this == matrix);
}

}  // namespace s21
