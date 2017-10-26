#ifndef LSH_CURVE_H_
#define LSH_CURVE_H_

#include "Grid.h"
#include "HashTable.h"
#include "List.h"

template <typename T,typename N,typename C>
class LSH_Curve{
private:
	Grid<T,N> ** G;
	HashTable<N,C> *HT;
	int k;
	int id;
public:
	LSH_Curve(int,int,int,int,int,int,int(*hash_function)(const N &,const std::vector<int> &,int,int,std::vector<double> **,double *));
	~LSH_Curve();
	N * Create_GridCurve(T *);
	int LSH_Insert(T *,char *);
	List<C> * LSH_Search(T *,char *,bool *);
	C * Check_all(T *,char *,C * ,long double *,bool *,double,std::vector<char *> *,C *,long double *,long double (*distance)(const T &,const T &));
};


#endif