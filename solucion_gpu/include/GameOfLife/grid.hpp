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
  vector<vector<bool>> matrix;
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

  // check next
  bool check_next(unsigned xin, unsigned yin);
  // load grid
  void load_grid(vector<vector<bool>> &matrix);
  // load grid from string
  // generate next grid
  vector<vector<bool>> next();
  // show grid
  friend std::ostream& operator<<(std::ostream &os, Grid const &grid);

  int getRows() const;
  int getCols() const;

};
#endif
