#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define THETA 0
#define PHI		1

#define BASE	0
#define LOW		1
#define UP		2
#define WRIST 3

/*PI = 3200 steps*/
#define PI2STEPS 			3200
#define RAD_PER_STEP	( 1/ PI2STEPS)
/**
 *		^ +z
 *		|
 *		|			+y
 *	 	*------>
 *	 /
 *	/ +x
 * v
 *
 */

//squared
static inline
double sq( double num)
{
	return num*num;
}

//Lower and Upper
double*
bend2rad( double x, double y, double a, double b)
{
	double trgRad = atan( y/x);//unit is Radian not angular
	double trgLen = hypot( x, y);// compute triangle hypotenus

	double* rad = (double*) malloc( sizeof( double) * 2);

	rad[ THETA] 	= acos(( sq(a) + sq( trgLen) - sq(b))/(2 * a * trgLen)) + trgRad;
	rad[ PHI] 		= acos(( sq(a) + sq(b) - sq( trgLen))/(2 * a * b)) - M_PI;

	return rad;
}

//transfer coordinate to radian
double*
coord2armrad( double x, double y , double z, double a, double b, double c)
{
	double* ret = (double*) malloc( sizeof( double) * 4);
	
	ret[ BASE] = atan( y/x);

	double* tmp = bend2rad( hypot( x, y) - c, z, a, b);

	ret[ LOW]   = tmp[ 0];
	ret[ UP]    = tmp[ 1];
	ret[ WRIST] = -( ret[LOW] + ret[UP]);
	
	free( tmp);

	return ret;
}


//transfer radian to stepper steps
inline int
rad2step( double rad)
{
	return ( int) round( rad * RAD_PER_STEP);
}

//transfer radian to angular
inline double
rad2ang( double rad)
{
	return rad * ( 180 / M_PI);
}
