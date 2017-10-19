#include "HashTable.h"
#include "List.h"
#include <iostream>
#include <vector>

using namespace std;


int hash1(const std::vector<double> & x){
	return 0;
}

int hash2 (const std::vector<double> & x){
	return 0;
}



int main(void){
	HashTable< std::vector<double> > h1(5,&hash1);
	std::vector<double> v = std::vector<double>();
	v.push_back(2.3);
	v.push_back(3.3);
	h1.Hash(v);
	h1.Hash_Insert(v);
	HashTable< std::vector<double> > h2(5,&hash2);
	h2.Hash(v);
	return 0;
}