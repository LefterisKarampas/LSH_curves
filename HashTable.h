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
	int Bucket_Insert(const Type & x);
};

template <typename Type>
class HashTable{
	Bucket<Type> ** T;
	int (*Hash_Function)(const Type &,int);
	int buckets;
	int k_vec;
public:
	HashTable(const int,const int,int(*hash_function)(const Type &,int));
	~HashTable();

	int Hash(const Type & x);

	int Hash_Insert(const Type & x);
};

#endif