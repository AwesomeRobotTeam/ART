#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define THETA 0
#define PHI		1

#define BASE	0
#define LOW		1
#define UP		2
#define WRIST 3
#define CLIP	4

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
bend2rad( double x, double y, double a, double b);

//transfer coordinate to radian
double*
coord2armrad( double x, double y , double z, double a, double b, double c);
