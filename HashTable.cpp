#include "HashTable.h"
#include "List.h"
#include <iostream>

using namespace std;

template <typename Type>
Bucket<Type>::Bucket(){
	this->list = NULL;
	//cout << "Bucket created!" << endl;
}
template <typename Type>
Bucket<Type>::~Bucket(){
	if(this->list != NULL){
		delete this->list;
	}
}
template <typename Type>
int Bucket<Type>::Bucket_Insert(const Type & x){
	if(list == NULL){
		this->list = new List<Type>();
	}
	return this->list->List_Insert(x);
	//this->list->print();
}

template <typename Type>
HashTable<Type>::HashTable(const int k_vect,const int n,int(*hash_function)(const Type &,int)):k_vec(k_vect),buckets(n),Hash_Function(hash_function){
	this->T = new Bucket<Type>*[n];
	for(int i=0;i<n;i++){
		this->T[i] = new Bucket<Type>();
	}
}

template <typename Type>
HashTable<Type>::~HashTable(){
	for(int i=0;i<this->buckets;i++){
		delete this->T[i];
	}
	delete[] this->T;
}
template <typename Type>
int HashTable<Type>::Hash(const Type & x){
	return (*this->Hash_Function)(x,this->buckets);
}

template <typename Type>
int HashTable<Type>::Hash_Insert(const Type & x){
	int bucket = this->Hash(x);
	if(bucket >= this->buckets){
		cerr << "Fail hash function: Index = " << bucket << endl;
		return -1;
	}
	return this->T[bucket]->Bucket_Insert(x);
}
