#include<iostream>

#include "GameOfLife/grid.hpp"
#include "GameOfLife/cpu_serial.hpp"


int main(int argc, char const *argv[])
{
    std::cout<<"El Juego de la Vida + CUDA - v0.0.1"<<std::endl;
	serial_simulation("cpu_serial.csv");
	//opencl_simulation("gpu_opencl_mode_simple.csv", "simple");

    return 0;
}
