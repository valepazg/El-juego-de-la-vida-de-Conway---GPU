#define __CL_ENABLE_EXCEPTIONS

#include <CL/cl.hpp>
#include "GameOfLife/util.hpp"
#include "GameOfLife/err_code.h"
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

void opencl_simple(){

    // abrir un archivo csv para guardar los datos
    ofstream file("opencl_simple.csv");
    if (!file.is_open()) {
        std::cerr << "Failed to open file\n";
    }
    // escribir la primera fila con los encabezados de las tablas del .csv
    file << "N, M, Iteration, Cells/sec, Time_sec\n";

    // para cada tamaño de grilla de 2^i x 2^i, se realizan 10 iteraciones del juego de la vida de Conway
    for(int i = 5; i < 11; i++){
        
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
            cl::Program program(context, util::loadProgram("simpleLifeKernel.cl"), true);
        
            // Get the command queue
            cl::CommandQueue queue(context);
        
            // Create the kernel functor

            auto simpleLifeKernel = cl::make_kernel<cl::Buffer, cl::Buffer, size_t, size_t>(program, "simpleLifeKernel");
        
            cl::Buffer d_lifeData(context, begin(h_lifeData), end(h_lifeData), true);
            cl::Buffer d_resultLifeData(context, CL_MEM_WRITE_ONLY, sizeof(uint) * dataLength);
            
            for(int k=0; k<10; k++){
                
                auto start_time = high_resolution_clock::now();
                
                simpleLifeKernel(cl::EnqueueArgs(queue,cl::NDRange(dataLength)), d_lifeData, d_resultLifeData, worldWidth, worldHeight);
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

void opencl_mod1(){

    // abrir un archivo csv para guardar los datos
    ofstream file("opencl_mod1.csv");
    if (!file.is_open()) {
        std::cerr << "Failed to open file\n";
    }

    // escribir la primera fila con los encabezados de las tablas del .csv
    file << "N, M, Iteration, Cells/sec, Time_sec\n";

    // para cada tamaño de grilla de 2^i x 2^i, se realizan 10 iteraciones del juego de la vida de Conway
    for(int i = 5; i < 11; i++){
        
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

            auto mod1LifeKernel = cl::make_kernel<cl::Buffer, cl::Buffer, size_t, size_t>(program, "mod1LifeKernel");
        
            cl::Buffer d_lifeData(context, begin(h_lifeData), end(h_lifeData), true);
            cl::Buffer d_resultLifeData(context, CL_MEM_WRITE_ONLY, sizeof(uint) * dataLength);
            
            for(int k=0; k<10; k++){
                
                auto start_time = high_resolution_clock::now();
                
                mod1LifeKernel(cl::EnqueueArgs(queue,cl::NDRange(dataLength)), d_lifeData, d_resultLifeData, worldWidth, worldHeight);
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

void opencl_mod2(){

    // abrir un archivo csv para guardar los datos
    ofstream file("opencl_mod2.csv");
    if (!file.is_open()) {
        std::cerr << "Failed to open file\n";
    }

    // escribir la primera fila con los encabezados de las tablas del .csv
    file << "N, M, Iteration, Cells/sec, Time_sec\n";

    // para cada tamaño de grilla de 2^i x 2^i, se realizan 10 iteraciones del juego de la vida de Conway
    for(int i = 5; i < 11; i++){

        
        size_t worldWidth = pow(2,i);
        size_t worldHeight = pow(2,i);
        size_t dataLength;
        dataLength = worldWidth * worldWidth;
        vector<vector<uint>> h_lifeData(worldHeight, vector<uint>(worldWidth));               
        vector<uint> h_tempLifeData(dataLength);               
        vector<vector<uint>> h_resultLifeData(worldHeight, vector<uint>(worldWidth));                     	
        // vector<uint> h_resultLifeData (dataLength);    // c = a + b, from compute device

        
        srand(time(nullptr)); 

        
        for (int j = 0; j < worldHeight; j++) {
            for(int l = 0; l < worldWidth; l++){
                h_lifeData[j][l] = uint(rand() % 2);
                h_tempLifeData[i * worldWidth + j] = h_lifeData[j][l];
            }
        }

        try {
            // Create a context
            cl::Context context(DEVICE);
        
            // Load in kernel source, creating a program object for the context
            cl::Program program(context, util::loadProgram("mod2LifeKernel.cl"), true);
        
            // Get the command queue
            cl::CommandQueue queue(context);
        
            // Create the kernel functor

            auto mod2LifeKernel = cl::make_kernel<cl::Buffer, cl::Buffer, size_t, size_t>(program, "mod2LifeKernel");
        
            cl::Buffer d_lifeData(context, begin(h_tempLifeData), end(h_tempLifeData), true);
            cl::Buffer d_resultLifeData(context, CL_MEM_READ_WRITE, sizeof(uint) * dataLength);

            
            for(int k=0; k<10; k++){
                
                auto start_time = high_resolution_clock::now();
                
                mod2LifeKernel(cl::EnqueueArgs(queue,cl::NDRange(worldHeight, worldWidth)), d_lifeData, d_resultLifeData, worldWidth, worldHeight);
                queue.finish();
                swap(d_lifeData, d_resultLifeData);

 
                cl::copy(queue, d_lifeData, begin(h_tempLifeData), end(h_tempLifeData));

                for (int i = 0; i < worldHeight; ++i) {
                    for (int j = 0; j < worldWidth; ++j) {
                        h_lifeData[i][j] = h_tempLifeData[i * worldWidth + j];
                    }
                }
                
                auto end_time = high_resolution_clock::now();
                
                // se transforma el resultado a los segundos que tomo la ejecucion
                auto ms_int = duration_cast<milliseconds>(end_time - start_time);
                duration<double, milli> ms_double = end_time - start_time;
                double total_time = ms_double.count();
                double total_time_sec = total_time / 1000;
                
                // se calcula el numero de celdas evaluadas por segundo
                size_t num_cells_evaluated = worldWidth * worldHeight;
                double cells_per_second = static_cast<double>(num_cells_evaluated)/static_cast<double>(total_time_sec);
                
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