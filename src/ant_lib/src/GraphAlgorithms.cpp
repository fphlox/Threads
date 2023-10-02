#include "ant_lib/GraphAlgorithms.hpp"

#include "ant_lib/Queue.hpp"
#include "ant_lib/SquareMatrix.hpp"

static bool findInList(std::vector<int> &spisok, int number) {
  unsigned i;
  unsigned size = spisok.size();

  for (i = 0; i < size; i++) {
    if (spisok[i] == number) return (true);
  }
  return (false);
}

int s21::GraphAlgorithms::getShortestPathBetweenVertices(s21::Graph &graph,
                                                         int vertex1,
                                                         int vertex2) {
  std::vector<t_deikstra> vertexes;
  s21::Queue<unsigned> need_to_visite;
  t_deikstra we_here;
  auto matrix_size = graph.vertexCount();

  if (((vertex1) <= 0) || (((std::size_t)vertex1) > matrix_size))
    throw std::runtime_error(
        "Ошибка! Стартовая вершина графа имеет отрицательный, нулевой или "
        "больший максимального номер!");
  if (((vertex2) <= 0) || (((std::size_t)vertex2) > matrix_size))
    throw std::runtime_error(
        "Ошибка! Искомая вершина графа имеет отрицательный, нулевой или "
        "больший максимального номер!");

  for (std::size_t i = 0; i < matrix_size; i++) {
    vertexes.push_back(we_here);
    vertexes[i].number = i + 1;
  }
  vertexes[vertex1 - 1].mark = 0;
  need_to_visite.push(vertex1);
  while (need_to_visite.size() != 0) {
    if (vertexes[need_to_visite.front() - 1].visited == false) {
      we_here = vertexes[need_to_visite.front() - 1];
      need_to_visite.pop();
      for (std::size_t i = 0; i < matrix_size; i++) {
        if ((graph.edge(we_here.number - 1, i) != 0) &&
            ((we_here.number - 1) != i)) {
          if (vertexes[i].visited == false) need_to_visite.push(i + 1);
          if ((we_here.mark + graph.edge(we_here.number - 1, i)) <
              vertexes[i].mark)
            vertexes[i].mark =
                (we_here.mark + graph.edge(we_here.number - 1, i));
        }
      }
      vertexes[we_here.number - 1].visited = true;
    } else
      need_to_visite.pop();
  }
  return (vertexes[vertex2 - 1].mark);
}

unsigned wantVertex(s21::Graph &graph,
                    const s21::SquareMatrix<double> &fero_matrix,
                    s21::GraphAlgorithms::t_ant ant) {
  unsigned i;
  int j;
  double probability;
  double waysSum;
  std::vector<double> ways(graph.vertexCount(), 0);
  std::vector<double> want(graph.vertexCount(), 0);
  unsigned from = ant.way.back() - 1;

  waysSum = 0;
  // Подсчет привлекательности путей
  for (i = 0; i < graph.vertexCount(); i++) {
    if ((graph.edge(from, i) != 0) && (from != i) &&
        (findInList(ant.way, i + 1) == false))
      ways[i] = fero_matrix(from, i) / graph.edge(from, i);
    waysSum = waysSum + ways[i];
  }
  // Посчет вероятности выбора пути, в зависимости от привлекательности
  if (waysSum == 0) return (0);
  for (i = 0; i < graph.vertexCount(); i++) {
    want[i] = ways[i] / waysSum;
    if ((i != 0) && (want[i] != 0)) {
      j = i - 1;
      while (j >= 0) {
        if (want[j] != 0) {
          want[i] += want[j];
          break;
        }
        j--;
      }
    }
  }
  // Проброс вероятности и выбор пути
  probability = (double)(rand() % 1000 + 1) / 1000;
  for (i = 0; i < graph.vertexCount(); i++) {
    if ((want[i] != 0) && (probability <= want[i])) return (i + 1);
  }

  return (0);
}

//Муравьи обходят граф
void s21::GraphAlgorithms::antMove(t_ant &ant,
                                   s21::SquareMatrix<double> &fero_matrix,
                                   s21::Graph &graph, std::size_t &dead_end) {
  unsigned vertex;

  while ((ant.way.size() < graph.vertexCount()) && (ant.ant_can_go == true)) {
    if ((vertex = wantVertex(graph, fero_matrix, ant)) != 0) {
      ant.distance += graph.edge(ant.way.back() - 1, vertex - 1);
      ant.way.push_back(vertex);
    } else {
      ant.ant_can_go = false;
      dead_end++;
    }
  }
  if (ant.ant_can_go == true)
    ant.way_back =
        getShortestPathBetweenVertices(graph, ant.way.back(), ant.way.front());
}

void s21::GraphAlgorithms::deadEnd(std::size_t dead_end, s21::Graph &graph) {
  if (dead_end == graph.vertexCount())
    throw std::runtime_error(
        "Ошибка! Все муравьи зашли в тупик! Муравьиный алгоритм не может "
        "обработать предоставленный граф!");
}

//распыление феромон в зависимости от длины отрезков пути
void s21::GraphAlgorithms::sprayingFeromons(
    t_ant &ant, s21::Graph &graph, s21::SquareMatrix<double> &fero_matrix) {
  if (ant.ant_can_go == true) {
    for (std::size_t i = 0; i < graph.vertexCount() - 1; i++) {
      fero_matrix(ant.way[i] - 1, ant.way[i + 1] - 1) +=
          ant.feromon * ((ant.distance / graph.vertexCount()) /
                         graph.edge(ant.way[i] - 1, ant.way[i + 1] - 1));
    }
  }
}

