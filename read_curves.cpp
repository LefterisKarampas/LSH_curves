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

#define DELIMETER "\t"

template <typename T,typename N>
LSH_Curve<T,N,Curve<T,N> > ** read_curves(char * path,int (*function)(const N &,int)){
	string line;
	ifstream myfile (path);
	if (!myfile.is_open()){
		cout << "Unable to open file";
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
	int d = 2;
	int num_points = 10;
	int buckets = (n>100?n/4:n);
	bool cond = false;
	int push_backs = 0;

	while ( getline (myfile,line) ){
		T * curve = new T();
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
				id = (char *)malloc(strlen(str)+1);
				strcpy(id,str);
				cout << "ID: " << str << endl;
			}
			else if(c == 1){
				cout << "Points: " << atoi(str) << endl;
			}
			else{
				double x[d];
				if(d == 2)
					sscanf(str,"(%lf,%lf)",x,x+1);
				else if(d == 3)
					sscanf(str,"(%lf,%lf,%lf)",x,x+1,x+2);
				else
					sscanf(str,"(%lf,%lf,%lf,%lf)",x,x+1,x+2,x+3);

				for(int counter=0;counter<d;counter++){
					v.push_back(x[counter]);
				}
				if(push_backs == 0){
					for(i=0;i<L;i++){
						LSH[i] = new LSH_Curve<T,N,Curve<T,N> >(k,d,k_vec,num_points,buckets,function);
					}
				}
				curve->push_back(v);
				push_backs++;
				for(i=0;i<L;i++){
					LSH[i]->LSH_Insert(curve,id);
				}
			}
			//printf ("%s\n",str);
			str = strtok (NULL, DELIMETER);
			c++;
		}
		free(dup);
		i++;
	}
	myfile.close();
	return LSH;
}