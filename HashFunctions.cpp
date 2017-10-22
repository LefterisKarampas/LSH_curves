#include "HashFunctions.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <climits>


using namespace std;
static unsigned int M = (unsigned) (2<<31) - 5;

int classic_function(const std::vector<double> & v,const std::vector<int> & r,int buckets){
	std::vector<double>::const_iterator i;
	long double sum = 0;
	for (int i =0;i<v.size();i++){
	    sum += v[i] * r[i];
	}
	if(sum < 0){
		sum *= -1;
	}
	long int index;
	index = (long int) sum % M;
	index %= buckets;
	return index;
};

int probabilistic(const std::vector<double> &v,const std::vector<int> & r,int buckets){
	return 0;
};

