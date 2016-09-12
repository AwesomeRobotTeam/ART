#ifndef RAD_TRF_HPP
#define RAD_TRF_HPP

#include <math.h>

/*PI = 3200 steps*/
#define PI2STEPS 			3200
#define RAD_PER_STEP	( 1/ PI2STEPS)


//transfer radian to angular
inline double
rad2ang( double rad)
{
	return rad * ( 180 / M_PI);
}

//transfer radian to stepper steps
inline int
rad2step( double rad)
{
	return ( int) round( rad * RAD_PER_STEP);
}

//transfer radian to servo angular
inline int
rad2servo( double rad)
{
	return (int) round( rad2ang( rad));
}
#endif
