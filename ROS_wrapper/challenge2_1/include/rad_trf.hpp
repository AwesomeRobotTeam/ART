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

inline double
ang2rad( double ang)
{
	return ang * ( M_PI / 180);
}

//transfer radian to stepper steps
inline int16_t
rad2step( double rad)
{
	return ( int16_t) round( rad / RAD_PER_STEP);
}

inline double
step2rad( int16_t step)
{
	return ( double) step * RAD_PER_STEP;
}

//transfer radian to servo angular
inline int16_t
rad2servo( double rad)
{
	return (int16_t) round( rad2ang( rad)) + 90;
}

inline int16_t
servo2rad( int16_t servo)
{
	return ang2rad( (double)( servo - 90));
}

#endif
