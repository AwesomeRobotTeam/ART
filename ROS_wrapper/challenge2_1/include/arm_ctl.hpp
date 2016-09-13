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

#endif
