#include <iostream>
#include <string>
#include <cassert>
#include <cmath>
#include <fstream>
#include <sstream> //std::stringstream
#include "GameOfLife/grid.hpp"
#include <random>
#include <functional>

using namespace std;
namespace fs = std::filesystem;


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

	 //std::cerr << line << std::endl;

	 vector<string> newline;
	 int count_chars = 0;
	 std::istringstream s_line(line);
	 while (std::getline(s_line, symbol, ',')) {
	   //std::cerr << symbol << std::endl;

	   if (symbol==sym_x || symbol==sym_o) {
		 count_chars++;
		 newline.push_back(symbol);
	   } else {
		  throw std::invalid_argument("Symbol must be 'X' for alive or 'O' for dead separated by comma ','");
	   }
	   column++;
	 }
 	 //std::cerr << "newline ok" << std::endl;

	 matrix_str.push_back(newline);

	 //std::cerr << "matrix pushback" << std::endl;
	 if (counter.size() == 0) {
	   //std::cerr << "count-chars" << count_chars << std::endl;
	   counter.push_back((int)count_chars);
	 }
	 else if (counter.back() == count_chars){ 
	   //std::cerr << "count-chars" << count_chars << std::endl;
 
	   counter.push_back((int)count_chars);
	 } else {
	   throw std::invalid_argument("Line must be same number of symbols");
	 }

	 row++;
	 column=0;
	}
	/**/
   
   int columns = counter.back();
   matrix.resize(row, vector<bool>(columns, false));
   
   for (int i=0;i<row;i++) {
	 for (int j=0;j<columns;j++) {
	   int result =  matrix_str[i][j]==sym_x;
	   matrix[i][j] = result;
	 }
   }

   return matrix;
}
 

Grid::~Grid(){

  // for (int i=0;i<rows*cols; i++) {
  // 	std::cerr << matrix[i] << std::endl;

  // 	if (matrix[i]) delete &matrix[i];
  // }


  delete[] matrix;
}

Grid::Grid(unsigned xin, unsigned yin){
  if (xin>0 && yin >0){
	this->rows = xin;
	this->cols = yin;

	this->matrix =  new bool[rows*cols];
	std::fill_n(matrix, rows * cols, false);
  }
  else {
	throw std::invalid_argument("Size of grid, must be rows>0 and cols>0");
  }
};


Grid::Grid(unsigned vin){
  if(vin>0){
	this->rows = vin;
	this->cols = vin;
	this->matrix =  new bool[rows*cols];
	std::fill_n(matrix, rows * cols, false);
	}
  else {
	throw std::invalid_argument("Size of grid, must be vin>0");
  }
};

Grid::Grid(){
	this->rows = 10;
	this->cols = 10;
	this->matrix =  new bool[rows*cols];
	std::fill_n(matrix, rows * cols, false);

};


Grid::Grid(string grid){
   vector<vector<bool>> matrix;
   matrix = validate_matrix_str(grid);
   load_grid(matrix);
};


Grid::Grid(Grid &old_grid){

  this->rows = old_grid.getRows();
  this->cols = old_grid.getCols();

  this->matrix =  new bool[rows*cols];

  std::fill_n(matrix, rows * cols, false);
 
   for (int i = 0; i < rows; i++)  {
	 for ( int j = 0; j < cols; j++) {
	   this->matrix[i * cols + j] = old_grid.check_next(i,j);
	 }
   }
};


Grid::Grid(std::filesystem::path path) {
  if (std::filesystem::is_regular_file(path)){
	  std::ifstream file(path, std::ios::in | std::ios::binary);
	  if (!file.is_open()){
			throw std::invalid_argument("File cannot be opened");
		  };
	  // Read contents
	  std::string grid{
		std::istreambuf_iterator<char>(file), 
		std::istreambuf_iterator<char>()
	  };
	  // Close the file
	  file.close();
	  vector<vector<bool>> matrix;
	  matrix = validate_matrix_str(grid);
	  load_grid(matrix);
  } else {
	throw std::invalid_argument("File path must be a filename path and be readabale");
  }

};


bool Grid::check_next(unsigned xin, unsigned yin){
  if (xin<rows && yin<cols) {
	  unsigned check = 0;
	  unsigned up = 0;
	  up = rows-1;
	  if (xin > 0){
		up = xin - 1;
	  } 
	 

	  unsigned down = xin + 1;
	  if (down==rows) {
		down=0;
	  }

	  unsigned left = 0;
	  left = cols -1;
	  if (yin>0){
		left = yin - 1;
	  } 

	  unsigned right = yin + 1;
	  if (right==cols) {
		right=0;
	  }

	  bool cell =  getValue(xin,yin);
	  int up_val = getValue(up,yin);
	  int down_val = getValue(down,yin);
	  int left_val = getValue(xin,left);
	  int right_val = getValue(xin,right);
	  int up_east_val = getValue(up, right);
	  int up_west_val = getValue(up, left);
	  int down_east_val = getValue(down, right);
	  int down_west_val = getValue(down, left);

	  check = up_val + down_val + left_val + right_val+up_east_val+up_west_val+down_east_val+down_west_val;
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
						Grid& grid) {


   for (int i = 0; i < grid.getRows(); i++)  {
	 for ( int j = 0; j < grid.getCols(); j++) {
	   int value = grid.getValue(i,j);
	   string symbol = value?"X":"O";
	   os << symbol << " ";
	 }
	 os<<endl;
   } 

 return os;
}



int Grid::getRows(){
  return this->rows;
};


int Grid::getCols(){
  return this->cols;
};


bool Grid::getValue(int i, int j){
  if (i<getRows() && j<getCols()) {
	return this->matrix[i*cols+j];
  } else {
	throw std::invalid_argument("Indexes must be low than number of rows and cols");
  }
};



void Grid::load_grid(vector<vector<bool>> &matrix) {
    this->rows = matrix.size();
	this->cols = matrix[0].size();
	this->matrix =  new bool[rows*cols];

	for (int i=0;i<rows; i++) {
	for (int j=0;j<cols; j++) {
	  this->matrix[i*cols+j]=matrix[i][j];
	}
	}
}



bool Grid::operator==(const Grid& other) const{
	if (rows != other.rows || cols != other.cols){
	  return false;
	}

	for (int i=0;i<rows*cols; i++) {
	  int check = other.matrix[i];
	  if(matrix[i]!=check) {
		return false;
	  }
	}
	return true;
  }


void Grid::gen_random(int seed){
  srand(seed);
  auto gen = std::bind(std::uniform_int_distribution<>(0,1),std::default_random_engine());

	for (int i=0;i<getRows(); i++) {
	for (int j=0;j<getCols(); j++) {
	  bool random = gen();
	  this->matrix[i*cols + j] = random;
	  }
	}

}


bool Grid::has_life(){
	for (int i=0;i<getRows(); i++) {
	for (int j=0;j<getCols(); j++) {
	  bool val = getValue(i,j);
	  if (val) {
		return true;
	  }
	  }
	}
	return false;
}


int Grid::alive() {
  int alive = 0;
	for (int i=0;i<getRows(); i++) {
	for (int j=0;j<getCols(); j++) {
	  bool val = getValue(i,j);
	  if (val) {
		alive++;
	  }
	  }
	}
	return alive;
}

int Grid::dead(){
  int dead = 0;
	for (int i=0;i<getRows(); i++) {
	for (int j=0;j<getCols(); j++) {
	  bool val = getValue(i,j);
	  if (!val) {
		dead++;
	  }
	  }
	}
	return dead;
}
