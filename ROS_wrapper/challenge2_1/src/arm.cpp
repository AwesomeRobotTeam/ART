#include "arm.hpp"
#include "rad_trf.hpp"

#define WRIST_H			 3.872//2.334 3.872
#define WRIST_W			 3.2  //3.872 3.2
#define WRIST_L			 4.2  //2.597 4.2

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
	double trgRad = atan( y/x) >= 0? atan( y/x) : M_PI + atan( y/x);//unit is Radian not angular
	double trgLen = hypot( x, y);// compute triangle hypotenus

	double* rad = (double*) malloc( sizeof( double) * 2);

	rad[ THETA] 	= acos(( sq(a) + sq( trgLen) - sq(b))/(2 * a * trgLen)) + trgRad;

	//printf("%ld")
	rad[ PHI] 		= acos(( sq(a) + sq(b) - sq( trgLen))/(2 * a * b)) - M_PI;

	printf("\nbend2rad(): x = %lf y = %lf a = %lf b = %lf trgRad = %lf rad[ 0] = %lf\n", x, y, a, b, trgRad, rad[0]);
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

double* coord2realarm( double x, double y , double z, double a, double b, double c)
{
	return coord2armrad( x - WRIST_L, y - WRIST_W, z -WRIST_H, a, b, c);
}
