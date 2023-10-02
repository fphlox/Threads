#pragma once

#include <vector>

namespace s21 {

template <class T, class Vector = std::vector<T>>
class Queue {
 protected:
  Vector container;

 public:
  explicit Queue(const Vector &cont = Vector()) : container(cont) {}

  void push(const T value) { container.push_back(value); }

  T pop() {
    T value = container.front();
    container.erase(container.begin());
    return (value);
  }

  T front() { return (container.back()); }

  T back() { return (container.front()); }

  unsigned int size() { return (container.size()); }
};

}  // namespace s21
