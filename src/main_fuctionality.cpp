#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <unistd.h>
#include <vector>
#include <stdlib.h>
#include "../include/LSH_Curve.h"
#include "LSH_Curve.cpp"
#include <string.h>
#include "../include/Curve.h"
#include "../include/List.h"
#include "../include/Distance.h"
#include "Distance.cpp"
#include <ctime>


#define DELIMETER "\t"

extern int k,k_vec,L;
static int d = 2;
extern bool stats;
extern char *hash_function;
extern char *distance_function;
extern int n;
static int num_points = 10;
static int buckets;


inline bool comparisonFunc(const char *c1, const char *c2)
{
    return strcmp(c1, c2) < 0;
}


//Reconstruct the LSH structure
template <typename T,typename N>
LSH_Curve<T,N,Curve<T,N> > ** reconstruct(T **Array_of_Curves,char **Array_of_Id,int (*function)(const N &,const std::vector<int> &,int,int,std::vector<double> **,double *)){
	LSH_Curve<T,N,Curve<T,N> > ** LSH = new LSH_Curve<T,N,Curve<T,N> > *[L];
	for(int i=0;i<L;i++){
		LSH[i] = new LSH_Curve<T,N,Curve<T,N> >(k,d,k_vec,num_points,buckets,i,function);
	}
	//Each curve, inserts it to LSH structure
	for(int i=0;i<n;i++){
		if(Array_of_Curves[i] != NULL){
			for(int j=0;j<L;j++){
				LSH[j]->LSH_Insert(Array_of_Curves[i],Array_of_Id[i]);
			}
		}
	}
	//Return the new LSH Structure
	return LSH;
}


//Read Curves and insert them to LSH structure
template <typename T,typename N>
LSH_Curve<T,N,Curve<T,N> > ** read_curves(char * path,int (*function)(const N &,const std::vector<int> &,int,int,std::vector<double> **,double *),char ***Array_of_Id,T *** Array_of_Curves){
	string line;
	ifstream myfile (path);
	//Check if the dataset file exists, othewise exit the program
	if (!myfile.is_open()){
		cerr << "Unable to open dataset file";
		exit(1);
	}

	//Count the lines of the file
	char * command;
	char buff[32];
	command = (char *) malloc(strlen(path)+strlen("wc -l  | cut -d ' ' -f1")+2);
	sprintf(command, "wc -l %s | cut -d ' ' -f1",path);
	FILE *in;
	if(!(in = popen(command, "r"))){
		exit(1);
	}
	n = 100;
	while(fgets(buff, sizeof(buff), in)!=NULL){
		n = atoi(buff);
	}
	pclose(in);
	free(command);

	//Create the LSH structure and the arrays to store the id and the curve for future reconstruction
	n = n+1;
	LSH_Curve<T,N,Curve<T,N> > ** LSH = new LSH_Curve<T,N,Curve<T,N> > *[L];
	(*Array_of_Id) = (char **)malloc(sizeof(char *)*n);
	if(*Array_of_Id == NULL){
		cerr << "Fail malloc" << endl;
		exit(1);
	}
	(*Array_of_Curves) = new T*[n];
	if(*Array_of_Curves == NULL){
		cerr << "Fail malloc" << endl;
		exit(1);
	}


	int counter_array = 0;
	buckets = (n>100?n/4:n);	
	bool cond = false;
	int push_backs = 0;
	int loop = 0;

	//Read the curves
	while ( getline (myfile,line) ){			//Get each line from file
		T * curve;
		char * str;
		int points;
		char *dup = strdup(line.c_str());
		str = strtok (dup,DELIMETER);
		int c = 0;
		char * id;

		//Split the line and get each field
		while (str != NULL){
			if(loop == 0 && c == 0 && !strcmp(str,"@dimension")){	//If we the first line has info for dimension
				cond = true;										//Flag on
			}
			else if(loop == 0 && c == 1 && cond){					//If flag on and we are on the second filed 
				d = atoi(str);										//of the first line, get info for dimension
				cout << "We have dimension: " << d << endl;
			}
			else if(c == 0){										//Else if first field has the id info
				curve = new T();
				int len = strlen(str) +1;
				id = (char *)malloc(sizeof(char)*len);
				sprintf(id,"%s",str);
				id[len-1] = '\0';
			}
			else if(c == 1){										//The second field has the number of curve points
				points = atoi(str);
				if(points == 0){
					cerr << "Failed in query file parsing" << endl;
					exit(1);
				}
			}
			else{													//And all the others fields have the points
				N v;
				double x[d];
				//Depending on the dimension read d-dimension point
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
				//Create the point
				for(int counter=0;counter<d;counter++){
					v.push_back(x[counter]);
				}

				//If we read the first point its time to create the LSH Structure
				if(push_backs == 0){
					for(int i=0;i<L;i++){
						LSH[i] = new LSH_Curve<T,N,Curve<T,N> >(k,d,k_vec,num_points,buckets,i,function);
					}
				}
				//Push the new point to the curve
				curve->push_back(v);
				push_backs++;
			}
			//Get the next field
			str = strtok (NULL, DELIMETER);
			c++;
		}

		//If we read all the points of the curve, we insert it to the LSH structure
		if(push_backs >0 && c > 0){
			(*Array_of_Curves)[counter_array] = curve;	//Store curve info for future reconstruction
			(*Array_of_Id)[counter_array] = id;			//Store id info for future reconstruction
			counter_array++;
			for(int i=0;i<L;i++){
				LSH[i]->LSH_Insert(curve,id);			//Insert the curve in LSH
			}
		}
		loop++;
		free(dup);
		free(str);
	}

	//If we have fewer curve than excpected, set the arrays NULL to the remaining positions
	for(int i=counter_array;i<n;i++){
		(*Array_of_Id)[i] = NULL;
		(*Array_of_Curves)[i] = NULL;
	}

	//Close file and return the LSH structure
	myfile.close();
	return LSH;
}




