#ifndef _LIST_H_
#define _LIST_H_

#include <iostream>
#include <vector>

using namespace std;

template <typename Type>
class Node{
private:
	Type * value;
	Node<Type> * next;
public:
	Node(Type * x);
	~Node();
	int Set_next(Node<Type> * next);
	Node<Type> * GetNext();
	Type * GetValue();
	void Clear_up();


};

template <typename Type>
class List{
private:
	Node<Type> * head;
	int count;
public:
	List();
	~List();
	int List_Insert(Type *);
	Type * List_Search(Type *);
	void Clear_up();
};



#endif