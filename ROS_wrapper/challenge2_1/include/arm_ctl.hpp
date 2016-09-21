#ifndef ARM_CTL_HPP
#define ARM_CTL_HPP

#include <stdint.h>
#include "object.h"
#include "rad_trf.hpp"
#include "arm.hpp"
#include "challenge2_1/Arm.h"


void arm_ctl( challenge2_1::Arm &msg, double x, double y, double z);

inline void
arm_clip_rot( challenge2_1::Arm &msg, uint16_t angle)
{
	obj[ CLIP].cur_motor.servo_now =( msg.clipAngle = angle);
}

inline void
arm_stepper_exit( int16_t base, int16_t lower, int16_t upper, double baseDlim, double baseUlim, double upperDlim, double upperUlim, double lowerDlim, double lowerUlim)
{
	if( baseDlim <= base && base <= baseUlim && lowerDlim <= lower && lower <= lowerUlim && upperDlim <= upper && upper <= upperUlim)
		return;
	else{
		fprintf( stderr, "Stepper out of range for the robot\n");
		getchar();
		exit( 0);
	}
}

inline void
arm_servo_exit( int16_t wrist, int16_t clip, int16_t wristDlim, int16_t wristUlim, int16_t clipDlim, int16_t clipUlim)
{
	if( wristDlim <= wrist && wrist <= wristUlim && clipDlim <= clip && clip <= clipUlim)
		return;
	else{
		fprintf( stderr, "Motor out of range for the robot\n");
		getchar();
		exit( 0);
	}
}

inline void
zero_arm( challenge2_1::Arm &msg)
{
	msg.baseSteps = (msg.lowerSteps = (msg.upperSteps = 0));
}

inline void
arm_zero( challenge2_1::Arm &msg)
{
	msg.baseSteps = -obj[BASE].cur_motor.stepper_now;
	msg.lowerSteps = obj[ LOW].cur_motor.stepper_now;
	msg.upperSteps = obj[ UP].cur_motor.stepper_now;
	msg.wristAngle = 0;
}
#endif
