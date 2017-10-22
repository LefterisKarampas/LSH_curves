#ifndef _GRID_H_
#define _GRID_H_

#include <vector>


using namespace std;


template <typename T,typename N>
class Grid{
	std::vector<double> t;
	double delta;
	//double Generator(double,double);
public:
	Grid(int,int);
	~Grid();
	N * Create_GridCurve(const T &);
};



#endif
