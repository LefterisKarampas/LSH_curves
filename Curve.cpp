#include "Curve.h"
#include <iostream>
#include <vector>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


using namespace std;


template <typename T_Curve,typename T_GridCurve>
Curve<T_Curve,T_GridCurve>::Curve(T_Curve * curve_,T_GridCurve * grid_curve_,char *id_):curve(curve_),grid_curve(grid_curve_){
	this->id = (char *)malloc(strlen(id_)+1);
	strcpy(this->id,id_);
}

template <typename T_Curve,typename T_GridCurve>
Curve<T_Curve,T_GridCurve>::~Curve(){
	//delete this->curve;
	delete this->grid_curve;
	//free(this->id);
}


template <typename T_Curve,typename T_GridCurve>
const T_GridCurve & Curve<T_Curve,T_GridCurve>::Get_GridCurve(){
	return *(this->grid_curve);
}

template <typename T_Curve,typename T_GridCurve>
bool Curve<T_Curve,T_GridCurve>::Compare_GridCurve(Curve *v){
	if(!strcmp(v->GetId(),this->GetId())){
		return false;
	}
	T_GridCurve t1 = v->Get_GridCurve();
	if(t1.size() != this->grid_curve->size()){
		return false;
	}
	else{
		for(int i=0;i<t1.size();i++){
			if(t1[i] != (*this->grid_curve)[i]){
				return false;
			}
		}
		return true;
	}
}

template <typename T_Curve,typename T_GridCurve>
char * Curve<T_Curve,T_GridCurve>::GetId(){
	return this->id;
}

