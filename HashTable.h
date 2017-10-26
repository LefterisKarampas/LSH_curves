#ifndef HASH_TABLE_H_
#define HASH_TABLE_H_

#include "List.h"

template <typename Type>
class Bucket{
private:
	List<Type> *list;
public:
	Bucket();
	~Bucket();
	int Bucket_Insert(Type * );
	List<Type> * Bucket_Search(Type *,bool *);
	void Clear_up();
	Type * find_nearest_min(Type *,Type *,long double *,bool *,double,std::vector<char *> *,Type *,long double *,long double(*distance)(const std::vector< std::vector<double> >&,const std::vector< std::vector<double> >&));
	Type * find_nearest(Type*,Type *,long double *,long double(*distance)(const std::vector< std::vector<double> >&,const std::vector< std::vector<double> >&));
};

template <typename Type_Function, typename Type>
class HashTable{
private:
	Bucket<Type> ** T;
	int (*Hash_Function)(const Type_Function &,const std::vector<int> &,int,int,std::vector<double> **,double *);
	int buckets;
	int k_vec;
	std::vector<int> r;
	std::vector<double> **v;
	double * t;
public:
	HashTable(const int,const int,int(*hash_function)(const Type_Function &,const std::vector<int> &,int,int,std::vector<double> **,double *));
	~HashTable();
	int Hash(Type * x);
	int Hash_Insert(Type * x);
	List<Type> * Hash_Search(Type * x,bool *);
	void Clear_up();
	Type * Check_all(Type *,Type *,long double *,bool *,double,std::vector<char *> *,Type *,long double *,long double(*distance)(const std::vector< std::vector<double> >&,const std::vector< std::vector<double> >&));
};


#endif