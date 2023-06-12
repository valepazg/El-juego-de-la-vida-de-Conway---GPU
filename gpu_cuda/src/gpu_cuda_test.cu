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
typedef unsigned int uint;

// __global__ void gol_next_grid(int *start, int *output, int cols, int rows) {
// 	int x = blockIdx.x;
// 	int y = blockIdx.y;

// 	output[x][y] = start[x][y]*start[x][y];
// }



int main() {
    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;

	const int SIZE = 100;
	unsigned int worldWidth=SIZE;
	unsigned int worldHeight=SIZE;
	unsigned int dataLength = worldHeight * worldWidth; 
    vector<vector<uint>> matrix(worldHeight, vector<uint>(worldWidth));               
	
	for (int j=0;j<worldHeight;j++){
		for (int k=0;k<worldHeight;k++){
			matrix[j][k] = uint(rand() % 10+1);
		}		
	}
	for (int j=0;j<worldHeight;j++){
		for (int k=0;k<worldHeight;k++){
			std::cout<<matrix[j][k]<<" ";
		}		
		std::cout<<std::endl;
	}
	


	int *deviceInput;
	int *deviceOutput;

	cudaMalloc((void**)&deviceInput, dataLength*sizeof(bool));
	cudaMalloc((void**)&deviceOutput, dataLength*sizeof(bool));

	// cudaMemcpy(
	// 		   deviceInput, 
	// 		   matrix, 
	// 		   dataLength*sizeof(bool), 
	// 		   cudaMemcpyHostToDevice);

	// dim3 blockSize(16,16);
	// dim3 gridSize((worldWidth+blockSize.x-1)/blockSize.x,
	// 			  (worldHeight+blockSize.y-1)/blockSize.y);


	return 0;
}