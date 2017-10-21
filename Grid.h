#ifndef _GRID_H_
#define _GRID_H_

#include <vector>

double randn (double , double );

template <typename T,typename N>
class Grid{
	std::vector<double> t;
	double delta;
public:
	Grid(int,int);
	~Grid();
	N * Create_GridCurve(const T &);
};

#endif
