#ifndef _MAIN_FUN_H_
#define _MAIN_FUN_H_


template <typename T,typename N>
LSH_Curve<T,N,Curve<T,N> > ** read_curves(char * ,int (*function)(const N &,const std::vector<int> &,int,int,std::vector<double> **,double *),char ***,T *** );

template <typename T,typename N>
LSH_Curve<T,N,Curve<T,N> > ** search_curves(char *,char * ,LSH_Curve<T,N,Curve<T,N> > **,char **,T **,int (*function)(const N &,const std::vector<int> &,int,int,std::vector<double> **,double *));

#endif