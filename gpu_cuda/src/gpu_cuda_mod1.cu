#include <cuda_runtime.h>
#include <iostream>
#include <chrono>
#include <stdlib.h>
#include <cstdlib>
#include <time.h>
#include <cmath>
#include <fstream>
#include <string>
#include "../include/GameOfLife/grid.hpp"
#include <random>
#include <functional>
#include <sstream> //std::stringstream


using namespace std;
namespace fs = std::filesystem;

class Malla {
private:
  bool* matrix;
  int rows;
  int cols;

public:
  // default constructor
  Malla(){
	this->rows = 10;
	this->cols = 10;
	this->matrix =  new bool[rows*cols];
	std::fill_n(matrix, rows * cols, false);
  };


  Malla(unsigned xin, unsigned yin){
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

  // square constructor
  Malla(unsigned vin){
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

  // constructor given a string with array
  Malla(string grid){
   vector<vector<bool>> matrix;
   matrix = validate_matrix_str(grid);
   load_grid(matrix); 
  };
  // load grid from file
  Malla(std::filesystem::path path){
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

  Malla(Malla &old_grid){
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
  // check next
  bool check_next(unsigned xin, unsigned yin){
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
  };
  // load grid
  void load_grid(vector<vector<bool>> &matrix){
    this->rows = matrix.size();
	this->cols = matrix[0].size();
	this->matrix =  new bool[rows*cols];

	for (int i=0;i<rows; i++) {
	for (int j=0;j<cols; j++) {
	  this->matrix[i*cols+j]=matrix[i][j];
	}
	}
  };

  void gen_random(int seed){
	  srand(seed);
	  auto gen = std::bind(std::uniform_int_distribution<>(0,1),std::default_random_engine());

		for (int i=0;i<getRows(); i++) {
		for (int j=0;j<getCols(); j++) {
		  bool random = gen();
		  this->matrix[i*cols + j] = random;
		  }
		}
  };

  // load grid from string
  // generate next grid
  // show grid
  friend std::ostream& operator<<(std::ostream &os, Malla &grid){
   for (int i = 0; i < grid.getRows(); i++)  {
	 for ( int j = 0; j < grid.getCols(); j++) {
	   int value = grid.getValue(i,j);
	   string symbol = value?"X":"O";
	   os << symbol << " ";
	 }
	 os<<endl;
   } 

 return os;

  };

  int getRows(){
	  return this->rows;
  };

  int getCols(){
	  return this->cols;
  };

  bool getValue(int i, int j){
  if (i<getRows() && j<getCols()) {
	return this->matrix[i*cols+j];
  } else {
	throw std::invalid_argument("Indexes must be low than number of rows and cols");
  }
  };

  bool operator==(const Malla& other) const{
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
  };

  bool has_life() {
	for (int i=0;i<getRows(); i++) {
	for (int j=0;j<getCols(); j++) {
	  bool val = getValue(i,j);
	  if (val) {
		return true;
	  }
	  }
	}
	return false;
  };

  int alive(){
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
  };
  int dead(){
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
  };

  ~Malla(){
	  delete[] matrix;
  };

  bool *getMatrix(){
	  return this->matrix;
  }

};

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
	 count_chars = 0;
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

/*
Define kernel
*/

__device__ bool getValue(const bool *matrix, int i, int j, int cols){
	return matrix[i*cols+j];
}

__global__ void gol_next_grid(
						  bool *start, 
						  bool *output, 
						  int cols, 
						  int rows){

  int worldSize = cols * rows;
  for (uint cellId = __mul24(blockIdx.x, blockDim.x) + threadIdx.x;
      cellId < worldSize;
      cellId += blockDim.x * gridDim.x) {

 	int xin = cellId*cols;//blockIdx.x * blockDim.x + threadIdx.x;
 	int yin = cellId - xin;//blockIdx.y * blockDim.y + threadIdx.y;

  
	int cell = getValue(start, xin, yin, cols);

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

	int up_val = getValue(start, up,yin, cols);
	int down_val = getValue(start, down,yin, cols);
	int left_val = getValue(start, xin,left, cols);
	int right_val = getValue(start, xin,right, cols);
	int up_east_val = getValue(start, up, right, cols);
	int up_west_val = getValue(start, up, left, cols);
	int down_east_val = getValue(start, down, right, cols);
	int down_west_val = getValue(start, down, left, cols);

	unsigned int check = 0;

	if (up_val) {check++;}
	if (down_val) {check++;}
	if (left_val) {check++;} 
	if (right_val) {check++;}
	if (up_east_val) {check++;}
	if (up_west_val) {check++;}
	if (down_east_val) {check++;}
	if (down_west_val) {check++;};

	int position = xin * cols + yin;


	output[position] = (cell==false && check==3) || (cell==true && (check==2 || check==3))?1:0;
	}
}





int main() {
	string filename =  "gpu_cuda_mod1.csv";

	unsigned int m_worldWidth;
	unsigned int m_worldHeight;
	unsigned int m_dataLength = 0;  // m_worldWidth * m_worldHeight

    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;

    // abrir un archivo csv para guardar los datos
    ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file\n";
        return 1;
    }
    // escribir la primera fila con los encabezados de las tablas del .csv
    file << "N, M, Iteration, Cells/sec, Time_sec\n";


	const int SEED = 500;
	
    srand(SEED);

	for (int i=5; i<15;i++){
	/*
	  Generate grid
	 */

		cout << i <<" Grilla de tamano: 2^" << i << "x 2^" << i << endl;

		m_worldWidth = pow(2,i);
		m_worldHeight = pow(2,i);
        m_dataLength = m_worldWidth * m_worldHeight;
		Malla grid(m_worldWidth, m_worldHeight);

		grid.gen_random(SEED);
		if (i<5){
			cout<<grid<<endl;
		}

		bool *hostInput = grid.getMatrix();
		bool *hostOutput = new bool[m_dataLength];

		/*
		  Alojar memoria en device
		 */
		bool *deviceInput;
		bool *deviceOutput;

		cudaMalloc((void**)&deviceInput, m_dataLength*sizeof(bool));
		cudaMalloc((void**)&deviceOutput, m_dataLength*sizeof(bool));
		
		cudaMemcpy(deviceInput, hostInput, m_dataLength*sizeof(bool), cudaMemcpyHostToDevice);

		dim3 blockSize(16,16);
		dim3 gridSize((m_worldWidth+blockSize.x-1)/blockSize.x,(m_worldHeight+blockSize.y-1)/blockSize.y);

		/* end init cuda*/

	/*
	  Compute iteration on diferent sizes
	 */	  
		for (int k=0;k<15; k++) {
            auto start_time = high_resolution_clock::now();
	
			// Grid old_grid = grid;
            // Grid grid(old_grid);
			gol_next_grid<<<gridSize,blockSize>>>(
												  deviceInput, 
												  deviceOutput,
												  m_worldWidth,
												  m_worldHeight);

            auto end_time = high_resolution_clock::now();

			cudaMemcpy(
					   deviceInput,  
					   deviceOutput, 
					   m_dataLength*sizeof(bool),
					   cudaMemcpyDeviceToDevice
					   );
			
        
            // se transforma el resultado a los segundos que tomo la ejecucion
            auto ms_int = duration_cast<milliseconds>(end_time - start_time);
            duration<double, milli> ms_double = end_time - start_time;
            double total_time = ms_double.count();
            double total_time_sec = total_time / 1000;

            // se calcula el numero de celdas evaluadas por segundo
            size_t num_cells_evaluated = m_dataLength;
            double cells_per_second = static_cast<double>(num_cells_evaluated)/static_cast<double>(total_time_sec);


            cout << "Celdas evaluadas por segundo: " << cells_per_second <<" en " << total_time_sec << " segundos. Iteracion: " << k+1 <<  endl;
            // se guardan los datos en un archivo csv
            file << m_worldWidth << "," << m_worldHeight << "," << k << "," << cells_per_second << "," << total_time_sec << "\n";

		}
		//cudaFree(deviceInput);
		//cudaFree(deviceOutput);

		//delete[]  hostInput;
		//delete[]  hostOutput;


	}


	return 0;
}