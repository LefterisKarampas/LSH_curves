#include <iostream>
#include <vector>
#include "../include/Node.h"


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
