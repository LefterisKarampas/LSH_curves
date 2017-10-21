#include "HashFunctions.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <climits>


using namespace std;


int classic_function(const std::vector<double> & v,int buckets){
	std::vector<double>::const_iterator i;
	long double sum = 0;
	for (i = v.begin(); i != v.end(); ++i){
	    sum += *i;
	}
	int index;
	index = (long int) sum % INT_MAX;
	index %= buckets;
	return index;
	return 0;
};

int probabilistic(const std::vector<double> &v,int buckets){
	return 0;
};

