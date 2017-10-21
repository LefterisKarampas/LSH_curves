#include "HashTable.h"
#include "List.h"
#include "Curve.h"
#include "Curve.cpp"
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
int Bucket<Type>::Bucket_Insert(Type * x){
	if(list == NULL){
		this->list = new List<Type>();
	}
	return this->list->List_Insert(x);
	//this->list->print();
}

template <typename Type>
Type * Bucket<Type>::Bucket_Search(Type * x){
	if(list != NULL){
		return this->list->List_Search(x);
	}
	else{
		return NULL;
	}
	//this->list->print();
}

template <typename Type_Function, typename Type>
HashTable<Type_Function,Type>::HashTable(const int k_vect,const int n,int(*hash_function)(const Type_Function &,int)):k_vec(k_vect),buckets(n),Hash_Function(hash_function){
	this->T = new Bucket<Type>*[n];
	for(int i=0;i<n;i++){
		this->T[i] = new Bucket<Type>();
	}
}

template <typename Type_Function, typename Type>
HashTable<Type_Function,Type>::~HashTable(){
	for(int i=0;i<this->buckets;i++){
		delete this->T[i];
	}
	delete[] this->T;
}
template <typename Type_Function, typename Type>
int HashTable<Type_Function,Type>::Hash(Type & x){
	return (*this->Hash_Function)(x.Get_GridCurve(),this->buckets);
}

template <typename Type_Function, typename Type>
int HashTable<Type_Function,Type>::Hash_Insert(Type * x){
	int bucket = this->Hash(*x);
	if(bucket >= this->buckets){
		cerr << "Fail hash function: Index = " << bucket << endl;
		return -1;
	}
	return this->T[bucket]->Bucket_Insert(x);

}


template <typename Type_Function, typename Type>
Type * HashTable<Type_Function,Type>::Hash_Search(Type * x){
	int bucket = this->Hash(*x);
	if(bucket >= this->buckets){
		cerr << "Fail hash function: Index = " << bucket << endl;
		return NULL;
	}
	return this->T[bucket]->Bucket_Search(x);
}
