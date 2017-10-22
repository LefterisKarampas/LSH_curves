#ifndef HASH_TABLE_H_
#define HASH_TABLE_H_
#include "List.h"

template <typename Type>
class Bucket{
	List<Type> *list;
public:
	Bucket();
	~Bucket();
	int Bucket_Insert(Type * );
	Type * Bucket_Search(Type *);
	void Clear_up();
};

template <typename Type_Function, typename Type>
class HashTable{
	Bucket<Type> ** T;
	int (*Hash_Function)(const Type_Function &,const std::vector<int> &,int);
	int buckets;
	int k_vec;
	std::vector<int> r;
public:
	HashTable(const int,const int,int(*hash_function)(const Type_Function &,const std::vector<int> &,int));
	~HashTable();
	int Hash(Type & x);
	int Hash_Insert(Type * x);
	Type * Hash_Search(Type * x);
	void Clear_up();
};


#endif