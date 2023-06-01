#include <iostream>
#include <string>
#include <cassert>
#include <cmath>

using namespace std;

#idndef CLASS_GRID
#define CLASS_GRID
/*
Grid define: size n,m
1: alive
0: dead

iteración:
1. Célula nace de un espacio muerto si hay 3 vecinos vivos
2. Célula sobrevie si tiene 2 o 3 vecino
*/
template <class T>
class Grid{
private:
  std::pair<T,T> pair;
  std::vector<std::vector<boolean>> matrix;
public:
  // constructor
  Grid(T xin, T yin);
  // square constructor
  Grid(T in);
  // default constructor
  Grid();
  // check next
  bool check_next(T xin, T yin);
  // load grid
  Grid load_grid(std::vector<std::vector<boolean>>  matrix);
  // load grid from string
  Grid load_grid(string grid);
  // load grid from file
  Grid load_grid(std::filesystem::path path);
  // generate next grid
  Grid next();
  // show grid
  friend std::ostream& operator<<(std::ostream& os, const Grid<T> grid)>>;
}
#endif
