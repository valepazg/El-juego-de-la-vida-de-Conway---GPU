#include <iostream>
#include "GameOfLife/grid.hpp"
#include "GameOfLife/cpu_serial.hpp"
#include <chrono>
#include <stdlib.h>
#include <cstdlib>
#include <time.h>
#include <cmath>
#include <fstream>
#include <string>


int serial_simulation(string filename) {

	size_t m_worldWidth;
	size_t m_worldHeight;
	size_t m_dataLength;  // m_worldWidth * m_worldHeight

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

	for (int i=5; i<14;i++){
	/*
	  Generate grid
	 */

		cout << i <<" Grilla de tamano: 2^" << i << "x 2^" << i << endl;

		m_worldWidth = pow(2,i);
		m_worldHeight = pow(2,i);
        m_dataLength = m_worldWidth * m_worldHeight;
		Grid grid(m_worldWidth, m_worldHeight);
		grid.gen_random(SEED);

	/*
	  Compute iteration on diferent sizes
	 */	  
		for (int k=0;k<30; k++) {
            auto start_time = high_resolution_clock::now();
            Grid grid(grid);
            auto end_time = high_resolution_clock::now();
        
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


	}

	return 0;
}