//Search for the nearest neighboors
template <typename T,typename N>
LSH_Curve<T,N,Curve<T,N> > ** search_curves(char *query_file,char * output_file,LSH_Curve<T,N,Curve<T,N> > **LSH,char **Array_of_Id,T **Array_of_Curves,int (*function)(const N &,const std::vector<int> &,int,int,std::vector<double> **,double *)){
	string line;

	//Select the distance function
	long double (*distance)(const std::vector<N> &,const std::vector<N> &);
	if(!strcmp(distance_function,"DFT")){
		distance = &(DFT<N>);
	}
	else{
		distance = &(DTW<N>);
	}

	//Open the output file to append the results
	ofstream out_file (output_file,std::ofstream::out | std::ofstream::app);
	if (!out_file.is_open()){
		cerr << "Unable to open dataset file";
		exit(1);
	}


	//Open the query file for reading the curves 
	ifstream myfile (query_file);
	if (!myfile.is_open()){
		cout << "Unable to open file";
		exit(1);
	}

	int i = 0;
	bool cond = false;
	double R = 0;

	//Read query file for search nearest neighbors
	while ( getline (myfile,line) ){
		T * curve = NULL;
		char * str;
		char *dup = strdup(line.c_str());
		str = strtok (dup,DELIMETER);
		int c = 0;
		char * id;

		while (str != NULL){
			if(i == 0 && c == 0 && !strcmp(str,"R:")){			//In the first line maybe exists info for the radius
				cond = true;									//Set the flag on
			}
			else if(i == 0 && c == 1 && cond){					//IF we have flag on and we are in the first line 
				R = atof(str);									//Initialize the radius
				cout << "We have R: " << R << endl;
			}
			else if(c == 0){									//Otherwise start read the curve
				curve = new T();								//The first field has the id 
				int len = strlen(str) +1;
				id = (char *)malloc(sizeof(char)*len);
				sprintf(id,"%s",str);
				id[len-1] = '\0';
			}
			else if(c == 1){									//The second field has the number of points
				int points = atoi(str);
				if(points == 0){
					cerr << "Failed in query file parsing" << endl;
					exit(1);
				}
			}
			else{
				N v;
				double x[d];
				//Depending on the dimension read d-dimension point
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
				//Push the new point to the curve
				curve->push_back(v);
			}
			str = strtok (NULL, DELIMETER);
			c++;
		}

		if(curve == NULL){
			free(dup);
			i++;
			free(str);
			continue;
		}
		long double minDistanceLSH;
		long double maxDistanceLSH;
		long double avgDistanceLSH = 0;
		double tLSHmin;
		double tLSHmax;
		double tLSHavg = 0;
		int loop = 0;
		int found_curves = 0;
		//Search for nearest neighboor
		do{

			if(loop > 0){					//Statistics mode, for each loop reconstruct the LSH
				for(int j=0;j<L;j++){		//Delete the old LSH
					delete LSH[j];
				}
				delete[] LSH;
				LSH = reconstruct(Array_of_Curves,Array_of_Id,function);	//Create the new LSH structure
			}

			clock_t begin = clock();		//Search time start
			//Flag for existing LSH neighbor with the same grid_curve
			bool find = false;	
			//R_near neighbors
			std::vector<char *> *r_near;
			r_near = new std::vector<char *>();
			//Array of list which contains the neighbors
			List< Curve<T,N> > ** x;
			x = new List< Curve<T,N> > *[L];
			//Condition for each LSH HT if found a same grid_curve neighbor
			bool condition[L];
			//LSH nearest neighbor and LSH min distnace
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
			for(int j=0;j<L;j++){
				condition[j] = false;
				//In the first LSH HT in the case that doesn't have statistics,find the real nearest neighbor and check for LSH nearest
				if(j == 0 && !stats){
					nearest_neigh = LSH[j]->Check_all(curve,id,neigh,&neigh_dist,condition+j,R,r_near,nearest_neigh,&nearest_dist,distance);
					x[j] = NULL;
				}
				//In the others check only for the LSH nearest neighbor
				else{
				 	x[j] = LSH[j]->LSH_Search(curve,id,condition+j);
				 }

				 //If there is at least a LSH neighbor with the same Grid_curve
				 if(condition[j]){
				 	if(min == NULL){
				 		if(j == 0){
				 			min = neigh;
				 			min_distance = neigh_dist;
				 		}
				 		else{
					 		if(R > 0){
					 			min = x[j]->find_min(my_curve,&min_distance,distance,R,r_near);
					 		}
					 		else{
					 			min = x[j]->find_min(my_curve,&min_distance,distance);
					 		}
					 	}
				 		find = true;	//Flag for LSH neighbor with the same Grid_Curve ON
				 	}
				 	else{
				 		Curve<T,N> * min_t = NULL;
						long double min_distance_t;
						if(!stats && R > 0)
							min_t = x[j]->find_min(my_curve,&min_distance_t,distance,R,r_near);
						else
							min_t = x[j]->find_min(my_curve,&min_distance_t,distance);
						if(min_distance > min_distance_t){
							min = min_t;
							min_distance = min_distance_t;
						}
				 	}
				 }
			}

			//If we don't find a neighbor with the same Grid_Curve, search the buckets for neighbor
			if(find == false){
				for(int j=0;j<L;j++){
					if(!stats && j == 0 && neigh != NULL){			//If stats flag off, then we have already check the first HT
						min = neigh;								//and if we get nearest from there, initialize the min with him
						min_distance = neigh_dist;
						continue;
					}
					if(x[j] == NULL){								//Each time we don't find any neighbor in the bucket
						continue;									//Continue to the next LSH HT
					}
					//If we don't have initilized already the min
					if(min == NULL){
				 		if(!stats && R > 0){
				 			min = x[j]->find_min(my_curve,&min_distance,distance,R,r_near);		//Check for LSH nearest and for r_near neighbors
				 		}
				 		else{
				 			min = x[j]->find_min(my_curve,&min_distance,distance);				//Check only for LSH nearest
				 		}
				 	}
				 	else{
				 		Curve<T,N> * min_t = NULL;
						long double min_distance_t;
						if(!stats && R > 0){
				 			min_t = x[j]->find_min(my_curve,&min_distance_t,distance,R,r_near);	//Check for LSH nearest and for r_near neighbors
				 		}
				 		else{
				 			min_t = x[j]->find_min(my_curve,&min_distance_t,distance);			//Check only for LSH nearest
				 		}
						if(min_distance > min_distance_t){
							min = min_t;
							min_distance = min_distance_t;
						}
				 	}
				}
			}
			clock_t end = clock();			//Search time end


			if(stats == false){
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
			}
			else if(min!= NULL){
				found_curves++;
				//Check for the min and max distance
				double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
				//First initialization
				if(tLSHavg == 0){
					tLSHmax = elapsed_secs;
					tLSHmin = elapsed_secs;
					minDistanceLSH = min_distance;
					maxDistanceLSH = min_distance;
				}
				//Compare with the old min and max
				else{
					if(elapsed_secs > tLSHmax){
						tLSHmax = elapsed_secs;
					}
					else if(elapsed_secs < tLSHmin){
						tLSHmin = elapsed_secs;
					}
					if(minDistanceLSH > min_distance){
						minDistanceLSH = min_distance;
					}
					else if(maxDistanceLSH < min_distance){
						maxDistanceLSH = min_distance;
					}
				}
				tLSHavg +=elapsed_secs;
				avgDistanceLSH +=min_distance;
			}
			//Delete the structure which we use for searching
			delete r_near;
			for(int j=0;j<L;j++){
				if(x[j] != NULL){
					if(condition[j] == false)
						x[j]->Set_NULL();
					else{
						x[j]->Search_Clear();
					}
				}
				delete x[j];
			}
			delete[] x;
			loop++;
		}while(stats && loop<100);			//If we want the statistics, do 100 loops. Otherwise exit!

		//Find the real nearest neighbor with exhaustive search and print the results to the output file
		if(stats){
			clock_t begin = clock();
			long double distanceTrue;
			for(int j=0;j<n;j++){
				long double temp_distance;
				if(Array_of_Curves[j] != NULL){
					temp_distance = (*distance)(*curve,*Array_of_Curves[j]);
					if(j == 0){
						distanceTrue = temp_distance;
					}
					else if(distanceTrue > temp_distance){
						distanceTrue = temp_distance;
					}
				}
			}
			clock_t end = clock();
			cout << "DistanceTrue: " << distanceTrue << endl;
			cout << "minDistanceLSH: " << minDistanceLSH << endl;
			double tTrue = double(end - begin) / CLOCKS_PER_SEC;
			out_file << "Query: " << id << endl;
			out_file << "DistanceFunction: "  << distance_function << endl;
			out_file << "HashFunction: " << hash_function << endl;
			out_file << "|minDistanceLSH – distanceTrue|: " << (double)fabs(minDistanceLSH - distanceTrue) <<endl; 
			out_file << "|maxDistanceLSH – distanceTrue|: " << (double)fabs(maxDistanceLSH - distanceTrue) <<endl;
			out_file << "|avgDistanceLSH – distanceTrue|: " << (double)fabs((double)avgDistanceLSH/found_curves - distanceTrue) <<endl;
			out_file << "tLSHmin: " << tLSHmin <<endl;
			out_file << "tLSHmax: " << tLSHmax <<endl;
			out_file << "tLSHavg: " << tLSHavg/loop <<endl;
			out_file << "tTrue: " << tTrue <<endl;  
		}

		//Clear the memory which we used for this curve and for the reading from the file
		delete curve;
		free(id);
		out_file << endl;
		free(dup);
		i++;
		free(str);
	}

	//Close files and return the LSH structure for other searches in case that reconstructed it
	out_file.close();
	myfile.close();
	return LSH;
}

