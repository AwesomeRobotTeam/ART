#include <math.h>

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

double*
coord2rad( double x, double y , double z, double a, double b, double c)
{
	double* ret = malloc( sizeof( double) * 4);
	
	ret[ BASE] = atan( y/x);

	double* tmp = arm2rad( hypot( x, y) - c, z, a, b);

	ret[ LOW]   = tmp[ 0];
	ret[ UP]    = tmp[ 1];
	ret[ WRIST] = -( ret[LOW] + ret[UP]);
	
	free( tmp);

	return ret;
}

//Lower and Upper
double*
arm2rad( double x, double y, double a, double b)
{
	double trgRad = atan( y/x);//unit is Radian not angular
	double trgLen = hypot( x, y);// compute triangle hypotenus

	double* rad = malloc( sizeof( double) * 2);

	rad[ THETA] 	= acos(( sq(a) + sq( trgLen) - sq(b))/(2 * a * trgLen));
	rad[ PHI] 		= acos(( sq(a) + sq(b) - sq( trgLen))/(2 * a * b)) - PI;

	return rad;
}

//squared
static inline
double sq( double num)
{
	return num*num;
}

//radian to stepper steps
inline int
rad2step( double rad)
{
	return ( int) round( rad * RAD_PER_STEP);
}

//
inline double
rad2ang( double rad)
{
	return rad / PI;
}
