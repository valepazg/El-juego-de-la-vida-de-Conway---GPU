#include "CudaGameoflife/grid.hpp"

Grid::Grid(){
  this->matrix = new bool[10];
}
Grid::~Grid(){
  delete[] matrix;
}
