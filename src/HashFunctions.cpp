#include "../include/HashFunctions.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <climits>
#include "../include/generator.h"

#define w 4

using namespace std;
static unsigned int M = (unsigned) (2<<31) - 5;
static std::vector<double> v[3];


int classic_function(const std::vector<double> & p,const std::vector<int> & r,int buckets,int k_vec,std::vector<double> **v,double * t){
	std::vector<double>::const_iterator i;
	long double sum = 0;
	for (unsigned int i =0;i<p.size();i++){
	    sum += p[i] * r[i];
	}
	if(sum < 0){
		sum *= -1;
	}
	long int index;
	index = (long int) sum % M;
	index %= buckets;
	return index;
};

int probabilistic(const std::vector<double> &p,const std::vector<int> & r,int buckets,int k_vec,std::vector<double> **v,double * t){
	unsigned int size = p.size();
	long int sum =0;
	int index;
	for(int j=0;j<k_vec;j++){
		long double temp = 0;
		if(v[j]->size() < size){
			unsigned int dif =  size - v[j]->size();
			for(unsigned int i =0;i<dif;i++){
				v[j]->push_back(Uniform_Generator(0,1));
			}
		}
		for(unsigned int i=0;i<size;i++){
			temp += p[i] * (*(v[j]))[i];
		}
		temp = (int) (temp + t[j]) / w;
		sum += (int) temp * r[j];
	}
	if(sum < 0)
		sum *= -1;
	index = (long int) sum % M;
	index %=buckets;
	return index;
};

