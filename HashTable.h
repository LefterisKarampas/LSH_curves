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
	Bucket(){
		this->list = NULL;
		//cout << "Bucket created!" << endl;
	}
	~Bucket(){
		if(this->list != NULL){
			delete this->list;
		}
	}
	int Bucket_Insert(const Type & x){
		if(list == NULL){
			this->list = new List<Type>();
		}
		this->list->List_Insert(x);
		//this->list->print();
	}
};

template <typename Type>
class HashTable{
	Bucket<Type> * T;
	int (*Hash_Function)(const Type &);
	int buckets;
public:
	HashTable(const int n,int(*hash_function)(const Type &)):buckets(n),Hash_Function(hash_function){
		this->T = new Bucket<Type>[n];
	}
	~HashTable(){
		delete[] this->T;
	}
	int Hash(const Type & x){
		return (*this->Hash_Function)(x);
	}

	int Hash_Insert(const Type & x){
		int bucket = this->Hash(x);
		return this->T[bucket].Bucket_Insert(x);
	}
};

#endif