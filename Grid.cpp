#include <vector>
#include "Grid.h"
#include <iostream>

using namespace std;

template <typename T>
Grid<T>::Grid(int d,int m){
	for(int i=0;i<d;i++){
		this->t.push_back(0.1*i);
	}
	this->delta = 1;
}


template <typename T>
Grid<T>::~Grid(){
	;
}

template <typename T>
std::vector<T> Grid<T>::Create_GridCurve(const std::vector<T> & v){
	std::vector<T> vec;
	for(int i=0;i<v.size();i++){
		T temp = (int)(v[i]+0.5)/this->delta - this->t[i];
		vec.push_back(temp);
	}
	return vec;
}

