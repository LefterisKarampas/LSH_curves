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
	delete this->value;
	if(this->next != NULL){
		delete this->next;
	}
}

template<typename Type>
int Node<Type>::Set_next(Node<Type> * next){
	this->next = next;
}


template <typename  Type>
Node<Type> * Node<Type>::GetNext(){
	return this->next;
};

template <typename Type>
Type * Node<Type>::GetValue(){
	return this->value;
};

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
	return 0;
};


template<typename Type>
Type * List<Type>::List_Search(Type * v){
	Node<Type> * temp;
	if(this->head != NULL){
		temp = this->head;
	}
	else{
		return NULL;
	}
	while(temp){
		Type *x = temp->GetValue();
		if(x->Compare_GridCurve(v)){
			return x;
		}
		temp = temp->GetNext();
	}
	return NULL;
};
