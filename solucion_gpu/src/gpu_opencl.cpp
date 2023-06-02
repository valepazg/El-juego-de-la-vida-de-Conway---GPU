#define CL_HPP_ENABLE_EXCEPTIONS
#define CL_TARGET_OPENCL_VERSION 300
#define CL_HPP_TARGET_OPENCL_VERSION 300

// pick up device type from compiler command line or from the default type

#include <iostream>
#include "GameOfLife/grid.hpp"
#include "GameOfLife/gpu_opencl.hpp"
#include "GameOfLife/util.hpp"
#include "GameOfLife/err_code.h"
#include <chrono>
#include <stdlib.h>
#include <cstdlib>
#include <time.h>
#include <cmath>
#include <fstream>
#include <string>
#include <CL/opencl.hpp>

#ifndef DEVICE
#define DEVICE CL_DEVICE_TYPE_DEFAULT
#endif

using namespace std;
typedef unsigned int uint;

int main() {
  string mode = "simple";
  string filename = "gpu_opencl_"+mode+"_simple.csv";
    string filename_mode = mode + "LifeKernel.cl";
	size_t worldWidth;
	size_t worldHeight;
	size_t dataLength;  // m_worldWidth * m_worldHeight

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
	

	for (int i=5; i<16;i++){
	/*
	  Generate grid
	 */

		cout << i <<" Grilla de tamano: 2^" << i << "x 2^" << i << endl;

		worldWidth = pow(2,i);
		worldHeight = pow(2,i);

        dataLength = worldWidth * worldHeight;
		Grid grid(worldWidth, worldHeight);

		grid.gen_random(SEED);

        size_t dataLength;


        vector<uint> h_lifeData(dataLength);                // b vector 	
        vector<uint> h_resultLifeData (dataLength);    // c = a + b, from compute device

	/*
	  Compute iteration on diferent sizes WITH GPU Opencl
	 */	  
		try {
		  
		  /*
			Load kernel opencl
		   */
            // Create a context
            cl::Context context(DEVICE);
        
            // Load in kernel source, creating a program object for the context
            cl::Program program(context, util::loadProgram("./src/gpu/opencl/"+mode+"LifeKernel.cl"), true);
        
            // Get the command queue
            cl::CommandQueue queue(context);
        
            // Create the kernel functor
			double* lifeData = &grid.matrix[0][0];
			
            auto LifeKernel = cl::compatibility::make_kernel<cl::Buffer, cl::Buffer, size_t, size_t>(program, mode+"LifeKernel");
        
            cl::Buffer d_lifeData(context, begin(lifeData), end(lifeData), true);
            cl::Buffer d_resultLifeData(context, CL_MEM_WRITE_ONLY, sizeof(uint) * dataLength);

		  /*
			end load kernel
		   */
		  
		for (int k=0;k<30; k++) {
            auto start_time = high_resolution_clock::now();
            Grid grid(grid);
			/* start new iteration */
			LifeKernel(cl::EnqueueArgs(queue,cl::NDRange(dataLength)), d_lifeData, d_resultLifeData, worldWidth, worldHeight);
			queue.finish();
			swap(d_lifeData, d_resultLifeData);
			cl::copy(queue, d_lifeData, begin(h_lifeData), end(h_lifeData));
			/* end new iteration*/
            auto end_time = high_resolution_clock::now();
        
            // se transforma el resultado a los segundos que tomo la ejecucion
            auto ms_int = duration_cast<milliseconds>(end_time - start_time);
            duration<double, milli> ms_double = end_time - start_time;
            double total_time = ms_double.count();
            double total_time_sec = total_time * 1000;

            // se calcula el numero de celdas evaluadas por segundo
            size_t num_cells_evaluated = dataLength;
            double cells_per_second = static_cast<double>(num_cells_evaluated)/static_cast<double>(total_time_sec);


            cout << "Celdas evaluadas por segundo: " << cells_per_second <<" en " << total_time_sec << " segundos. Iteracion: " << k+1 <<  endl;
            // se guardan los datos en un archivo csv
            file << worldWidth << "," << worldHeight << "," << k << "," << cells_per_second << "," << total_time_sec << "\n";

		}

		} catch (...) {
            cout << "Exception\n"; 
            //cerr << "ERROR: " << err.what() << "(" << err_code(err.err()) << ")" << std::endl;
        }


	}

	return 0;
}

