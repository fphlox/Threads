#include <gtest/gtest.h>

#include <cstdio>
#include <iostream>

#include "winograd_lib.hpp"

TEST(Winograd, one) {
  s21::Winograd win("src/winograd_test/data/file1",
                    "src/winograd_test/data/file2");
  std::vector<std::vector<long long>> ansvers = {
      {7, -2, 19}, {-15, 3, -18}, {23, -4, 17}};

  std::vector<std::vector<long long>> ret = win.multiplyStart(20000);
  EXPECT_EQ(ret, ansvers);
}

TEST(Winograd, two) {
  s21::Winograd win("src/winograd_test/data/file3",
                    "src/winograd_test/data/file4");
  std::vector<std::vector<long long>> ansvers = {{191, 56, 109, 149},
                                                 {1068, 256, 169, 369},
                                                 {416, -218, 103, 475},
                                                 {366, 280, 250, 305},
                                                 {313, 266, 224, 200}};

  std::vector<std::vector<long long>> ret = win.multiplyStart(20000);
  EXPECT_EQ(ret, ansvers);
}

TEST(Winograd, three) {
  s21::Winograd win("src/winograd_test/data/file5",
                    "src/winograd_test/data/file6");
  std::vector<std::vector<long long>> ansvers = {{227, 56, 37, 101},
                                                 {1098, 256, 109, 329},
                                                 {464, -218, 7, 411},
                                                 {438, 280, 106, 209},
                                                 {367, 266, 116, 128}};

  std::vector<std::vector<long long>> ret = win.multiplyStart(20000);
  EXPECT_EQ(ret, ansvers);
}
//
// TEST(Winograd, big) {
//  s21::Winograd win("src/winograd_test/data/file7",
//                    "src/winograd_test/data/file8");
//
//  EXPECT_NO_THROW (win.multiplyStart(200));
//}

TEST(WinogradConveyor, one) {
  s21::Winograd win("src/winograd_test/data/file1",
                    "src/winograd_test/data/file2");
  std::vector<std::vector<long long>> ansvers = {
      {7, -2, 19}, {-15, 3, -18}, {23, -4, 17}};

  std::vector<std::vector<long long>> ret = win.multiplyConveyorStart(20000);
  EXPECT_EQ(ret, ansvers);
}

TEST(WinogradConveyor, two) {
  s21::Winograd win("src/winograd_test/data/file3",
                    "src/winograd_test/data/file4");
  std::vector<std::vector<long long>> ansvers = {{191, 56, 109, 149},
                                                 {1068, 256, 169, 369},
                                                 {416, -218, 103, 475},
                                                 {366, 280, 250, 305},
                                                 {313, 266, 224, 200}};

  std::vector<std::vector<long long>> ret = win.multiplyConveyorStart(20000);
  EXPECT_EQ(ret, ansvers);
}

TEST(WinogradConveyor, three) {
  s21::Winograd win("src/winograd_test/data/file5",
                    "src/winograd_test/data/file6");
  std::vector<std::vector<long long>> ansvers = {{227, 56, 37, 101},
                                                 {1098, 256, 109, 329},
                                                 {464, -218, 7, 411},
                                                 {438, 280, 106, 209},
                                                 {367, 266, 116, 128}};

  std::vector<std::vector<long long>> ret = win.multiplyConveyorStart(20000);
  EXPECT_EQ(ret, ansvers);
}

// TEST(WinogradConveyor, big) {
//   s21::Winograd win("src/winograd_test/data/file7",
//                     "src/winograd_test/data/file8");
//
//   EXPECT_NO_THROW (win.multiplyConveyorStart(200));
// }

TEST(WinogradParallels, one) {
  s21::Winograd win("src/winograd_test/data/file1",
                    "src/winograd_test/data/file2");
  std::vector<std::vector<long long>> ansvers = {
      {7, -2, 19}, {-15, 3, -18}, {23, -4, 17}};

  std::vector<std::vector<long long>> ret =
      win.multiplyParallelsStart(20000, 20);
  EXPECT_EQ(ret, ansvers);
}

TEST(WinogradParallels, two) {
  s21::Winograd win("src/winograd_test/data/file3",
                    "src/winograd_test/data/file4");
  std::vector<std::vector<long long>> ansvers = {{191, 56, 109, 149},
                                                 {1068, 256, 169, 369},
                                                 {416, -218, 103, 475},
                                                 {366, 280, 250, 305},
                                                 {313, 266, 224, 200}};

  std::vector<std::vector<long long>> ret =
      win.multiplyParallelsStart(20000, 20);
  EXPECT_EQ(ret, ansvers);
}

TEST(WinogradParallels, three) {
  s21::Winograd win("src/winograd_test/data/file5",
                    "src/winograd_test/data/file6");
  std::vector<std::vector<long long>> ansvers = {{227, 56, 37, 101},
                                                 {1098, 256, 109, 329},
                                                 {464, -218, 7, 411},
                                                 {438, 280, 106, 209},
                                                 {367, 266, 116, 128}};

  std::vector<std::vector<long long>> ret =
      win.multiplyParallelsStart(20000, 20);
  EXPECT_EQ(ret, ansvers);
}

// TEST(WinogradParallels, big) {
//   s21::Winograd win("src/winograd_test/data/file7",
//                     "src/winograd_test/data/file8");
//
//   EXPECT_NO_THROW (win.multiplyParallelsStart(200, 30));
// }

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
