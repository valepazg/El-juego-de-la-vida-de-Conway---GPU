#include <iostream>
#include "GameOfLife/grid.hpp"
#include <gtest/gtest.h>

using namespace std;

/*
Primero se pone el nombre del test asignado y se hereda del tipo de
assertion de googletest
*/
// crea grid de tamaño 1
TEST(GridTest_0, BasicAssertions) {
  Grid grid(1);
  int rows = grid.getRows();
  int cols = grid.getCols();
  EXPECT_EQ(rows,1);
  EXPECT_EQ(cols,1);
}


// crea grid de tamaño 1
TEST(GridTest_1, BasicAssertions) {
  Grid grid(10000);
  int rows = grid.getRows();
  int cols = grid.getCols();
  EXPECT_EQ(rows,10000);
  EXPECT_EQ(cols,10000);
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



TEST(ValidateMatrixStr, BasicAssertions) {
  string matrix_str =  "X,O,O,X,O\nX,X,O,O,X\nO,O,X,X,O";
  vector<vector<bool>> matrix = validate_matrix_str(matrix_str);
  int rows = matrix.size();
  EXPECT_EQ(rows,3);
}

// string to matrix
TEST(GridTest_5, BasicAssertions) {
  string matrix_str =  "X,O,O,X,O\nX,X,O,O,X\nO,O,X,X,O";
  Grid grid(matrix_str);
  int rows = grid.getRows();
  int cols = grid.getCols();
  EXPECT_EQ(rows,3);
  EXPECT_EQ(cols,5);
}


// string to matrix bigger matrix
TEST(GridTest_6, BasicAssertions) {
  string matrix_str =  "X,O,O,X,O,X,O,O,X,O\nX,X,O,O,X,X,O,O,X,O\nO,O,X,X,O,X,O,X,X,O\nO,O,X,X,O,X,X,O,O,O";
  Grid grid(matrix_str);
  int rows = grid.getRows();
  int cols = grid.getCols();
  EXPECT_EQ(rows,4);
  EXPECT_EQ(cols,10);
}

// crea grid debe fallar por simbolos diferentes a X y O
TEST(GridTest_7, TestFail_3) {
EXPECT_THROW(
  string matrix_str =  "X,O,O,X,O,X,O,O,X,O\nX,X,z,Z,X,X,O,O,X,O\nO,O,X,X,O,X,O,X,X,O\nO,O,X,X,O,X,X,O,0,O";
  Grid grid(matrix_str), std::exception
);
}



// crea grid short line debe fallar
TEST(GridTest_8, TestFail_4) {
EXPECT_THROW(
  string matrix_str =  "X,O,O,X,O,X,O,X,O\nX,X,z,Z,X,X,O,O,X,O\nO,O,X,X,X,O,X,X,O\nO,O,X,X,O,X,X,O,0,O";
  Grid grid(matrix_str), std::exception
);
}

namespace fs = std::filesystem;

// leer desde un archivo
TEST(GridReadFromFile, BasicAssertions) {
  fs::path matrix_path("./tests/matrix_ok.txt");
  Grid grid(matrix_path);
  int rows = grid.getRows();
  int cols = grid.getCols();
  EXPECT_EQ(rows,4);
  EXPECT_EQ(cols,10);
}



// leer desde un archivo
TEST(GridReadFromFileFail, TestFail_5) {
  fs::path matrix_path("./tests/matrix_not_ok.txt");
  EXPECT_THROW(Grid grid(matrix_path), std::exception);
}


// check
TEST(GridCheckNext, BassicAssertions){
  string matrix_str = "X,O,O\nX,X,O\nX,O,X";
  string next = "O,O,O\nX,O,O\nO,X,O";
  vector<vector<bool>> matrix_next = validate_matrix_str(next);

  Grid grid(matrix_str);
  Grid grid_next;
  grid_next.load_grid(matrix_next);

  std::cerr << "NOW" << std::endl;
  std::cerr << grid << std::endl;
  std::cerr << "NEXT" << std::endl;
  std::cerr << grid_next << std::endl;

  // size is 3x3
  for (int i=0;i<3;i++) {
	for (int j=0;j<3;j++) {
	  int next_value = grid.check_next(i,j);
	  EXPECT_EQ(next_value, matrix_next[i][j]);
	}
  }
}


// test getvalue
TEST(GridCheckGetValue, BassicAssertions){
  string matrix_str = "X,O,O\nX,X,O\nX,O,X";
  vector<vector<bool>> matrix_next = validate_matrix_str(matrix_str);

  Grid grid(matrix_str);

  // size is 3x3
  for (int i=0;i<3;i++) {
	for (int j=0;j<3;j++) {
	  int next_value = grid.getValue(i,j);
	  EXPECT_EQ(next_value, matrix_next[i][j]);
	}
  }
}



// generate next grid and compare
TEST(GridCheckNextUsingGrid, BassicAssertions){
  string matrix_str = "X,O,O\nX,X,O\nX,O,X";
  string next = "O,O,O\nX,O,O\nO,X,O";
  vector<vector<bool>> matrix_next = validate_matrix_str(next);

  Grid grid(matrix_str);
  Grid grid_next;
  grid_next.load_grid(matrix_next);


  std::cerr << "Creating grid next test" << std::endl;
  
  Grid grid_test(grid);

  std::cerr << "NOW" << std::endl;
  std::cerr << grid << std::endl;
  std::cerr << "NEXT TRUE" << std::endl;
  std::cerr << grid_next << std::endl;
  std::cerr << "NEXT TEST" << std::endl;
  std::cerr << grid_test << std::endl;

  // size is 3x3
  for (int i=0;i<3;i++) {
	for (int j=0;j<3;j++) {
	  int next_value = grid_test.getValue(i,j);
	  EXPECT_EQ(next_value, matrix_next[i][j]);
	}
  }
}


// generate next grid and compare with ==
TEST(GridCheckCompareEq, BassicAssertions){
  string matrix_str = "X,O,O\nX,X,O\nX,O,X";
  string next = "O,O,O\nX,O,O\nO,X,O";
  vector<vector<bool>> matrix_next = validate_matrix_str(next);

  Grid grid(matrix_str);
  Grid grid_next;
  grid_next.load_grid(matrix_next);


  std::cerr << "Creating grid next test" << std::endl;
  
  Grid grid_test(grid);

  std::cerr << "NOW" << std::endl;
  std::cerr << grid << std::endl;
  std::cerr << "NEXT TRUE" << std::endl;
  std::cerr << grid_next << std::endl;
  std::cerr << "NEXT TEST" << std::endl;
  std::cerr << grid_test << std::endl;

  // size is 3x3
  EXPECT_EQ(grid_test, grid_next);
}


