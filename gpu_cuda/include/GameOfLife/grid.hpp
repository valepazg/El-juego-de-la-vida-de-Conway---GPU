#include <iostream>
#include <string>
#include <cassert>
#include <cmath>
#include <vector>
#include <filesystem>

using namespace std;

#ifndef CLASS_GRID
#define CLASS_GRID
/*
Grid define: size n,m
1: alive
0: dead

iteración:
1. Célula nace de un espacio muerto si hay 3 vecinos vivos
2. Célula sobrevie si tiene 2 o 3 vecino
*/
class Grid{
private:
  bool* matrix;
  int rows;
  int cols;
public:
  // constructor
  Grid(unsigned xin, unsigned yin);
  // square constructor
  Grid(unsigned vin);
  // default constructor
  Grid();
  // constructor given a string with array
  Grid(string grid);
  // load grid from file
  Grid(std::filesystem::path path);
  // create the next grid
  Grid(Grid &grid);
  // check next
  bool check_next(unsigned xin, unsigned yin);
  // load grid
  void load_grid(vector<vector<bool>> &matrix);
  void gen_random(int seed);

  // load grid from string
  // generate next grid
  vector<vector<bool>> next();
  // show grid
  friend std::ostream& operator<<(std::ostream &os, Grid &grid);

  int getRows();
  int getCols();
  bool getValue(int i, int j);

  bool operator==(const Grid& other) const;

  bool has_life() ;

  int alive() ;
  int dead() ;
  ~Grid();
};

vector<vector<bool>> validate_matrix_str(string grid);
#endif
