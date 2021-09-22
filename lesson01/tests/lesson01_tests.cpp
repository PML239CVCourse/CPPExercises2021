#include <gtest/gtest.h>

#include "../src/some_math.h"

// Скопируйте из папки cmake-build-debug/bin/ файлы
// - gtest_maind.dll
// - gtestd.dll
// в папку: cmake-build-debug/lesson01


TEST(lesson01, fib0) {
    GTEST_ASSERT_EQ(fibbonachiRecursive(0), 0);
}

TEST(lesson01, fib1) {
    GTEST_ASSERT_EQ(fibbonachiRecursive(1), 1);
}

TEST(lesson01, fib2) {
    GTEST_ASSERT_EQ(fibbonachiRecursive(2), 1);
}

TEST(lesson01, fib3) {
    GTEST_ASSERT_EQ(fibbonachiRecursive(3), 2);
}

TEST(lesson01, fib4) {
    GTEST_ASSERT_EQ(fibbonachiRecursive(4), 3);
}

TEST(lesson01, fib10) {
    GTEST_ASSERT_EQ(fibbonachiRecursive(10), 55);
}

TEST(lesson01, fib100) {
//    fibbonachiRecursive(100);
//    fibbonachiFast(100);
}

TEST(lesson01, findX1) {
    double x = solveLinearAXB(2.0, -4.0);
    GTEST_ASSERT_EQ(x, 2.0);
}

TEST(lesson01, findX2) {
    double x = solveLinearAXB(2.0, 4.0);
    GTEST_ASSERT_EQ(x, -2.0);
}

TEST(lesson01, findX3) {
    double x = solveLinearAXB(0.0, 4.0);
    GTEST_ASSERT_EQ(x, std::numeric_limits<double>::max());
}

TEST(lesson01, findX4) {
    double x = solveLinearAXB(0.0, 0.0);
    GTEST_ASSERT_EQ(x, -std::numeric_limits<double>::max());
}

TEST(lesson01, findX5) {
    double x = solveLinearAXB(2.5, -5.0);
    GTEST_ASSERT_EQ(x, 2.0);
}

TEST(lesson01, findX6) {
    double x = solveLinearAXB(4.0, 6.0);
    GTEST_ASSERT_EQ(x, -1.5);
}

TEST(lesson01, solveSquare1) {
    // 0*x^2 + 4*x - 6 = 0
    std::vector<double> xs = solveSquare(0.0, 4.0, -6.0);
    GTEST_ASSERT_EQ(xs.size(), 1); // сверяем что найден ровно один корень
    double x0 = xs[0];
    GTEST_ASSERT_EQ(x0, 1.5); // сверяем что найденный корень правильный
}

