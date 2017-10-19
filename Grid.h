#ifndef _GRID_H_
#define _GRID_H_

#include <vector>

template <typename T>
class Grid{
	std::vector<double> t;
	double delta;
public:
	Grid(int,int);
	~Grid();
	std::vector<T> Create_GridCurve(const std::vector<T> &);
};

#endif
