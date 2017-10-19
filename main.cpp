#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "grid_curves.h"

using namespace std;

int main(int argc,char *argv[]){
	bool end_flag = false;
	char * input_file, *distance_function, *hash_function, *output_file, *query_file;
	int k = 2;
	int L = 3;
	bool stats = false;
	for(int i = 1; i <argc;i+=2){
		if(!strcmp(argv[i],"-d")){
			if(i+1 < argc){
				int m = sizeof(argv[i+1]);
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
				int m = sizeof(argv[i+1]);
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
				int m = sizeof(argv[i+1]);
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
				int m = sizeof(argv[i+1]);
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
				int m = sizeof(argv[i+1]);
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
	
	std::vector<double *> v = std::vector<double *>();
	Grid_curve x(v);

	return 0;
}