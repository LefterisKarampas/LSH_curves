#include <iostream>
#include "Distance.h"
#include <vector>
#include <math.h>

using namespace std;

template <typename T>
T my_max(T x, T y){
	return (x>y?x:y);
}

template <typename T>
T my_min(T x, T y){
	return (x>y?y:x);
}


template <typename T>
long double Euclidean_Distance(const std::vector<T> & v1,const std::vector<T> &v2){
 	long double sum = 0;
 	for(int i = 0;i<v1.size();i++){
 		sum += pow((v1[i] - v2[i]),2);
 	}
 	return sqrt(sum);
 };

template <typename T>
long double Frechet_Distance(const std::vector<T> & v1,const std::vector<T> &v2){
	int n = v1.size();
	int m = v2.size();
	long double C[n][m];
	for(int i=0;i<n;i++){
		for(int j=0;j<m;j++){
			if(i == 0 && j == 0){
				C[0][0] = Euclidean_Distance(v1[0],v2[0]);
			}
			else if(i == 0 && j>0){
				C[i][j] = my_max(C[i][j-1],Euclidean_Distance(v1[i],v2[j]));
			}
			else if(j == 0){
				C[i][j] = my_max(C[i-1][j],Euclidean_Distance(v1[i],v2[j]));
			}
			else{
				C[i][j] = my_max(my_min(my_min(C[i-1][j],C[i-1][j-1]),C[i][j-1]),
					Euclidean_Distance(v1[i],v2[j])); 
			}
			cout << C[i][j] << "  ";
		}
		cout << endl;
	}
	return C[n-1][m-1];
}

template <typename T>
long double Dynamic_Time_Wraping(const std::vector<T> &v1,const std::vector<T> &v2){
	int n = v1.size();
	int m = v2.size();
	long double C[n][m];
	for(int i=0;i<n;i++){
		for(int j=0;j<m;j++){
			if(i == 0 && j == 0){
				C[0][0] = Euclidean_Distance(v1[0],v2[0]);
			}
			else if(i == 0 && j>0){
				C[i][j] = C[i][j-1] + Euclidean_Distance(v1[i],v2[j]);
			}
			else if(j == 0){
				C[i][j] = C[i-1][j] + Euclidean_Distance(v1[i],v2[j]);
			}
			else{
				C[i][j] = my_min(my_min(C[i-1][j],C[i-1][j-1]),C[i][j-1])+Euclidean_Distance(v1[i],v2[j]); 
			}
			cout << C[i][j] << "  ";
		}
		cout << endl;
	}
	return C[n-1][m-1];
}
