#include "../include/LSH_Curve.h"
#include "../include/HashFunctions.h"
#include "../include/HashTable.h"
#include "../include/Curve.h"
#include "../include/Grid.h"
#include "HashTable.cpp"
#include "Curve.cpp"
#include "Grid.cpp"



template <typename T,typename N,typename C>
LSH_Curve<T,N,C>::LSH_Curve(int k,int dim,int k_vect,int num_points,int buckets,int id,int(*hash_function)(const N &,const std::vector<int> &,int,int,std::vector<double> **,double *)){
	this->k = k;
	this->id = id;
	this->G = new Grid<T,N>*[k];									//Create a array of k Grids
	for(int i =0;i<k;i++){
		this->G[i] = new Grid<T,N>(dim,num_points);					//Initialize each Grid
	}
	this->HT = new HashTable<N,C>(k_vect,buckets,hash_function);		//Create the LSH HashTable 
}



template <typename T,typename N,typename C>
LSH_Curve<T,N,C>::~LSH_Curve(){
	for(int i =0;i<this->k;i++){
		delete this->G[i];											//Delete each Grid
	}
	delete[] this->G;												//Delete the array of Grids
	delete this->HT;												//Delete the LSH HashTable
}



template <typename T,typename N,typename C>
N * LSH_Curve<T,N,C>::Create_GridCurve(T *v){						//Get a curve and return the grid_curve
	N *Grid_Concat;
	for(int i =0;i<this->k;i++){									//For each grid
		if(i==0){
			Grid_Concat = this->G[i]->Create_GridCurve(*v);			//get the grid_curve
		}
		else{
			N *temp = this->G[i]->Create_GridCurve(*v);				//get the grid_curve
			unsigned int size = Grid_Concat->size();
			bool cond = false;
			for(unsigned int i=0;i<temp->size();i++){				//Concat the grid_curves without have 2 consecutive points
				if(cond || (((*Grid_Concat)[size-1]) != (*temp)[i])){
					cond = true;
					Grid_Concat->push_back((*temp)[i]);
				}
			}
			delete temp;
		}
	}
	return Grid_Concat;
}


template <typename T,typename N,typename C>
int LSH_Curve<T,N,C>::LSH_Insert(T * v,char *id){					//Insert a curve in the LSH HashTable
	N* Grid_Concat = Create_GridCurve(v);							//First get the grid_curve
	C * curve;
	curve = new C(v,Grid_Concat,id); 								//Create a Curve object that contains the Curve info
	return this->HT->Hash_Insert(curve);							//THen insert the new object to HT
}



template <typename T,typename N,typename C>
List<C> * LSH_Curve<T,N,C>::LSH_Search(T * v,char *id,bool * flag){	 	//Search and return a List of neighbor Curves
	N* Grid_Concat = Create_GridCurve(v);								//Create a grid_curve
	C * curve;
	curve = new C(v,Grid_Concat,id); 								
	List<C> * result = this->HT->Hash_Search(curve,flag);				//Search into HT
	delete curve;														//Clear the used memory space to avoid memory leaks
	return result;														//return the List
}




template <typename T,typename N,typename C>								//Search for LSH nearest and true nearest neighbor
C * LSH_Curve<T,N,C>::Check_all(T *v,char *id,C * neigh,long double *neigh_dist,bool *cond,double R,std::vector<char *> *r_near,C *nearest_neigh,long double *nearest_dist,long double (*distance)(const T&,const T &)){
	N * Grid_Concat = Create_GridCurve(v);								//Create a grid_curve
	C * curve;
	curve = new C(v,Grid_Concat,id);									//Create a Curve object that contains the Curve info
	nearest_neigh = this->HT->Check_all(curve,neigh,neigh_dist,cond,R,r_near,nearest_neigh,nearest_dist,distance);
	delete curve;														//Clear the used memory space to avoid memory leaks
	return nearest_neigh;												//Return the true nearest
}
