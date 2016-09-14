#include "arm.hpp"
#include "rad_trf.hpp"

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

//Lower and Upper
double*
bend2rad( double x, double y, double a, double b)
{
	double trgRad = atan( y/x) > 0? atan( y/x) : M_PI + atan( y/x);//unit is Radian not angular
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
	
	///< atan = [ -PI/2 ~ PI/2] map to [ 0 ~ PI ]
	ret[ BASE] = atan( y/x) > 0? atan( y/x) : M_PI + atan( y/x) ;

	double* tmp = bend2rad( hypot( x, y) - c, z, a, b);

	ret[ LOW]   = tmp[ 0];
	ret[ UP]    = tmp[ 1];
	ret[ WRIST] = -( ret[LOW] + ret[UP]);
	
	free( tmp);

	return ret;
}
