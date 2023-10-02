#include <gtest/gtest.h>

#include <cstdio>
#include <iostream>

#include "gauss_lib.hpp"

// Эта хрень здесь чтобы показать что текут гугл тесты а не проект
// #include <stdexcept>
// TEST(test, tester) {throw std::runtime_error("LOL");}

TEST(Gauss, normal) {
  s21::Gauss gau("src/gauss_test/data/file1");
  std::vector<double> ansvers = {};

  std::vector<double> ret = gau.solutionSale(10);
  EXPECT_EQ(ret[0], 4.25);
  EXPECT_EQ(ret[1], 2.5);
  EXPECT_EQ(ret[2], -7);
}

TEST(Gauss, big) {
  s21::Gauss gau("src/gauss_test/data/file2");
  std::vector<double> ansvers = {};

  std::vector<double> ret = gau.solutionSale(10);
  EXPECT_LT(ret[0] - -0.860461, 0.000001);
  EXPECT_LT(ret[1] - 0.961202, 0.000001);
  EXPECT_LT(ret[2] - -0.707961, 0.000001);
  EXPECT_LT(ret[3] - 0.0934686, 0.000001);
  EXPECT_LT(ret[4] - 0.609078, 0.000001);
  EXPECT_LT(ret[5] - 0.546683, 0.000001);
  EXPECT_LT(ret[6] - -0.20096, 0.000001);
  EXPECT_LT(ret[7] - 0.321913, 0.000001);
  EXPECT_LT(ret[8] - 0.178919, 0.000001);
  EXPECT_LT(ret[9] - 0.643499, 0.000001);
}

TEST(GaussParallels, normal) {
  s21::Gauss gau("src/gauss_test/data/file1");
  std::vector<double> ansvers = {};

  std::vector<double> ret = gau.solutionSaleParallels(10);
  EXPECT_EQ(ret[0], 4.25);
  EXPECT_EQ(ret[1], 2.5);
  EXPECT_EQ(ret[2], -7);
}

TEST(GaussParallels, big) {
  s21::Gauss gau("src/gauss_test/data/file2");
  std::vector<double> ansvers = {};

  std::vector<double> ret = gau.solutionSaleParallels(10);
  EXPECT_LT(ret[0] - -0.860461, 0.000001);
  EXPECT_LT(ret[1] - 0.961202, 0.000001);
  EXPECT_LT(ret[2] - -0.707961, 0.000001);
  EXPECT_LT(ret[3] - 0.0934686, 0.000001);
  EXPECT_LT(ret[4] - 0.609078, 0.000001);
  EXPECT_LT(ret[5] - 0.546683, 0.000001);
  EXPECT_LT(ret[6] - -0.20096, 0.000001);
  EXPECT_LT(ret[7] - 0.321913, 0.000001);
  EXPECT_LT(ret[8] - 0.178919, 0.000001);
  EXPECT_LT(ret[9] - 0.643499, 0.000001);
}

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
