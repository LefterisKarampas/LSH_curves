#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include "../include/LSH_Curve.h"
#include "../include/Curve.h"
#include "../include/HashFunctions.h"
#include "../include/main_fuctionality.h"
#include "main_fuctionality.cpp"


using namespace std;

int k = 2;
int L = 3;
int k_vec = 0;
bool stats = false;
int n = 0;
char * input_file = NULL;
char *output_file = NULL;
char *query_file = NULL;
char *distance_function = NULL;
char *hash_function = NULL;


int read_args(int argc,char **argv){
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
	return 0;
}

int main(int argc,char *argv[]){
	//Read args for starting the construction
	read_args(argc,argv);
	string buf;
	if(!input_file){
		cout << "Give the path of dataset file" << endl;
		cin >> buf;
		input_file = (char *)malloc(buf.size()+1);
		strcpy(input_file,buf.c_str());
	}
	if(!distance_function){
		do{
		cout << "Give the distance_function {DFT,DTW}" << endl;
		cin >> buf;
		}while(buf.compare("DFT") && buf.compare("DTW"));
		distance_function = (char *)malloc(buf.size()+1);
		strcpy(distance_function,buf.c_str());
	}
	if(!hash_function){
		do{
		cout << "Give the hash_function {classic,probabilistic}" << endl;
		cin >> buf;
		}while(buf.compare("classic") && buf.compare("probabilistic") );
		hash_function = (char *)malloc(buf.size()+1);
		strcpy(hash_function,buf.c_str());
	}

	//Select the hash_function which we will use
	int (*function)(const std::vector<double> &,const std::vector<int> &,int,int,std::vector<double> **,double *);
	if(!strcmp(hash_function,"classic")){
		function = &classic_function;
	}
	else{
		function = &probabilistic;
		k_vec = 3;
	}

	//Store the id and the curve in arrays to use them if we will have created the statistics
	char ** Array_of_Id;
	std::vector< std::vector<double> > ** Array_of_Curves;
	//Read curves and constuct the LSH for Curves
	LSH_Curve< std::vector< std::vector<double> > ,std::vector<double>, Curve<  std::vector< std::vector<double> >, std::vector<double> > > **LSH;
	LSH = read_curves< std::vector< std::vector<double> >,std::vector<double> >(input_file,function,&Array_of_Id,&Array_of_Curves);

	//Read query file and output file if these were not given it on command line
	if(!query_file){
		cout << "Give the path of query_file" << endl;
		cin >> buf;
		query_file = (char *)malloc(buf.size()+1);
		strcpy(query_file,buf.c_str());
	}
	if(!output_file){
		cout << "Give the path of output_file" << endl;
		cin >> buf;
		output_file = (char *)malloc(buf.size()+1);
		strcpy(output_file,buf.c_str());
	}

	//Create a output file if not exists or truncate it
	ofstream out_file (output_file,std::ofstream::out | std::ofstream::trunc);
	if(out_file.is_open()){
		out_file.close();
	}

	//Now its time to search for the nearest curves
	LSH = search_curves(query_file,output_file,LSH,Array_of_Id,Array_of_Curves,function);

	//Reapet the search for different query files
	while(1){
		char f;
		do{
			cout << "If you want to continue search with other query file press 'r', else press 'f'" << endl;
			cin >> f;
		}while(f != 'f' && f != 'r');
		if(f == 'f'){
			break;
		}
		if(query_file){
			free(query_file);
		}	
		cout << "Give the path of query_file" << endl;
		cin >> buf;
		query_file = (char *)malloc(buf.size()+1);
		strcpy(query_file,buf.c_str());
		LSH = search_curves(query_file,output_file,LSH,Array_of_Id,Array_of_Curves,function);
	}

	//Destroy the structures before exit to avoid memory leaks
	for(int i=0;i<n;i++){
		if(Array_of_Id[i] != NULL){
			free(Array_of_Id[i]);
		}
		if(Array_of_Curves[i] != NULL){
			delete Array_of_Curves[i];
		}
	}
	free(Array_of_Id);
	delete[] Array_of_Curves;

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