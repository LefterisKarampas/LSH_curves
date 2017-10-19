#include "LSH_Curve.h"
#include "Grid.h"
#include "Grid.cpp"
#include "HashFunctions.h"
#include "HashTable.cpp"

template <typename T,typename N>
LSH_Curve<T,N>::LSH_Curve(int k,int dim,int k_vec,int num_points,int buckets,int(*hash_function)(const N &,int)){
	this->k = k;
	this->G = new Grid<T>*[k];
	for(int i =0;i<k;i++){
		this->G[i] = new Grid<T>(dim,num_points);
	}
	this->HT = new HashTable<N>(k_vec,buckets,hash_function);
}

template <typename T,typename N>
LSH_Curve<T,N>::~LSH_Curve(){
	for(int i =0;i<this->k;i++){
		delete this->G[i];
	}
	delete[] this->G;
	delete this->HT;
}

template <typename T,typename N>
int LSH_Curve<T,N>::LSH_Insert(const N & v){
	N x;
	for(int i =0;i<this->k;i++){
		N temp = this->G[i]->Create_GridCurve(v);
		x.insert( x.end(), temp.begin(), temp.end() );
	}
	for (std::vector<double>::const_iterator i = x.begin(); i != x.end(); ++i)
	    std::cout << *i << ' ';
	return this->HT->Hash_Insert(x);
}

int main(void){
	LSH_Curve< double,std::vector<double> > x(3,2,1,1,4,&classic_function);
	std::vector<double> v;
	v.push_back(3.3);
	v.push_back(4.4);
	if(!x.LSH_Insert(v)){
		cout << "Insertion Completed!" << endl;
	}
}