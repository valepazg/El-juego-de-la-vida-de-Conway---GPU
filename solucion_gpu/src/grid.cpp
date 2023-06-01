#include <iostream>
#include <string>
#include <cassert>
#include <cmath>
#include <fstream>
#include <sstream> //std::stringstream
#include "GameOfLife/grid.hpp"


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


Grid::Grid(Grid &grid){
  int rows = grid.getRows();
  int cols = grid.getCols();

   std::cerr << "Creating grid next" << rows<< " "<<cols<<std::endl;
   this->matrix.resize(rows, vector<bool>(cols, false));
   for (int i = 0; i < rows; i++)  {
	 for ( int j = 0; j < cols; j++) {
	   this->matrix[i][j] = grid.check_next(i,j);
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
  int rows = getRows();
  int cols = getCols();

  if (xin<rows && yin<cols) {
	  unsigned check = 0;
	  unsigned up = 0;
	  bool upflag = false;
	  //std::cerr << "================" << std::endl;

	  //std::cerr << "xin " << xin << std::endl;

	  if (xin > 0){
		upflag = true;
		up = xin - 1;
    	//std::cerr << "up " << up << std::endl;

	  }
      //std::cerr << "up " << up << std::endl;
	 
	  unsigned down = xin + 1;
	  

	  unsigned left = 0;
	  bool leftflag = false;

	  if (yin>0){
		leftflag=true;
		left = yin - 1;
	  }

	  unsigned right = yin + 1;

	  bool cell =  this->matrix[xin][yin];

	  // std::cerr << "X ->" <<xin << " :Y ->"<< yin << std::endl;
	  // std::cerr << "up " << up << std::endl;
	  // std::cerr << "down " << down << std::endl;
	  // std::cerr << "left " << left << std::endl;
	  // std::cerr << "right " << right << std::endl;
	  

	  if (up>=0 &&upflag){
		if (this->matrix[up][yin]) {
		  //std::cerr << "up" << std::endl;

		  check++;
		}
	  }
	  if (down < rows) {
		if (this->matrix[down][yin]) {
		  //std::cerr << "down" << std::endl;
		  check++;
		}
	  }

	  if (left>=0 && leftflag){
		if (this->matrix[xin][left]) {
		  //std::cerr << "left" << std::endl;
		  check++;
		}
	  }
	  if (right < cols) {
		if (this->matrix[xin][right]) {
		  //std::cerr << "right" << std::endl;
		  check++;
		}
	  }
	  
	  // std::cerr << "X ->" <<xin << " :Y ->"<< yin << std::endl;
	  // std::cerr << "cell ->" <<cell<< std::endl;
	  // std::cerr << "check ->" <<check<< std::endl;

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
	  string symbol = value?"X":"O";
	  os << symbol << " ";
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


bool Grid::getValue(int i, int j){
  if (i<getRows() && j<getCols()) {
	return this->matrix[i][j];
  } else {
	throw std::invalid_argument("Indexes must be low than number of rows and cols");
  }
};



void Grid::load_grid(vector<vector<bool>> &matrix) {
  this->matrix=matrix;
}




bool Grid::operator==(const Grid& other) const {
	if (getRows() != other.getRows() || getCols() != other.getCols()){
	  return false;
	}

	for (int i=0;i<getRows(); i++) {
	for (int j=0;j<getCols(); j++) {
	  int check = other.matrix[i][j];
	  if(matrix[i][j]!=check) {
		return false;
	  }
	}
	}
	return true;
  }