//получение минимального пути
void s21::GraphAlgorithms::minWay(t_ant &ant, TsmResult &rezult,
                                  bool &need_retry) {
  if ((ant.distance + ant.way_back < rezult.distance) &&
      (ant.ant_can_go == true)) {
    rezult.distance = ant.distance + ant.way_back;
    rezult.vertices = ant.way;
    need_retry = true;
  }
}

//обнуление муравьев перед следующей итерацией
void s21::GraphAlgorithms::antZeroing(t_ant &ant, int number) {
  ant.ant_can_go = true;
  ant.feromon = 0;
  ant.way_back = 0;
  ant.distance = 0;
  ant.way.clear();
  ant.way.push_back(number + 1);
}

s21::GraphAlgorithms::TsmResult
s21::GraphAlgorithms::solveTravelingSalesmanProblem(s21::Graph &graph) {
  std::vector<t_ant> colony(graph.vertexCount());
  s21::SquareMatrix<double> fero_matrix(graph.vertexCount());
  TsmResult rezult;
  // double feromon = 5;
  int retry = 100;

  rezult.distance = std::numeric_limits<double>::infinity();
  for (std::size_t j = 0; j != graph.vertexCount(); j++) {
    for (std::size_t i = 0; i != graph.vertexCount(); i++)
      fero_matrix(j, i) = 1;
  }

  for (std::size_t i = 0; i < colony.size(); i++)
    colony[i].way.push_back(i + 1);

  while (retry > 0) {
    bool need_retry = false;
    std::size_t dead_end = 0;

    for (std::size_t i = 0; i < graph.vertexCount(); i++) {
      antMove(std::ref(colony[i]), std::ref(fero_matrix), std::ref(graph),
              std::ref(dead_end));
      deadEnd(dead_end, std::ref(graph));
      sprayingFeromons(std::ref(colony[i]), std::ref(graph),
                       std::ref(fero_matrix));
      minWay(std::ref(colony[i]), std::ref(rezult), std::ref(need_retry));
      antZeroing(std::ref(colony[i]), i);
    }
    if (need_retry == true) retry = 100;
    retry--;
  }
  return rezult;
}

// Parallels

void s21::GraphAlgorithms::antDo(t_ant &ant, s21::Graph &graph,
                                 s21::SquareMatrix<double> &fero_matrix,
                                 std::size_t &dead_end, std::mutex &mtx,
                                 bool &need_retry, TsmResult &rezult,
                                 int &retry) {
  antMove(std::ref(ant), std::ref(fero_matrix), std::ref(graph),
          std::ref(dead_end));
  deadEnd(dead_end, std::ref(graph));
  sprayingFeromons(std::ref(ant), std::ref(graph), std::ref(fero_matrix));
  mtx.lock();
  minWay(std::ref(ant), std::ref(rezult), std::ref(need_retry));
  mtx.unlock();
  antZeroing(std::ref(ant), ant.way[0] - 1);
  if (ant.way[0] == 1) {
    if (need_retry == true) retry = 100;
    retry--;
  }
}

s21::GraphAlgorithms::TsmResult
s21::GraphAlgorithms::solveTravelingSalesmanProblemParallels(
    s21::Graph &graph) {
  std::vector<t_ant> colony(graph.vertexCount());
  s21::SquareMatrix<double> fero_matrix(graph.vertexCount());
  TsmResult rezult;
  std::mutex mtx;

  rezult.distance = std::numeric_limits<double>::infinity();
  int retry = 100;
  for (std::size_t j = 0; j != graph.vertexCount(); j++) {
    for (std::size_t i = 0; i != graph.vertexCount(); i++)
      fero_matrix(j, i) = 1;
  }

  unsigned threads_num = 40;
  if (graph.vertexCount() < threads_num) threads_num = graph.vertexCount();
  std::vector<std::thread> threads(threads_num);
  // Каждый муравей начинает со своей вершины графа
  for (std::size_t i = 0; i < colony.size(); i++) {
    colony[i].way.push_back(i + 1);
  }

  while (retry > 0) {
    bool need_retry = false;
    std::size_t alg_can_work = 0;

    std::size_t i;
    for (i = 0; i < threads_num; i++) {
      threads[i] = std::thread(antDo, std::ref(colony[i]), std::ref(graph),
                               std::ref(fero_matrix), std::ref(alg_can_work),
                               std::ref(mtx), std::ref(need_retry),
                               std::ref(rezult), std::ref(retry));
    }
    while (i < graph.vertexCount()) {
      for (auto &thread : threads) {
        if (thread.joinable()) {
          thread.join();
          thread = std::thread(antDo, std::ref(colony[i]), std::ref(graph),
                               std::ref(fero_matrix), std::ref(alg_can_work),
                               std::ref(mtx), std::ref(need_retry),
                               std::ref(rezult), std::ref(retry));
          i++;
          break;
        }
      }
    }
    for (auto &thread : threads) thread.join();
  }

  return rezult;
}

std::vector<time_t> s21::GraphAlgorithms::сomparisonMethods(s21::Graph &graph,
                                                            int iter) {
  time_t timer;
  time_t timer_after;
  std::vector<time_t> rezult;
  int i;

  if (iter <= 0)
    throw std::runtime_error(
        "Ошибка! Введено отрицательное или нулевое число итераций алгоритмов");

  time(&timer);
  for (i = 0; i < iter; i++) solveTravelingSalesmanProblem(graph);
  time(&timer_after);
  rezult.push_back(difftime(timer_after, timer));

  time(&timer);
  for (i = 0; i < iter; i++) solveTravelingSalesmanProblemParallels(graph);
  time(&timer_after);
  rezult.push_back(difftime(timer_after, timer));

  std::cout << rezult[0] << "   Parallels   " << rezult[1] << std::endl;
  return (rezult);
}
