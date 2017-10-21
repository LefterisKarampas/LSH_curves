#ifndef HASH_FUNCTION_H_
#define HASH_FUNCTION_H_
#include <vector>
#include <iostream>
#include "List.h"
#include "List.cpp"
using namespace std;


template <typename Type>
class Bucket{
	List<Type> *list;
public:
	Bucket();
	~Bucket();
	int Bucket_Insert(Type * );
	Type * Bucket_Search(Type *);
};

template <typename Type_Function, typename Type>
class HashTable{
	Bucket<Type> ** T;
	int (*Hash_Function)(const Type_Function &,int);
	int buckets;
	int k_vec;
public:
	HashTable(const int,const int,int(*hash_function)(const Type_Function &,int));
	~HashTable();
	int Hash(Type & x);
	int Hash_Insert(Type * x);
	Type * Hash_Search(Type * x);
};

#endif