#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <unistd.h>
#include <vector>
#include <stdlib.h>
#include "LSH_Curve.h"
#include "LSH_Curve.cpp"
#include <string.h>
#include "Curve.h"
#include "List.h"
#include "Distance.h"
#include "Distance.cpp"

inline bool comparisonFunc(const char *c1, const char *c2)
{
    return strcmp(c1, c2) < 0;
}




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
			}
			else if(c == 1){
				int points = atoi(str);
				if(points == 0){
					cerr << "Failed in query file parsing" << endl;
					exit(1);
				}
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
	long double (*distance)(const std::vector<N> &,const std::vector<N> &);
	if(!strcmp(distance_function,"DFT")){
		distance = &(DFT<N>);
	}
	else{
		distance = &(DTW<N>);
	}
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

	//Read query file for search nearest neighbors
	while ( getline (myfile,line) ){
		T * curve = NULL;
		char * str;
		char *dup = strdup(line.c_str());
		str = strtok (dup,DELIMETER);
		int c = 0;
		char * id;
		while (str != NULL){
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
			}
			else if(c == 1){
				int points = atoi(str);
				if(points == 0){
					cerr << "Failed in query file parsing" << endl;
					exit(1);
				}
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

		//Search for nearest neighboor
		if(curve != NULL){
			//Flag for existing LSH neighbor with the same grid_curve
			bool find = false;
			//R_near neighbors
			std::vector<char *> *r_near;
			r_near = new std::vector<char *>();
			//Array of list which contains the neighbors
			List< Curve<T,N> > ** x;
			x = new List< Curve<T,N> > *[L];
			//Condition for each LSH HT if have a same grid_curve neighbor
			bool condition[L];
			//LSH nearesth neighbor and LSH min distnace
			Curve<T,N> * min = NULL;
			long double min_distance;
			//True nearest neighbor and true min distnace
			Curve<T,N> *nearest_neigh = NULL;
			long double  nearest_dist;
			//Temp LSH neighbor and LSH distance from first HT
			Curve<T,N> *neigh = NULL;
			long double  neigh_dist;
			
			Curve<T,N> my_curve(curve,NULL,id);
			//Search the LSH_structure for neighbors
			for(i=0;i<L;i++){
				condition[i] = false;
				//In the first HT,find the real nearest neighbor and check for LSH nearest
				if(i == 0){
					nearest_neigh = LSH[i]->Check_all(curve,id,neigh,&neigh_dist,condition+i,R,r_near,nearest_neigh,&nearest_dist,distance);
					x[i] = NULL;
				}
				//In the others check only for the LSH nearest neighbor
				else{
				 	x[i] = LSH[i]->LSH_Search(curve,id,condition+i);
				 }
				 //If there is at least a LSH neighbor with the same Grid_curve
				 if(condition[i]){
				 	if(min == NULL){
				 		if(i == 0){
				 			min = neigh;
				 			min_distance = neigh_dist;
				 		}
				 		else{
					 		if(R > 0){
					 			min = x[i]->find_min(my_curve,&min_distance,distance,R,r_near);
					 		}
					 		else{
					 			min = x[i]->find_min(my_curve,&min_distance,distance);
					 		}
					 	}
				 		find = true;	//Flag for LSH neighbor with the same Grid_Curve ON
				 	}
				 	else{
				 		Curve<T,N> * min_t = NULL;
						long double min_distance_t;
						if(R > 0)
							min_t = x[i]->find_min(my_curve,&min_distance_t,distance,R,r_near);
						else
							min_t = x[i]->find_min(my_curve,&min_distance_t,distance);
						if(min_distance > min_distance_t){
							min = min_t;
							min_distance = min_distance_t;
						}
				 	}
				 }
			}

			//If we don't find a neighbor curve with the same Grid_Curve, search the buckets for neighbor
			if(find == false){
				for(i=0;i<L;i++){
					if(i == 0 && neigh){
						min = neigh;
						min_distance = neigh_dist;
						continue;
					}
					if(x[i] == NULL){
						continue;
					}
					if(min == NULL){
				 		if(R > 0){
				 			min = x[i]->find_min(my_curve,&min_distance,distance,R,r_near);
				 		}
				 		else{
				 			min = x[i]->find_min(my_curve,&min_distance,distance);
				 		}
				 	}
				 	else{
				 		Curve<T,N> * min_t = NULL;
						long double min_distance_t;
						if(R > 0){
				 			min_t = x[i]->find_min(my_curve,&min_distance_t,distance,R,r_near);
				 		}
				 		else{
				 			min_t = x[i]->find_min(my_curve,&min_distance_t,distance);
				 		}
						if(min_distance > min_distance_t){
							min = min_t;
							min_distance = min_distance_t;
						}
				 	}
				}
			}

			//Write the results in output file
			out_file << "Query: " << id << endl;
			out_file << "DistanceFunction: " << distance_function << endl;
			out_file << "HashFunction: " << hash_function << endl;
			out_file << "FoundGridCurve: " << (find>0?"True":"False") << endl;
			out_file << "LSH Nearest neighbor: " << (min != NULL?min->GetId():"None") << endl;
			out_file << "True Nearest neighbor: " << (nearest_neigh != NULL?nearest_neigh->GetId():"Problem") << endl;
			out_file << "distanceLSH: " << min_distance << endl;
			out_file << "distanceTrue: " << nearest_dist << endl;
			if(R > 0){
				//Sort lexicography the R-nearest neighbors 
				std::sort (r_near->begin(), r_near->end(),comparisonFunc); 
				//Remove the duplicate
				r_near->erase( unique( r_near->begin(), r_near->end() ), r_near->end() );
				out_file << "R-near neighbors: " << endl;
				for(unsigned int j=0;j<r_near->size();j++){
					out_file << (*r_near)[j] << endl;
				}
			}

			//Delete the structure which we use for searching
			delete r_near;
			out_file << endl;
			for(int i=0;i<L;i++){
				if(x[i] != NULL){
					if(condition[i] == false)
						x[i]->Set_NULL();
					else{
						x[i]->Search_Clear();
					}
				}
				delete x[i];
			}
			delete[] x;
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

