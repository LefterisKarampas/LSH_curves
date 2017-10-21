#ifndef _LIST_H_
#define _LIST_H_

#include <iostream>
#include <vector>

using namespace std;

template <typename Type>
class Node{
	Type * value;
	Node<Type> * next;
public:
	Node(Type * x);
	~Node();
	int Set_next(Node<Type> * next);
	Node<Type> * GetNext();
	Type * GetValue();

	void print(){
		cout << "-> " << this->value << " ";
		if(this->next != NULL)
			this->next->print();
		else
			cout << endl; 
	}
};

template <typename Type>
class List{
	Node<Type> * head;
	int count;
public:
	List();
	~List();
	int List_Insert(Type *);
	Type * List_Search(Type *);
	void print(){
		Node<Type> * temp;
		temp = this->head;
		while(temp != NULL){
			Type x = temp->GetValue();
			std::vector<double>::const_iterator i;
			for (i = x.begin(); i != x.end(); ++i)
				std::cout  << "-> " << *i << " ";
			
			temp = temp->GetNext(); 
		}
		cout << endl;
	}
};

#endif