#define __CL_ENABLE_EXCEPTIONS

#include <CL/cl.hpp>
#include "util.hpp"
#include "err_code.h"
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <chrono>
#include <cmath>
#include <fstream>
#include <string>

#include <iostream>
#include <fstream>


// pick up device type from compiler command line or from the default type
#ifndef DEVICE
#define DEVICE CL_DEVICE_TYPE_DEFAULT
#endif

using namespace std;
typedef unsigned int uint;

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;

int main(void){

    std::string kernelSource = R"(

	   __kernel void mod1LifeKernel(__global uint* lifeData, __global uint* resultLifeData, const size_t worldWidth, const size_t worldHeight){
    int cellId = get_global_id(0);  
    size_t worldSize = worldWidth * worldHeight;   
   
    size_t x = cellId % worldWidth;
    size_t yAbs = cellId - x;
    size_t xLeft = (x + worldWidth - 1) % worldWidth;
    size_t xRight = (x + 1) % worldWidth;
    size_t yAbsUp = (yAbs + worldSize - worldWidth) % worldSize;
    size_t yAbsDown = (yAbs + worldWidth) % worldSize;
    
    size_t aliveCells = 0;

    if(lifeData[xLeft + yAbsUp] == 1){
        aliveCells++;
    }

    if(lifeData[x + yAbsUp] == 1){
        aliveCells++;
    }

    if(lifeData[xRight + yAbsUp] == 1){
        aliveCells++;
    }

    if(lifeData[xLeft + yAbs] == 1){
        aliveCells++;
    }

    if(lifeData[xRight + yAbs] == 1){
        aliveCells++;
    }

    if(lifeData[xLeft + yAbsDown] == 1){
        aliveCells++;
    }

    if(lifeData[x + yAbsDown] == 1){
        aliveCells++;
    }

    if(lifeData[xRight + yAbsDown] == 1){
        aliveCells++;
    }

    resultLifeData[x + yAbs] = (aliveCells == 3 || (aliveCells == 2 && lifeData[x + yAbs])) ? 1 : 0;
}      

    )";

    // abrir un archivo csv para guardar los datos
    ofstream file("opencl_mod1.csv");
    if (!file.is_open()) {
        std::cerr << "Failed to open file\n";
        return 1;
    }
    // escribir la primera fila con los encabezados de las tablas del .csv
    file << "N, M, Iteration, Cells/sec, Time_sec\n";

    // para cada tamaño de grilla de 2^i x 2^i, se realizan 10 iteraciones del juego de la vida de Conway
    for(int i = 5; i < 11; i++){

        cout << "Grilla de tamano: 2^" << i << "x 2^" << i << endl;
        
        size_t worldWidth = pow(2,i);
        size_t worldHeight = pow(2,i);
        size_t dataLength;
        dataLength = worldWidth * worldWidth;
        vector<uint> h_lifeData(dataLength);                // b vector 	
        vector<uint> h_resultLifeData (dataLength);    // c = a + b, from compute device

        
        srand(time(nullptr)); 

        for (int j = 0; j < dataLength; j++) {
            h_lifeData[j] = uint(rand() % 2);
        }

        try {
            // Create a context
            cl::Context context(DEVICE);
        
            // Load in kernel source, creating a program object for the context
            cl::Program program(context, util::loadProgram("mod1LifeKernel.cl"), true);
        
            // Get the command queue
            cl::CommandQueue queue(context);
        
            // Create the kernel functor

            auto mod1LifeKernel = cl::compatibility::make_kernel<cl::Buffer, cl::Buffer, size_t, size_t>(program, "mod1LifeKernel");
        
            cl::Buffer d_lifeData(context, begin(h_lifeData), end(h_lifeData), true);
            cl::Buffer d_resultLifeData(context, CL_MEM_WRITE_ONLY, sizeof(uint) * dataLength);
            
            for(int k=0; k<10; k++){
                
                auto start_time = high_resolution_clock::now();
                
                mod1LifeKernel(
							   cl::EnqueueArgs(queue,cl::NDRange(dataLength)), 
							   d_lifeData, 
							   d_resultLifeData, 
							   worldWidth, 
							   worldHeight);
                queue.finish();
                swap(d_lifeData, d_resultLifeData);
                cl::copy(queue, d_lifeData, begin(h_lifeData), end(h_lifeData));
                
                auto end_time = high_resolution_clock::now();
                
                // se transforma el resultado a los segundos que tomo la ejecucion
                auto ms_int = duration_cast<milliseconds>(end_time - start_time);
                duration<double, milli> ms_double = end_time - start_time;
                double total_time = ms_double.count();
                double total_time_sec = total_time / 1000;
                
                // se calcula el numero de celdas evaluadas por segundo
                size_t num_cells_evaluated = worldWidth * worldHeight;
                double cells_per_second = static_cast<double>(num_cells_evaluated)/static_cast<double>(total_time_sec);
                
                cout << "Celdas evaluadas por segundo: " << cells_per_second <<" en " << total_time_sec << " segundos. Iteracion: " << k+1 <<  endl;
                // se guardan los datos en un archivo csv
                file << worldWidth << "," << worldHeight << "," << k << "," << cells_per_second << "," << total_time_sec << "\n";
                
            }
        }

        catch (cl::Error err) {
            cout << "Exception\n"; 
            cerr << "ERROR: " << err.what() << "(" << err_code(err.err()) << ")" << std::endl;
        }

    }

    file.close();
}
