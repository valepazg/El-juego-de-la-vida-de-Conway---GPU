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

	unsigned int check = up_val + down_val + left_val + right_val+up_east_val+up_west_val+down_east_val+down_west_val;

	int position = xin * cols + yin;


	output[position] = (cell==false && check==3) || (cell==true && (check==2 || check==3))?1:0;
	}

}





int main() {
	string filename =  "gpu_cuda_mod2.csv";

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

	srand(500);
	auto gen = std::bind(std::uniform_int_distribution<>(0,1),std::default_random_engine());


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

        vector<vector<bool>> h_lifeData(m_worldHeight, vector<bool>(m_worldWidth));               
        bool* h_tempLifeData= new bool[m_dataLength];               
        vector<vector<bool>> h_resultLifeData(m_worldHeight, vector<bool>(m_worldWidth));                     	

        for (int j = 0; j < m_worldHeight; j++) {
            for(int l = 0; l < m_worldWidth; l++){
                h_lifeData[j][l] = gen();
                h_tempLifeData[i * m_worldWidth + j] = h_lifeData[j][l];
            }
        }


		/*
		  Alojar memoria en device
		 */
		bool *deviceInput;
		bool *deviceOutput;
		
		// crear espacio de memoria para la matriz
		cudaMalloc((void**)&deviceInput, m_dataLength*sizeof(bool));
		cudaMalloc((void**)&deviceOutput, m_dataLength*sizeof(bool));
		

		// cpu -> GPU
		cudaMemcpy(
				   deviceInput, 
				   h_tempLifeData, 
				   m_dataLength*sizeof(bool), 
				   cudaMemcpyHostToDevice);


		//
		dim3 blockSize(16,16);
		dim3 gridSize(
					  (m_worldWidth+blockSize.x-1)/blockSize.x,
					  (m_worldHeight+blockSize.y-1)/blockSize.y);
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