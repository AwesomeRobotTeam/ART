#include <stdio.h>
#include <stdlib.h>
#define OFFSET { 2., 3., 4.}	//cm

double* cam2arm(double* cam, int num)
{
	double* arm 	 = ( double*) malloc( sizeof( double) * num);
	//double* offset = ( double*) malloc( sizeof( double) * num);

	double offset[3] = OFFSET;

	for( int i = 0; i < num; i++)
	{
		arm[ i] = cam[ i] + offset[ i];
	}

	//free( offset);

	return arm;
}
