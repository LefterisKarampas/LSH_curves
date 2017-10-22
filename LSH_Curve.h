#ifndef LSH_CURVE_H_
#define LSH_CURVE_H_

#include "LSH_Curve.h"
#include "Grid.h"
#include "HashTable.h"

template <typename T,typename N,typename C>
class LSH_Curve{
private:
	Grid<T,N> ** G;
	HashTable<N,C> *HT;
	int k;
	int id;
public:
	LSH_Curve(int,int,int,int,int,int,int(*hash_function)(const N &,const std::vector<int> &,int));
	~LSH_Curve();
	int LSH_Insert(T *,char *);
	C * LSH_Search(T *,char *);
};


#endif