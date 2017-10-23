#ifndef _HASH_FUNCTIONS_H
#define _HASH_FUNCTIONS_H

#include <vector>

int classic_function(const std::vector<double> &,const std::vector<int> & ,int,int,std::vector<double> **,double *);

int probabilistic(const std::vector<double> &,const std::vector<int> &,int,int,std::vector<double> **,double *);

#endif