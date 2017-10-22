#ifndef _GRID_CPP_
#define _GRID_CPP_
#include <vector>
#include "Grid.h"
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include "generator.h"


using namespace std;

template <typename T,typename N>
Grid<T,N>::Grid(int d,int m){
	for(int i=0;i<d;i++){
		double x;
		do{
			x = Uniform_Generator(0,1);
		}while(x<0 || x>=d);
		this->t.push_back(x);
	}
	this->delta = 1;
}


template <typename T,typename N>
Grid<T,N>::~Grid(){
	;
}

template <typename T,typename N>
N * Grid<T,N>::Create_GridCurve(const T & v){
	N *vec = new N();
	double prev;
	for(int k=0;k<v.size();k++){
		for(int i=0;i<v[k].size();i++){
			double temp = (int)(((v[k])[i]+0.5)/this->delta) + this->t[i];
			if(i == 0 || prev != temp)
				vec->push_back(temp);
			prev = temp;
		}
	}
	return vec;
}


#endif