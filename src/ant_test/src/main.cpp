#include <gtest/gtest.h>

#include <cstdio>
#include <iostream>

#include "ant_lib.hpp"

// #include "s21_graph/include/s21_graph.hpp"
// #include "s21_graph_algorithms/include/s21_graph_algorithms.hpp"

// Эта хрень здесь чтобы показать что текут гугл тесты а не проект
// #include <stdexcept>
// TEST(test, tester) {throw std::runtime_error("LOL");}

// GRAPH

TEST(Graph, init) { EXPECT_NO_THROW(s21::Graph gr); }

TEST(Graph, load) {
  s21::Graph gr;
  EXPECT_NO_THROW(
      gr.loadGraphFromFile("src/ant_test/data/chekConnection/file1"));
}

TEST(Graph, edge) {
  s21::Graph gr;
  gr.loadGraphFromFile("src/ant_test/data/chekConnection/file1");
  EXPECT_EQ(gr.edge(2, 0), 11);
}

TEST(Graph, vertexCount) {
  s21::Graph gr;
  gr.loadGraphFromFile("src/ant_test/data/chekConnection/file1");
  EXPECT_EQ(gr.vertexCount(), 4ul);
}

// CHECK CONNECTION

TEST(Connection, full) {
  s21::Graph gr;
  gr.loadGraphFromFile("src/ant_test/data/chekConnection/file1");
  EXPECT_EQ(gr.checkConnectivity(), true);
}

TEST(Connection, minimum) {
  s21::Graph gr;

  gr.loadGraphFromFile("src/ant_test/data/chekConnection/file2");
  EXPECT_EQ(gr.checkConnectivity(), true);
}

TEST(Connection, big) {
  s21::Graph gr;

  gr.loadGraphFromFile("src/ant_test/data/chekConnection/file3");
  EXPECT_EQ(gr.checkConnectivity(), true);
}

TEST(Connection, mini_fail) {
  s21::Graph gr;

  EXPECT_ANY_THROW(
      gr.loadGraphFromFile("src/ant_test/data/chekConnection/file4"));
}

TEST(Connection, zero_fail) {
  s21::Graph gr;

  EXPECT_ANY_THROW(
      gr.loadGraphFromFile("src/ant_test/data/chekConnection/file5"));
}

TEST(Connection, fail) {
  s21::Graph gr;

  EXPECT_ANY_THROW(
      gr.loadGraphFromFile("src/ant_test/data/chekConnection/file6"));
}

TEST(Connection, fail_half) {
  s21::Graph gr;

  EXPECT_ANY_THROW(
      gr.loadGraphFromFile("src/ant_test/data/chekConnection/file7"));
}

TEST(Connection, roll_sucsess) {
  s21::Graph gr;

  gr.loadGraphFromFile("src/ant_test/data/chekConnection/file8");
  EXPECT_EQ(gr.checkConnectivity(), true);
}

// ALGORITMS

// getShortestPathBetweenVertices

TEST(getShortestPathBetweenVertices, full) {
  s21::Graph gr;
  s21::GraphAlgorithms alg;
  int test;

  gr.loadGraphFromFile("src/ant_test/data/GraphAlgorithms/file1");
  test = alg.getShortestPathBetweenVertices(gr, 1, 4);
  EXPECT_EQ(56, test);
}

TEST(getShortestPathBetweenVertices, full_II) {
  s21::Graph gr;
  s21::GraphAlgorithms alg;
  int test;

  gr.loadGraphFromFile("src/ant_test/data/GraphAlgorithms/file1");
  test = alg.getShortestPathBetweenVertices(gr, 3, 2);
  EXPECT_EQ(21, test);
}

TEST(getShortestPathBetweenVertices, mini) {
  s21::Graph gr;
  s21::GraphAlgorithms alg;
  int test;

  gr.loadGraphFromFile("src/ant_test/data/GraphAlgorithms/file2");
  test = alg.getShortestPathBetweenVertices(gr, 1, 2);
  EXPECT_EQ(1, test);
}

TEST(getShortestPathBetweenVertices, mini_II) {
  s21::Graph gr;
  s21::GraphAlgorithms alg;
  int test;

  gr.loadGraphFromFile("src/ant_test/data/GraphAlgorithms/file2");
  test = alg.getShortestPathBetweenVertices(gr, 2, 1);
  EXPECT_EQ(5, test);
}

