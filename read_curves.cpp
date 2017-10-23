#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <stdlib.h>
#include "LSH_Curve.h"
#include "LSH_Curve.cpp"
#include <string.h>
#include "Curve.h"


extern int k,k_vec,L;
static int d = 2;
extern char *hash_function;
extern char *distance_function;

#define DELIMETER "\t"

template <typename T,typename N>
LSH_Curve<T,N,Curve<T,N> > ** read_curves(char * path,int (*function)(const N &,const std::vector<int> &,int,int,std::vector<double> **,double *)){
	string line;
	ifstream myfile (path);
	if (!myfile.is_open()){
		cerr << "Unable to open dataset file";
		exit(1);
	}
	char * command;
	char buff[32];
	command = (char *) malloc(strlen(path)+strlen("wc -l  | cut -d ' ' -f1")+2);
	sprintf(command, "wc -l %s | cut -d ' ' -f1",path);
	FILE *in;
	if(!(in = popen(command, "r"))){
		exit(1);
	}
	int n = 100;
	while(fgets(buff, sizeof(buff), in)!=NULL){
		n = atoi(buff);
	}
	pclose(in);
	free(command);
	LSH_Curve<T,N,Curve<T,N> > ** LSH = new LSH_Curve<T,N,Curve<T,N> > *[L];

	int i = 0;
	int num_points = 10;
	int buckets = (n>100?n/4:n);
	bool cond = false;
	int push_backs = 0;

	while ( getline (myfile,line) ){
		T * curve;
		char * str;
		char *dup = strdup(line.c_str());
		str = strtok (dup,DELIMETER);
		int c = 0;
		char * id;
		while (str != NULL)
		{
			N v;
			if(i == 0 && c == 0 && !strcmp(str,"@dimension")){
				cond = true;
			}
			else if(i == 0 && c == 1 && cond){
				d = atoi(str);
				cout << "We have dimension: " << d << endl;
			}
			else if(c == 0){
				curve = new T();
				id = (char *)malloc(strlen(str)+1);
				strcpy(id,str);
				cout << "ID: " << str << endl;
			}
			else if(c == 1){
				cout << "Points: " << atoi(str) << endl;
			}
			else{
				double x[d];
				if(d == 2){
					if(sscanf(str,"(%lf,%lf)",x,x+1) != d){
						cerr << "We expected " <<d<<"-dimensional curve"<<endl;
						exit(1);
					}
				}
				else if(d == 3){
					if(sscanf(str,"(%lf,%lf,%lf)",x,x+1,x+2) != d){
						cerr << "We expected " <<d<<"-dimensional curve"<<endl;
						exit(1);
					}
				}
				else{
					if(sscanf(str,"(%lf,%lf,%lf,%lf)",x,x+1,x+2,x+3) != d){
						cerr << "We expected " <<d<<"-dimensional curve"<<endl;
						exit(1);
					}
				}
				for(int counter=0;counter<d;counter++){
					v.push_back(x[counter]);
				}
				if(push_backs == 0){
					for(i=0;i<L;i++){
						LSH[i] = new LSH_Curve<T,N,Curve<T,N> >(k,d,k_vec,num_points,buckets,i,function);
					}
				}
				curve->push_back(v);
				push_backs++;
			}
			str = strtok (NULL, DELIMETER);
			c++;
		}
		if(push_backs >0){
			for(i=0;i<L;i++){
				LSH[i]->LSH_Insert(curve,id);
			}
		}
		free(dup);
		i++;
		free(str);
	}
	myfile.close();
	return LSH;
}

template <typename T,typename N>
int search_curves(char *query_file,char * output_file,LSH_Curve<T,N,Curve<T,N> > **LSH){
	string line;
	ofstream out_file (output_file,std::ofstream::out | std::ofstream::app);
	if (!out_file.is_open()){
		cerr << "Unable to open dataset file";
		exit(1);
	}
	int i = 0;
	bool cond = false;
	double R = 0;
	ifstream myfile (query_file);
	if (!myfile.is_open()){
		cout << "Unable to open file";
		exit(1);
	}
	while ( getline (myfile,line) ){
		T * curve = NULL;
		char * str;
		char *dup = strdup(line.c_str());
		str = strtok (dup,DELIMETER);
		int c = 0;
		char * id;
		while (str != NULL)
		{
			N v;
			if(i == 0 && c == 0 && !strcmp(str,"R:")){
				cond = true;
			}
			else if(i == 0 && c == 1 && cond){
				R = atof(str);
				cout << "We have R: " << R << endl;
			}
			else if(c == 0){
				curve = new T();
				id = (char *)malloc(strlen(str)+1);
				strcpy(id,str);
				cout << "ID: " << str << endl;
			}
			else if(c == 1){
				cout << "Points: " << atoi(str) << endl;
			}
			else{
				double x[d];
				if(d == 2){
					if(sscanf(str,"(%lf,%lf)",x,x+1) != d){
						cerr << "We expected " <<d<<"-dimensional curve"<<endl;
						exit(1);
					}
				}
				else if(d == 3){
					if(sscanf(str,"(%lf,%lf,%lf)",x,x+1,x+2) != d){
						cerr << "We expected " <<d<<"-dimensional curve"<<endl;
						exit(1);
					}
				}
				else{
					if(sscanf(str,"(%lf,%lf,%lf,%lf)",x,x+1,x+2,x+3) != d){
						cerr << "We expected " <<d<<"-dimensional curve"<<endl;
						exit(1);
					}
				}
				for(int counter=0;counter<d;counter++){
					v.push_back(x[counter]);
				}
				curve->push_back(v);
			}
			str = strtok (NULL, DELIMETER);
			c++;
		}
		if(curve != NULL){
			Curve<T,N>* x;
			for(i=0;i<L;i++){
				 x = LSH[i]->LSH_Search(curve,id);
			}
			if(x != NULL){
				out_file << "Query: " << id << endl;
				out_file << "DistanceFunction: " << distance_function << endl;
				out_file << "HashFunction: " << hash_function << endl;
				out_file << "FoundGridCurve: " << "{True,False}" << endl;
				out_file << "LSH Nearest neighbor: " << x->GetId() << endl;
				out_file << "True Nearest neighbor: " << x->GetId() << endl;
				out_file << "distanceLSH: " << "0.0" << endl;
				out_file << "distanceTrue: " << "0.0" << endl;
			}
			out_file << endl;
			delete curve;
			free(id);
		}
		free(dup);
		i++;
		free(str);
	}
	out_file.close();
	myfile.close();
	return 0;
}

