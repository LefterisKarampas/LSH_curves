#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "LSH_Curve.h"
#include "HashFunctions.h"
#include "read_curves.cpp"
#include "Curve.h"

using namespace std;

int k = 2;
int L = 3;
int k_vec = 3;

template <typename T,typename N>
LSH_Curve<T,N,Curve<T,N> > ** read_curves(char * path,int (*function)(const N &,int));

int main(int argc,char *argv[]){
	bool end_flag = false;
	char * input_file = NULL;
	char *distance_function = NULL;
	char *hash_function = NULL;
	char *output_file = NULL;
	char *query_file = NULL;
	bool stats = false;
	for(int i = 1; i <argc;i+=2){
		if(!strcmp(argv[i],"-d")){
			if(i+1 < argc){
				int m = strlen(argv[i+1]);
				input_file = (char *) malloc(m+1);
				strcpy(input_file,argv[i+1]);
				input_file[m] = '\0';
				cout << "Input file: " << input_file << endl;
			}
			else{
				cerr << "Fail args" << endl;
				exit(1);
			}
		}
		else if(!strcmp(argv[i],"-q")){
			if(i+1 < argc){
				int m = strlen(argv[i+1]);
				query_file = (char *) malloc(m+1);
				strcpy(query_file,argv[i+1]);
				query_file[m] = '\0';
				cout << "Query file: " << query_file << endl;
			}
			else{
				cerr << "Fail args" << endl;
				exit(1);
			}
		}
		else if(!strcmp(argv[i],"-k")){
			if(i+1 < argc){
				k = atoi(argv[i+1]);
				if(k <= 0){
					cerr << "Fail k arg" << endl;
					exit(1);
				}
				else{
					cout << "k: " << k << endl;
				}
			}
			else{
				cerr << "Fail args" << endl;
				exit(1);
			}
		}
		else if(!strcmp(argv[i],"-L")){
			if(i+1 < argc){
				L = atoi(argv[i+1]);
				if(L <= 0){
					cerr << "Fail L arg" << endl;
					exit(1);
				}
				else{
					cout << "L: " << L << endl;
				}
			}
			else{
				cerr << "Fail args" << endl;
				exit(1);
			}
		}
		else if(!strcmp(argv[i],"-o")){
			if(i+1 < argc){
				int m = strlen(argv[i+1]);
				output_file = (char *) malloc(m+1);
				strcpy(output_file,argv[i+1]);
				output_file[m] = '\0';
				cout << "Output file: " << output_file << endl;
			}
			else{
				cerr << "Fail args" << endl;
				exit(1);
			}
		}
		else if(!strcmp(argv[i],"-stats")){
			cout << "Stats: true" << endl;
			stats = true;
			i -=1;
		}
		else if(!strcmp(argv[i],"-function")){
			if(i+1 < argc){
				int m = strlen(argv[i+1]);
				distance_function = (char *) malloc(m+1);
				strcpy(distance_function,argv[i+1]);
				distance_function[m] = '\0';
				cout << "Distance function: " << distance_function << endl;
			}
			else{
				cerr << "Fail args" << endl;
				exit(1);
			}

		}
		else if(!strcmp(argv[i],"-hash")){
			if(i+1 < argc){
				int m = strlen(argv[i+1]);
				hash_function = (char *) malloc(m+1);
				strcpy(hash_function,argv[i+1]);
				hash_function[m] = '\0';
				cout << "Hash function: " << hash_function << endl;
			}
			else{
				cerr << "Fail args" << endl;
				exit(1);
			}
		}
		else{
			cerr << "Fail args" << endl;
			exit(1);
		}
	}
	while(!end_flag){
		char key = 'a';
		while(key != 'r' && key != 'f'){
			cout << "If you want to search for another set, press 'r' else press 'f' to finish the program" << endl;
			cin >> key;
		}
		if(key == 'f'){
			end_flag = true;
		}
	}
	
	LSH_Curve< std::vector< std::vector<double> > ,std::vector<double>, Curve<  std::vector< std::vector<double> >, std::vector<double> > > **LSH;
	int (*function)(const std::vector<double> &,int);
	if(!strcmp(hash_function,"classic")){
		function = &classic_function;
	}
	else{
		function = &probabilistic;
	}
	
	LSH = read_curves< std::vector< std::vector<double> >,std::vector<double> >(input_file,function);
	std::vector< std::vector<double> > * y = new std::vector< std::vector<double> >();
	std::vector<double> y1;
	std::vector<double> y2;
	std::vector<double> y3;
	y1.push_back(8.11);
	y1.push_back(3.41);
	y2.push_back(9.13);
	y2.push_back(2.9);
	y3.push_back(0.57);
	y3.push_back(3.14);
	y->push_back(y1);
	y->push_back(y2);
	y->push_back(y3);
	char *id = (char *)malloc(strlen("id1")+1);
	strcpy(id,"id1");
	cout << "---------------" << endl;
	for(int i=0;i<L;i++){
		Curve< std::vector< std::vector<double> >,std::vector<double> >* x = LSH[i]->LSH_Search(y,id);
		if(x != NULL){
			cout << x->GetId() << endl;
		}
	}
	for(int i=0;i<L;i++){
		delete LSH[i];
	}
	delete[] LSH;
	if(hash_function)
		free(hash_function);
	if(input_file)
		free(input_file);
	if(query_file)	
		free(query_file);
	if(distance_function)
		free(distance_function);
	if(output_file)
		free(output_file);
	return 0;
}