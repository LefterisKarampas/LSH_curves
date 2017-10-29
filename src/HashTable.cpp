#include "../include/HashTable.h"
#include "../include/List.h"
#include "List.cpp"
#include "../include/Curve.h"
#include <iostream>
#include <limits.h>
#include <stdlib.h>
#include <vector>
#include "../include/generator.h"


//rand [M,N]
#define MIN 1
#define MAX 1000

#define W 4


using namespace std;


//----------------------------BUCKET FUNCTIONS---------------------------------------

template <typename Type>
Bucket<Type>::Bucket(){								//Initialize the Bucket
	this->list = NULL;
}



template <typename Type>	
Bucket<Type>::~Bucket(){							//Delete the Bucket
	if(this->list != NULL){
		delete this->list;							//Delete the list if exists
	}
}



template <typename Type>
int Bucket<Type>::Bucket_Insert(Type * x){
	if(this->list == NULL){							//If list is empty
		this->list = new List<Type>();				//Initialize it
	}
	return this->list->List_Insert(x);				//Insert a new object
	//this->list->print();
}



template <typename Type>
List<Type> * Bucket<Type>::Bucket_Search(Type * x,bool *flag){		//Search into Bucket
	if(list != NULL){												//If list is not empty
		return this->list->List_Search(x,flag);						//Search the list
	}
	else{
		return NULL;												//Otherwise return NULL
	}
}



template <typename Type>							//Clear up the list
void Bucket<Type>::Clear_up(){
	if(this->list != NULL){							//If list not empty
		this->list->Clear_up();						//Clear up the list
	}
}



template <typename Type>		//Find the nearest and the real neighbor and their distances in curve_bucket
Type * Bucket<Type>::find_nearest_min(Type *curve,Type *neigh,long double *neigh_dist,bool *cond,double R,std::vector<char *> *r_near,Type * nearest_neigh,long double *nearest_dist,long double(*distance)(const std::vector< std::vector<double> >&,const std::vector< std::vector<double> >&)){
	if(this->list != NULL){							//If list is not empty
		nearest_neigh = NULL;						//Search for the neighbors there
		nearest_neigh = this->list->find_nearest_min(curve,neigh,neigh_dist,cond,R,r_near,nearest_neigh,nearest_dist,distance);
		return nearest_neigh;						//Return the real nearest
	}
	else{											//Othwerwise
		neigh = NULL;								
		*cond = false;								//Set flag false for existing same grid_curve neighbor
		return NULL;								//Return null
	}
}
	


template <typename Type>							//Find the real neighbor and its distance in not curve_bucket
Type * Bucket<Type>::find_nearest(Type *curve,Type *nearest_neigh,long double *nearest_dist,long double(*distance)(const std::vector< std::vector<double> >&,const std::vector< std::vector<double> >&)){
	if(this->list != NULL){							//If list is not empty
		Type *nearest_neigh = NULL;					//Search for the neighbors there
		nearest_neigh = this->list->find_nearest(curve,nearest_neigh,nearest_dist,distance);
		return nearest_neigh;						//Return the real nearest
	}
	else{											//Othwerwise
		return NULL;								//Return null
	}
}





//----------------------------HASHTABLE FUNCTIONS---------------------------------------


template <typename Type_Function, typename Type>		//Create the HashTable
HashTable<Type_Function,Type>::HashTable(const int k_vect,const int n,int(*hash_function)(const Type_Function &,const std::vector<int> &,int,int,std::vector<double> **,double *)):k_vec_(k_vect),buckets(n),Hash_Function(hash_function){
	this->T = new Bucket<Type>*[n];						//Create Array of n Buckets
	for(int i=0;i<n;i++){
		this->T[i] = new Bucket<Type>();				//Initialize each bucket
	}
	if(this->k_vec_ > 0){										//If k_vec > 0, we have LSH Hash_function
		this->t = (double *) malloc(sizeof(double)*k_vec_);		//othewise classic hash function
		this->v = new std::vector<double> * [k_vec_];			//Initialize the v and t parameters for function
		for(int i=0;i<k_vec_;i++){
			this->v[i] = new std::vector<double>();				//We have to create k_vec vectors v,t
			do{
				this->t[i] = Uniform_Generator(0,1);
			}while(t[i]<0 || t[i]>=W);
			this->r.push_back( MIN + (rand() / (RAND_MAX + 1.0))*(MAX-MIN+1));
		}
	}
}


