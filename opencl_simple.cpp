#define __CL_ENABLE_EXCEPTIONS

#include <CL/cl.hpp>
#include "util.hpp"
#include "err_code.h"
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <string>

#include <iostream>
#include <fstream>


// pick up device type from compiler command line or from the default type
#ifndef DEVICE
#define DEVICE CL_DEVICE_TYPE_DEFAULT
#endif

//------------------------------------------------------------------------------

#define TOL    (0.001)   // tolerance used in floating point comparisons
#define LENGTH (1024)    // length of vectors a, b, and c

using namespace std;
typedef unsigned int uint;

int main(void){

    srand(time(nullptr));
    
    //ubyte *h_lifeData;                // a vector 
    //ubyte *h_resultLifeData;                // a vector 
    size_t worldWidth = 10;
    size_t worldHeight = 10;
    size_t dataLength;

    dataLength = worldWidth * worldWidth;
    vector<uint> h_lifeData(dataLength);                // b vector 	
    vector<uint> h_resultLifeData (dataLength);    // c = a + b, from compute device

    
    cl::Buffer d_lifeData;                        // device memory used for the input  a vector
    cl::Buffer d_resultLifeData;                        // device memory used for the input  b vector
                  

    for (int i = 0; i < dataLength; i++) {
            h_lifeData[i] = uint(rand() % 2);
            cout << h_lifeData[i] << " ";
            if (i % worldWidth == worldWidth-1) {
                cout << endl;
            }
        }
        cout << endl;

    
    try 
    {
    	// Create a context
        cl::Context context(DEVICE);

    
        // Load in kernel source, creating a program object for the context

        cl::Program program(context, util::loadProgram("simpleLifeKernel.cl"), true);

    
        // Get the command queue
        cl::CommandQueue queue(context);

    
        // Create the kernel functor
 
        auto simpleLifeKernel = cl::make_kernel<cl::Buffer, cl::Buffer, size_t, size_t>(program, "simpleLifeKernel");
    

        d_lifeData   = cl::Buffer(context, begin(h_lifeData), end(h_lifeData), true);
        d_resultLifeData  = cl::Buffer(context, CL_MEM_WRITE_ONLY, sizeof(uint) * dataLength);

    
        util::Timer timer;

        simpleLifeKernel(
            cl::EnqueueArgs(
                queue,
                cl::NDRange(dataLength)), 
            d_lifeData,
            d_resultLifeData,
            worldWidth,
            worldHeight);

        queue.finish();
        swap(d_lifeData, d_resultLifeData);
    

        double rtime = static_cast<double>(timer.getTimeMilliseconds()) / 1000.0;
        printf("\nThe kernels ran in %lf seconds\n", rtime);

    
        cl::copy(queue, d_lifeData, begin(h_lifeData), end(h_lifeData));

        for (int i = 0; i < dataLength; i++) {
            cout << h_lifeData[i] << " ";
            if (i % worldWidth == worldWidth-1) {
                cout << endl;
            }
        }
        cout << endl;
    
        
    }

    catch (cl::Error err) {
        std::cout << "Exception\n";
        std::cerr 
            << "ERROR: "
            << err.what()
            << "("
            << err_code(err.err())
           << ")"
           << std::endl;
    }
}