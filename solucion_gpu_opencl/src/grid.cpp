#include <iostream>
#include <string>
#include <cassert>
#include <cmath>
#include "grid.hpp"


using namespace std;

#define N 10

template <class T>
Grid::Grid(T xin, T yin){
  this->pair = {xin, yin};
};


template <class T>
Grid::Grid(T vin){
  this->pair = {vin, vin};
};

template <class T>
Grid::Grid(){
  this->pair = {N, N};
};





