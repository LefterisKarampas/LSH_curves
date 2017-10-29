#ifndef _LIST_H_
#define _LIST_H_

#include <iostream>
#include <vector>
#include "Node.h"

using namespace std;

template <typename Type>
class List{
private:
	Node<Type> * head;
	int count;
public:
	List();
	~List();
	int List_Insert(Type *);
	List<Type> * List_Search(Type *,bool *);
	void Clear_up();
	void Set_NULL();
	void Search_Clear();
	Type * find_min(Type ,long double *,
			long double (*distance)(const  std::vector< std::vector<double> > &,const std::vector< std::vector<double> > &),double,std::vector<char *> *);
	Type * find_min(Type ,long double *,
			long double (*distance)(const  std::vector< std::vector<double> > &,const std::vector< std::vector<double> > &));
	Type * find_nearest_min(Type *,Type *,long double *,bool *,double ,std::vector<char *> *,Type *,long double *,long double(*distance)(const std::vector< std::vector<double> >&,const std::vector< std::vector<double> >&));
	Type * find_nearest(Type *,Type *,long double *,long double(*distance)(const std::vector< std::vector<double> >&,const std::vector< std::vector<double> >&));
};



#endif