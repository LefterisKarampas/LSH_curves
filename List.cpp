#include <iostream>
#include "List.h"
using namespace std;


//Node Functions
template<typename Type>
Node<Type>::Node(Type * x):value(x){
	this->next = NULL;
};

template<typename Type>
Node<Type>::~Node(){
	if(this->value != NULL)
		delete this->value;
	if(this->next != NULL){
		delete this->next;
	}
}

template<typename Type>
int Node<Type>::Set_next(Node<Type> * next){
	this->next = next;
	return 0;
}


template <typename  Type>
Node<Type> * Node<Type>::GetNext(){
	return this->next;
};

template <typename Type>
Type * Node<Type>::GetValue(){
	return this->value;
};

template <typename Type>
void Node<Type>::Clear_up(){
	this->value->Clear_up();
};

template <typename Type>
void Node<Type>::Search_Clear(){
	this->value = NULL;
}


//List Functions

template<typename Type>
List<Type>::List(){
	this->head = NULL;
	this->count = 0;
};

template<typename Type>
List<Type>::~List(){
	if(this->head != NULL){
		delete this->head;
	}
}

template<typename Type>
int List<Type>::List_Insert(Type * v){
	Node<Type> *k = new Node<Type>(v);
	if(this->head != NULL){
		k->Set_next(this->head);
	}
	this->head = k;
	this->count++;
	return 0;
};


template<typename Type>
List<Type> * List<Type>::List_Search(Type * v,bool *flag){
	Node<Type> * temp;
	if(this->head != NULL){
		temp = this->head;
	}
	else{
		return NULL;
	}
	List<Type> * result;
	result = new List<Type>();
	while(temp){
		Type *x = temp->GetValue();
		if(x->Compare_GridCurve(v)){
			if(*flag == false){
				*flag = true;
			}
			result->List_Insert(x);
		}
		temp = temp->GetNext();
	}
	if(*flag == false){
		result->head = this->head;
	}
	return result;
};

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

template<typename Type>
void List<Type>::Search_Clear(){
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

template<typename Type>
Type * List<Type>::find_min(Type curve,long double *min_distance,
	long double (*distance)(const  std::vector< std::vector<double> > &,const std::vector< std::vector<double> > &)){
	Node<Type> *temp;
	temp = this->head;
	Type *x;
	x = temp->GetValue();
	(*min_distance) = (*distance)(x->GetCurve(),curve.GetCurve());
	long double temp_min;
	Type *temp_x;
	temp = temp->GetNext();
	while(temp){
		temp_x = temp->GetValue();
		temp_min = (*distance)(temp_x->GetCurve(),curve.GetCurve());
		if(*min_distance > temp_min){
			x = temp_x;
			*min_distance = temp_min;
		}
		temp = temp->GetNext(); 
	}
	return x;
}

template<typename Type>
Type * List<Type>::find_min(Type curve,long double *min_distance,
	long double (*distance)(const  std::vector< std::vector<double> > &,const std::vector< std::vector<double> > &),double R,std::vector<char *> *r_near){
	Node<Type> *temp;
	temp = this->head;
	Type *x;
	x = temp->GetValue();
	(*min_distance) = (*distance)(x->GetCurve(),curve.GetCurve());
	if(*min_distance < R){
		r_near->push_back(x->GetId());
	} 
	long double temp_min;
	Type *temp_x;
	temp = temp->GetNext();
	while(temp != NULL){
		temp_x = temp->GetValue();
		temp_min = (*distance)(temp_x->GetCurve(),curve.GetCurve());
		if(temp_min < R){
			r_near->push_back(temp_x->GetId());
		} 
		if(*min_distance > temp_min){
			x = temp_x;
			*min_distance = temp_min;
		}
		temp = temp->GetNext(); 
	}
	return x;
}


