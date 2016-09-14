#ifndef RAD_TRF_HPP
#define RAD_TRF_HPP

#include <math.h>

/*PI = 3200 steps*/
#define PI2STEPS 			3200
#define RAD_PER_STEP	( 2 * M_PI / PI2STEPS)


//transfer radian to angular
inline double
rad2ang( double rad)
{
	return rad * ( 180 / M_PI);
}

//transfer radian to stepper steps
inline int16_t
rad2step( double rad)
{
	return ( int16_t) round( rad / RAD_PER_STEP);
}

//transfer radian to servo angular
inline int16_t
rad2servo( double rad)
{
	return (int16_t) round( rad2ang( rad)) + 90;
}
#endif