template <typename Type_Function, typename Type>		//Delete the HashTable
HashTable<Type_Function,Type>::~HashTable(){
	for(int i=0;i<this->buckets;i++){
		delete this->T[i];								//Delete each bucket
	}
	delete[] this->T;									//Delete the array of buckets
	if(this->k_vec_ > 0){								//If we have k_vec > 0, delete and the others structure v,t
		free(this->t);
		for(int i =0;i<this->k_vec_;i++){
			delete this->v[i];
		}
		delete[] this->v;
	}
}



template <typename Type_Function, typename Type>		//This function call the dynamic hash_function and
int HashTable<Type_Function,Type>::Hash(Type * x){		//return the hash_value
	if(this->k_vec_ == 0){								//If we use classic_function
		unsigned int size = x->Get_GridCurve().size();
		if(size > this->r.size()){						
			unsigned int rsize = this->r.size();
			for(unsigned int i=0;i<size-rsize;i++){
				this->r.push_back( MIN + (rand() / (RAND_MAX + 1.0))*(MAX-MIN+1));	//Create more ri
			}
		}
	}
	//Call the hash_function and return the number of bucket
	return (*this->Hash_Function)(x->Get_GridCurve(),this->r,this->buckets,this->k_vec_,this->v,this->t);
}



template <typename Type_Function, typename Type>
int HashTable<Type_Function,Type>::Hash_Insert(Type * x){		//Insert a new object to HT
	int bucket = this->Hash(x);									//Get the number of bucket
	if(bucket >= this->buckets){								//Check if all OK
		cerr << "Fail hash function: Index = " << bucket << endl;
		return -1;
	}
	return this->T[bucket]->Bucket_Insert(x);					//Insert the object to the bucket

}




template <typename Type_Function, typename Type>				//Search for the nearest neighbors
List<Type> * HashTable<Type_Function,Type>::Hash_Search(Type * x,bool *flag){
	int bucket = this->Hash(x);									//Find bucket where we have to search
	if(bucket >= this->buckets){
		cerr << "Fail hash function: Index = " << bucket << endl;
		exit(1);
	}
	return this->T[bucket]->Bucket_Search(x,flag);				//Return the list of nearest neighbors

}



template <typename Type_Function, typename Type>				//Clear up some structures
void HashTable<Type_Function,Type>::Clear_up(){
	for(int i=0;i<this->buckets;i++){
		this->T[i]->Clear_up();
	}
}




template <typename Type_Function, typename Type>	//Search for the LSH nearest and true nearest neighbors
Type * HashTable<Type_Function,Type>::Check_all(Type *curve,Type *neigh,long double *neigh_dist,bool *cond,double R,std::vector<char *> *r_near,Type *nearest_neigh,long double *nearest_dist,long double (*distance)(const std::vector< std::vector<double> > &,const std::vector< std::vector<double> > &)){
	int bucket = this->Hash(curve);					//Get bucket number where LSH nearest could be found.
	if(bucket >= this->buckets || bucket < 0){
		cerr << "Fail hash function: Index = " << bucket << endl;
		exit(1);
	}
	bool x = false;
	for(int i=0;i<this->buckets;i++){			//Search all the buckets for true nearest neighbor
		long double near_dist;
		Type * near_neigh = NULL;
		if(i == bucket){						//And the LSH bucket for both of them
			near_neigh = this->T[i]->find_nearest_min(curve,neigh,neigh_dist,cond,R,r_near,near_neigh,&near_dist,distance);
		}
		else{
			near_neigh = this->T[i]->find_nearest(curve,near_neigh,&near_dist,distance);
		}
		if(near_neigh != NULL){					//If we find a neighbor
			if(!x){								//If we don't have already initialized the nearest
				x = true;						//Set flag true and initialize the nearest
				*nearest_dist = near_dist;
				nearest_neigh = near_neigh; 
			}
			else if(*nearest_dist > near_dist){		//Othwerwise compare the distance
				*nearest_dist = near_dist;
				nearest_neigh = near_neigh;
			}
		}
	}
	return nearest_neigh;						//Return the nearest neighbor
}