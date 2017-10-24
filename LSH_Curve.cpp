#include "LSH_Curve.h"
#include "HashFunctions.h"
#include "HashTable.h"
#include "Curve.h"
#include "Grid.h"
#include "HashTable.cpp"
#include "Curve.cpp"
#include "Grid.cpp"

template <typename T,typename N,typename C>
LSH_Curve<T,N,C>::LSH_Curve(int k,int dim,int k_vec,int num_points,int buckets,int id,int(*hash_function)(const N &,const std::vector<int> &,int,int,std::vector<double> **,double *)){
	this->k = k;
	this->id = id;
	this->G = new Grid<T,N>*[k];
	for(int i =0;i<k;i++){
		this->G[i] = new Grid<T,N>(dim,num_points);
	}
	this->HT = new HashTable<N,C>(k_vec,buckets,hash_function);
}

template <typename T,typename N,typename C>
LSH_Curve<T,N,C>::~LSH_Curve(){
	for(int i =0;i<this->k;i++){
		delete this->G[i];
	}
	delete[] this->G;
	if(this->id == 0){
		this->HT->Clear_up();
	}
	delete this->HT;
}

template <typename T,typename N,typename C>
int LSH_Curve<T,N,C>::LSH_Insert(T * v,char *id){
	N *Grid_Concat;
	for(int i =0;i<this->k;i++){
		if(i==0){
			Grid_Concat = this->G[i]->Create_GridCurve(*v);
		}
		else{
			N *temp = this->G[i]->Create_GridCurve(*v);
			unsigned int size = Grid_Concat->size();
			bool cond = false;
			for(unsigned int i=0;i<temp->size();i++){
				if(cond || (((*Grid_Concat)[size-1]) != (*temp)[i])){
					cond = true;
					Grid_Concat->push_back((*temp)[i]);
				}
			}
			delete temp;
		}
	}
	C * curve;
	curve = new C(v,Grid_Concat,id); 
	return this->HT->Hash_Insert(curve);
}

template <typename T,typename N,typename C>
List<C> * LSH_Curve<T,N,C>::LSH_Search(T * v,char *id,bool * flag){
	N *Grid_Concat;
	for(int i =0;i<this->k;i++){
		if(i==0){
			Grid_Concat = this->G[i]->Create_GridCurve(*v);
		}
		else{
			N *temp = this->G[i]->Create_GridCurve(*v);
			unsigned int size = Grid_Concat->size();
			bool cond = false;
			for(unsigned int i=0;i<temp->size();i++){
				if(cond || (((*Grid_Concat)[size-1]) != (*temp)[i])){
					cond = true;
					Grid_Concat->push_back((*temp)[i]);
				}
			}
			delete temp;
		}
	}
	C * curve;
	curve = new C(v,Grid_Concat,id); 
	List<C> * result = this->HT->Hash_Search(curve,flag);
	delete curve;
	return result;
}
