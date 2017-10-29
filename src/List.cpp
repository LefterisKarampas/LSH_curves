#include <iostream>
#include <vector>
#include "../include/List.h"
#include "Node.cpp"
using namespace std;


//---------------------------List Functions------------------------------

template<typename Type>
List<Type>::List(){							//Initialize the list
	this->head = NULL;
	this->count = 0;
}


template<typename Type>
List<Type>::~List(){						//Destroy the list
	if(this->head != NULL){
		delete this->head;					//Delete the head
	}
}

template<typename Type>					
int List<Type>::List_Insert(Type * v){		//Insert a new node to list		
	Node<Type> *k = new Node<Type>(v);		//Create a node
	if(this->head != NULL){					//If list is not empty,set head as next to the new node
		k->Set_next(this->head);
	}
	this->head = k;							//Set the new node as head
	this->count++;
	return 0;
}



template<typename Type>						//Search for nearest Curves
List<Type> * List<Type>::List_Search(Type * v,bool *flag){
	Node<Type> * temp;							
	if(this->head != NULL){					//If list is empty,return NULL
		temp = this->head;
	}
	else{
		return NULL;
	}
	List<Type> * result;
	result = new List<Type>();
	while(temp){							//For each list's node
		Type *x = temp->GetValue();
		if(x->Compare_GridCurve(v)){		//Check if the curves have the same grid_curves
			if(*flag == false){
				*flag = true;				//If find at least one we will return only
			}								//curves with the same grid_curve
			result->List_Insert(x);			
		}
		temp = temp->GetNext();
	}
	if(*flag == false){						//If we don't find any same grid_curve 
		result->head = this->head;			//Return all the list
	}
	return result;							//Return the list
}



template<typename Type>
void List<Type>::Clear_up(){				
	if(this->head != NULL){				
		Node<Type> *temp;	
		temp = this->head;
		while(temp){
			temp->Clear_up();
			temp = temp->GetNext();
		}
	}
}



template<typename Type>
void List<Type>::Set_NULL(){
	this->head = NULL;
}



template<typename Type>					//Remove the list with the result but only the node
void List<Type>::Search_Clear(){		//not the info 
	Node<Type> *temp;
	Node<Type> *k;
	temp = this->head;
	this->head = NULL;
	while(temp){
		temp->Search_Clear();
		k = temp->GetNext();
		temp->Set_next(NULL);
		delete temp;
		temp = k;
	}
	this->head = NULL;
}



template<typename Type>				//Find the LSH nearest neighbor
Type * List<Type>::find_min(Type curve,long double *min_distance,
	long double (*distance)(const  std::vector< std::vector<double> > &,const std::vector< std::vector<double> > &)){
	Node<Type> *temp;
	temp = this->head;
	Type *x;
	x = temp->GetValue();
	(*min_distance) = (*distance)(x->GetCurve(),curve.GetCurve());		//Compute the distance for first curve
	long double temp_min;
	Type *temp_x;
	temp = temp->GetNext();
	while(temp){														//For each curve in list
		temp_x = temp->GetValue();
		temp_min = (*distance)(temp_x->GetCurve(),curve.GetCurve());	//compute the distance 
		if(*min_distance > temp_min){									//compare the distances
			x = temp_x;
			*min_distance = temp_min;
		}
		temp = temp->GetNext(); 
	}	
	return x;															//Return the nearest neighbors
}



template<typename Type>				//Find the LSH nearest neighbor and the r_near neighbors
Type * List<Type>::find_min(Type curve,long double *min_distance,
	long double (*distance)(const  std::vector< std::vector<double> > &,const std::vector< std::vector<double> > &),double R,std::vector<char *> *r_near){
	Node<Type> *temp;
	temp = this->head;
	Type *x;
	x = temp->GetValue();
	(*min_distance) = (*distance)(x->GetCurve(),curve.GetCurve());		//Compute the distance for first curve
	if(*min_distance < R){
		r_near->push_back(x->GetId());
	} 
	long double temp_min;
	Type *temp_x;
	temp = temp->GetNext();
	while(temp != NULL){												//For each curve in list
		temp_x = temp->GetValue();
		temp_min = (*distance)(temp_x->GetCurve(),curve.GetCurve());	//compute the distance 
		if(temp_min < R){												//Check if curve is r_near neighbor
			r_near->push_back(temp_x->GetId());
		} 
		if(*min_distance > temp_min){									//compare the distances
			x = temp_x;
			*min_distance = temp_min;
		}
		temp = temp->GetNext(); 
	}
	return x;
}




template<typename Type>				//Find true nearest,LSH nearest neighbor and r_near neighbors
Type * List<Type>::find_nearest_min(Type *curve,Type *neigh,long double *neigh_dist,bool *cond,double R,std::vector<char *> *r_near,Type *nearest_neigh,long double *nearest_dist,long double(*distance)(const std::vector< std::vector<double> >&,const std::vector< std::vector<double> >&)){
	Node<Type> *temp;
	temp = this->head;
	bool condition = false;
	while(temp){
		Type *x;
		x = temp->GetValue();
		long double temp_dist;
		temp_dist = (*distance)(x->GetCurve(),curve->GetCurve());
		if(x->Compare_GridCurve(curve)){
			*cond = true;
			if(neigh == NULL){
				neigh = x;
				*neigh_dist = temp_dist;
			}
			else if(*neigh_dist > temp_dist){
				*neigh_dist = temp_dist;
				neigh = x;
			}
		}
		if(temp_dist < R){
			r_near->push_back(x->GetId());
		}
		if(!condition){
			nearest_neigh = x;
			*nearest_dist = temp_dist;
			condition = true;
		}
		else if(*nearest_dist > temp_dist){
			nearest_neigh = x;
			*nearest_dist = temp_dist;
		}
		temp = temp->GetNext();
	}
	return nearest_neigh;
}



template<typename Type>					//Find true nearest
Type * List<Type>::find_nearest(Type *curve,Type *nearest_neigh,long double *nearest_dist,long double(*distance)(const std::vector< std::vector<double> >&,const std::vector< std::vector<double> >&)){
	Node<Type> *temp;
	temp = this->head;
	Type *x;
	while(temp){
		x = temp->GetValue();
		long double temp_dist;
		temp_dist = (*distance)(x->GetCurve(),curve->GetCurve());
		if(nearest_neigh == NULL){
			nearest_neigh = x;
			*nearest_dist = temp_dist;
		}
		else if(*nearest_dist > temp_dist){
			nearest_neigh = x;
			*nearest_dist = temp_dist;
		}
		temp = temp->GetNext();
	}
	return nearest_neigh;
}