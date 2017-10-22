#ifndef CURVE_H_
#define CURVE_H_

#include <vector>


template <typename T_Curve,typename T_GridCurve>
class Curve{
	T_Curve * curve;
	T_GridCurve *grid_curve;
	char * id;
public:
	Curve(T_Curve *, T_GridCurve *,char *);
	~Curve();
	const T_GridCurve & Get_GridCurve();
	bool Compare_GridCurve(Curve *);
	char * GetId();
	void Clear_up();
};




#endif