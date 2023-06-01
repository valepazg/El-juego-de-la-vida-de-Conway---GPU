#include <iostream>
#include <string>
#include <cassert>
#include <cmath>
#include <fstream>
#include <sstream> //std::stringstream
#include "GameOfLife/grid.hpp"


using namespace std;



vector<vector<bool>> validate_matrix_str(string grid){
   vector<vector<bool>> matrix;

   std::string line;
   std::string symbol;

   std::string sym_x = "X";
   std::string sym_o = "O";
  
   vector<int> counter;
   vector<vector<string>> matrix_str;
   int row=0;
   int column=0;
   std::istringstream s_grid(grid);
   int count_chars = 0;
   while (std::getline(s_grid, line, '\n')){
	 vector<string> newline;
	 int count_chars = 0;
	 std::istringstream s_line(line);
	 while (std::getline(s_line, symbol, ',')) {
	   if (symbol==sym_x || symbol==sym_o) {
		 count_chars++;
		 newline.push_back(symbol);
	   } else {
		  throw std::invalid_argument("Symbol must be 'X' for alive or 'O' for dead separated by comma ','");
	   }
	   column++;
	 }
	 matrix_str.push_back(newline);
	 if (counter.back() == count_chars){ 
	   counter.push_back(count_chars);
	 } else {
	   throw std::invalid_argument("Line must be same number of symbols");
	 }

	 row++;
	 column=0;
	}
	/**/

   int columns = counter.back();;

   for (int i=0;i<row;i++) {
	 for (int j=0;j<column;j++) {
	   matrix[i][j] = matrix_str[i][j]==sym_x;
	 }
   }

   return matrix;
}
 
Grid::Grid(unsigned xin, unsigned yin){
  if (xin>0 && yin >0){
  this->matrix.resize(xin, vector<bool>(yin, false));}
  else {
	throw std::invalid_argument("Size of grid, must be rows>0 and cols>0");
  }
};


Grid::Grid(unsigned vin){
  if(vin>0){
  this->matrix.resize(vin, vector<bool>(vin, false));}
  else {
	throw std::invalid_argument("Size of grid, must be vin>0");
  }
};

Grid::Grid(){
  unsigned N = 10;
  this->matrix.resize(N, vector<bool>(N, false));
};


Grid::Grid(string grid){
   vector<vector<bool>> matrix;
   matrix = validate_matrix_str(grid);
   load_grid(matrix);
};


bool Grid::check_next(unsigned xin, unsigned yin){
  int rows = getRows();
  int cols = getCols();
  if (xin<rows && yin<cols) {
	  unsigned check = 0;
	  unsigned up = xin - 1;
	  unsigned down = xin + 1;
	  unsigned left = yin - 1;
	  unsigned right = yin + 1;
	  bool cell =  this->matrix[xin][yin];
	  if (up>=0){
		if (this->matrix[xin][up]) {
		  check++;
		}
	  }
	  if (down < rows) {
		if (this->matrix[xin][down]) {
		  check++;
		}
	  }

	  if (left>=0){
		if (this->matrix[xin][up]) {
		  check++;
		}
	  }
	  if (right < cols) {
		if (this->matrix[xin][down]) {
		  check++;
		}
	  }
	  
	  if (cell==false && check==3) {
		return true;
	  } else if (cell==true && (check==2 || check==3)) {
		return true;
	  } else {
		return false;
	  }
  } else {
	throw std::invalid_argument("Índices fuera de rango");
  }
}


/*
show the matrix grid
*/


std::ostream& operator<<(
						std::ostream& os, 
						const Grid& grid) {
  
  for (const auto& row: grid.matrix) {
	for (const auto& value:row) {
	  os << value << " ";
	}
	os << std::endl;
  }

 return os;
}



int Grid::getRows() const{
  return this->matrix.size();
};


int Grid::getCols() const{
  if (this->matrix.size()>0) {
	return this->matrix[0].size();
  }
  return 0;
};


void Grid::load_grid(vector<vector<bool>> &matrix) {
  this->matrix=matrix;
}
