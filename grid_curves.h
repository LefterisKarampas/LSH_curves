#ifndef GRID_CURVE_H_
#define GRID_CURVE_H_

#include <iostream>
#include <vector>

using namespace std;

class Grid_curve{
private:
	std::vector<double *> grid_curve;
public:
	Grid_curve(std::vector<double *> v);
	~Grid_curve();
};

#endif