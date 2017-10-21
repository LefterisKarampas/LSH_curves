#include <stdlib.h>
#include <math.h>
#include <limits>
#include <iostream>
#include <time.h>

using namespace std;

double randn (double mu, double sigma)
{
	double U, V, S, mult;
	static double X, Y;
	static int call = 0;

	if (call == 1)
	{
		call = !call;
		return (mu + sigma * (double) Y);
	}

	do
	{
		U = -1 + ((double) rand () / RAND_MAX) * 2;
		V = -1 + ((double) rand () / RAND_MAX) * 2;
		S = pow (U, 2) + pow (V,2);
	}
	while (S >= 1 || S == 0);

	mult = sqrt ((-2 * log (S)) / S);
	X = U * mult;
	Y = V * mult;

	call = !call;

	return (mu + sigma * (double) X);
}

int main(void){
	srand(time(NULL));
	double d = 0.125;
	double x;
	for(int i=0;i<100;i++){
		do{
			x = randn(0,1);
		}while(x<0 || x>=d);
		cout << x << endl;
	}
}
