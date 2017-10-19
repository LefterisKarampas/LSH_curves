#ifndef LSH_CURVE_H_
#define LSH_CURVE_H_

#include "LSH_Curve.h"
#include "Grid.h"
#include "HashTable.h"

template <typename T,typename N>
class LSH_Curve{
	Grid<T> ** G;
	HashTable<N> *HT;
	int k;
public:
	LSH_Curve(int,int,int,int,int,int(*hash_function)(const N &,int));
	~LSH_Curve();
	int LSH_Insert(const N &);
};

#endif