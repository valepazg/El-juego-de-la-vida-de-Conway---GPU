#include <iostream>
#include <chrono>
#include <stdlib.h>
#include <cstdlib>
#include <time.h>
#include <cmath>
#include <fstream>
#include <string>
using namespace std;

// se usa unsigned char porque es la estructura mas pequeña para guardar solo un bit 0 o 1
typedef unsigned int uint;
 
// se trata el universo bidimensional dentro de un arreglo unidimensional para mejorar eficiencia
// es decir la celda (x,y) en un espacio bidimensional, es ahora (y * ancho + x) en un espacio unidimensional
// siendo "ancho" el tamaño en x del universo
uint* m_data;
uint* m_resultData;
 
size_t m_worldWidth;
size_t m_worldHeight;
size_t m_dataLength;  // m_worldWidth * m_worldHeight

// funcion para contar las celdas vivas alrededor de la celda (x1,y1)
uint countAliveCells(size_t x0, size_t x1, size_t x2, size_t y0, size_t y1, size_t y2, uint *&m_data) {
    return m_data[y0 + x0] + m_data[y0 + x1] + m_data[y0 + x2] + m_data[y1 + x0] + 
           m_data[y1 + x2] + m_data[y2 + x0] + m_data[y2 + x1] + m_data[y2 + x2];

}

// funcion que calcula una iteracion del juego de la vida de Conway
// actualiza el estado del universo guardado en m_data
void computeIterationSerial(uint *&m_data, uint *&m_resultData) {

    // calcular los vecinos en las filas alrededor de y1
    for (size_t y = 0; y < m_worldHeight; y++) {
        // se toma el universo como ciclico, los vecinos del borde del arreglo son los del lado contrario del mismo
        size_t y0 = ((y + m_worldHeight - 1) % m_worldHeight) * m_worldWidth;
        size_t y1 = y * m_worldWidth;
        size_t y2 = ((y + 1) % m_worldHeight) * m_worldWidth;
        
 
    // calcular los vecinos en las columnas alrededor de x1
        for (size_t x = 0; x < m_worldWidth; x++) {
            size_t x0 = (x + m_worldWidth - 1) % m_worldWidth;
            size_t x2 = (x + 1) % m_worldWidth;
            
            // para cada x,y1 se calcula el numero de celdas vivas en su vecindario   
            uint aliveCells = countAliveCells(x0, x, x2, y0, y1, y2, m_data);
            
            // la celda esta viva si hay 3 celdas vivas a su alrededor o si esta viva y ademas hay 2 celdas vivas alrededor
            m_resultData[y1 + x] = (aliveCells == 3 || (aliveCells == 2 && m_data[x + y1])) ? 1 : 0;
        }
    }
    // se actualiza el estado del universo al fin de la operacion en el arreglo m_data
    swap(m_data, m_resultData);
}


int main(){

    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;

    // abrir un archivo csv para guardar los datos
    ofstream file("cpu_serial.csv");
    if (!file.is_open()) {
        std::cerr << "Failed to open file\n";
        return 1;
    }
    // escribir la primera fila con los encabezados de las tablas del .csv
    file << "N, M, Iteration, Cells/sec, Time_sec\n";
    
    // para cada tamaño de grilla de 2^i x 2^i, se realizan 10 iteraciones del juego de la vida de Conway
    for (int i = 5; i < 11; i++){
        cout << "Grilla de tamano: 2^" << i << "x 2^" << i << endl;
        srand(time(nullptr));
        m_worldWidth = pow(2,i);
        m_worldHeight = pow(2,i);
        m_dataLength = m_worldWidth * m_worldHeight;
            
        delete[] m_data;
        delete[] m_resultData;

        uint* m_data = new uint[m_dataLength];
        uint* m_resultData = new uint[m_dataLength];

        for (int j = 0; j < m_dataLength; j++) {
            m_data[j] = uint(rand() % 2);
        }
        
        
        for(int k  = 0; k < 10; k++){

            // se regitra la "hora" antes y despues de comenzar una iteración
            auto start_time = high_resolution_clock::now();
            computeIterationSerial(m_data, m_resultData);
            auto end_time = high_resolution_clock::now();
        
            // se transforma el resultado a los segundos que tomo la ejecucion
            auto ms_int = duration_cast<milliseconds>(end_time - start_time);
            duration<double, milli> ms_double = end_time - start_time;
            double total_time = ms_double.count();
            double total_time_sec = total_time / 1000;

            // se calcula el numero de celdas evaluadas por segundo
            size_t num_cells_evaluated = m_worldWidth * m_worldHeight;
            double cells_per_second = static_cast<double>(num_cells_evaluated)/static_cast<double>(total_time_sec);


            cout << "Celdas evaluadas por segundo: " << cells_per_second <<" en " << total_time_sec << " segundos. Iteracion: " << k+1 <<  endl;
            // se guardan los datos en un archivo csv
            file << m_worldWidth << "," << m_worldHeight << "," << k << "," << cells_per_second << "," << total_time_sec << "\n";
        }


    }

    delete[] m_data;
    delete[] m_resultData;

}





