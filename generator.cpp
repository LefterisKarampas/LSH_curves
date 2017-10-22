#include <stdlib.h>
#include <math.h>
#include <limits>
#include <iostream>
#include <time.h>

using namespace std;

double Uniform_Generator (double mu, double sigma)
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
