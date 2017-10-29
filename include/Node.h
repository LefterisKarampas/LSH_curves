#ifndef _NODE_H_
#define _NODE_H_

#include <iostream>

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
	void Search_Clear();
};


#endif