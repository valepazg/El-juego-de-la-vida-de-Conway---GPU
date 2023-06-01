#include <iostream>
#include "GameOfLife/grid.hpp"
#include <gtest/gtest.h>

using namespace std;

/*
Primero se pone el nombre del test asignado y se hereda del tipo de
assertion de googletest
*/
// crea grid de tamaño 1
TEST(GridTest_1, BasicAssertions) {
  Grid grid(1);
  int rows = grid.getRows();
  int cols = grid.getCols();
  EXPECT_EQ(rows,1);
  EXPECT_EQ(cols,1);
}

// crea grid de tamaño default
TEST(GridTest_2, BasicAssertions) {
  Grid grid;
  int rows = grid.getRows();
  int cols = grid.getCols();
  EXPECT_EQ(rows,10);
  EXPECT_EQ(cols,10);
}



// crea grid debe fallar
TEST(GridTest_3, TestFail_1) {
EXPECT_THROW(
  Grid grid(0), std::exception
);
}

// crea grid debe fallar
TEST(GridTest_4, TestFail_2) {
EXPECT_THROW(
  Grid grid(0,0), std::exception
);
}