TEST(getShortestPathBetweenVertices, normal) {
  s21::Graph gr;
  s21::GraphAlgorithms alg;
  int test;

  gr.loadGraphFromFile("src/ant_test/data/GraphAlgorithms/file3");
  test = alg.getShortestPathBetweenVertices(gr, 1, 5);
  EXPECT_EQ(421, test);
}

TEST(getShortestPathBetweenVertices, normal_II) {
  s21::Graph gr;
  s21::GraphAlgorithms alg;
  int test;

  gr.loadGraphFromFile("src/ant_test/data/GraphAlgorithms/file3");
  test = alg.getShortestPathBetweenVertices(gr, 4, 6);
  EXPECT_EQ(90, test);
}

TEST(getShortestPathBetweenVertices, roll) {
  s21::Graph gr;
  s21::GraphAlgorithms alg;
  int test;

  gr.loadGraphFromFile("src/ant_test/data/GraphAlgorithms/file4");
  test = alg.getShortestPathBetweenVertices(gr, 1, 6);
  EXPECT_EQ(20, test);
}

TEST(getShortestPathBetweenVertices, roll_II) {
  s21::Graph gr;
  s21::GraphAlgorithms alg;
  int test;

  gr.loadGraphFromFile("src/ant_test/data/GraphAlgorithms/file4");
  test = alg.getShortestPathBetweenVertices(gr, 5, 4);
  EXPECT_EQ(16, test);
}

// solveTravelingSalesmanProblem

TEST(solveTravelingSalesmanProblem, full) {
  s21::Graph gr;
  s21::GraphAlgorithms alg;
  s21::GraphAlgorithms::TsmResult rezult;

  gr.loadGraphFromFile("src/ant_test/data/TravelingSalesmanProblem/file1");
  rezult = alg.solveTravelingSalesmanProblem(gr);
  EXPECT_EQ(rezult.distance, 118);
}

TEST(solveTravelingSalesmanProblem, mini) {
  s21::Graph gr;
  s21::GraphAlgorithms alg;
  s21::GraphAlgorithms::TsmResult rezult;

  gr.loadGraphFromFile("src/ant_test/data/TravelingSalesmanProblem/file2");
  rezult = alg.solveTravelingSalesmanProblem(gr);
  EXPECT_EQ(rezult.distance, 6);
}

TEST(solveTravelingSalesmanProblem, big) {
  s21::Graph gr;
  s21::GraphAlgorithms alg;
  s21::GraphAlgorithms::TsmResult rezult;

  gr.loadGraphFromFile("src/ant_test/data/TravelingSalesmanProblem/file3");
  rezult = alg.solveTravelingSalesmanProblem(gr);
  EXPECT_EQ(rezult.distance, 123);
}

// solveTravelingSalesmanProblemParallel

TEST(solveTravelingSalesmanProblemParallels, full) {
  s21::Graph gr;
  s21::GraphAlgorithms alg;
  s21::GraphAlgorithms::TsmResult rezult;

  gr.loadGraphFromFile("src/ant_test/data/TravelingSalesmanProblem/file1");
  rezult = alg.solveTravelingSalesmanProblemParallels(gr);
  EXPECT_EQ(rezult.distance, 118);
}

TEST(solveTravelingSalesmanProblemParallels, mini) {
  s21::Graph gr;
  s21::GraphAlgorithms alg;
  s21::GraphAlgorithms::TsmResult rezult;

  gr.loadGraphFromFile("src/ant_test/data/TravelingSalesmanProblem/file2");
  rezult = alg.solveTravelingSalesmanProblemParallels(gr);
  EXPECT_EQ(rezult.distance, 6);
}

TEST(solveTravelingSalesmanProblemParallels, big) {
  s21::Graph gr;
  s21::GraphAlgorithms alg;
  s21::GraphAlgorithms::TsmResult rezult;

  gr.loadGraphFromFile("src/ant_test/data/TravelingSalesmanProblem/file3");
  rezult = alg.solveTravelingSalesmanProblemParallels(gr);
  EXPECT_EQ(rezult.distance, 123);
}

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
