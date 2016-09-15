#include "arm_ctl.hpp"

#define BASE	0
#define LOW		1
#define UP		2
#define WRIST 3
#define CLIP	4

void arm_ctl( challenge2_1::Arm &msg, double x, double y, double z)
{
	double* rad = coord2armrad( x, y, z, obj[ LOW].length, obj[ UP].length, obj[ WRIST].length);


	printf("arm_ctl rad x = %lf, y = %lf, z = %lf", rad2ang( rad[0]), rad2ang( rad[1]), rad2ang( rad[2]));
	
///< (+) = clockwise (-) = anti-clockwise
	msg.baseSteps = rad2step( rad[ BASE])	- obj[BASE].cur_motor.stepper_now;
	obj[BASE].cur_motor.stepper_now +=  msg.baseSteps;

	msg.lowerSteps = rad2step( rad[ LOW]) + obj[ LOW].cur_motor.stepper_now;
	msg.lowerSteps *= -1;///< because clockwise would be anti on real arm
	obj[ LOW].cur_motor.stepper_now +=  msg.lowerSteps;

	msg.upperSteps = rad2step( rad[ UP]) + obj[ UP].cur_motor.stepper_now;
	msg.upperSteps *= -1;///< because clockwise would be anti on real arm
	obj[ UP].cur_motor.stepper_now += msg.upperSteps;

	obj[ WRIST].cur_motor.servo_now = ( msg.wristAngle = rad2servo( rad[WRIST]));

	ROS_INFO("\nNow: Base %d, Lower %d, Upper %d, Wrist %d\n",
		obj[BASE].cur_motor.stepper_now, obj[ LOW].cur_motor.stepper_now,
			obj[ UP].cur_motor.stepper_now, obj[ WRIST].cur_motor.servo_now);

//	arm_stepper_exit( step2rad( obj[ BASE].cur_motor.stepper_now), step2rad( obj[ LOW].cur_motor.stepper_now), step2rad( obj[ UP].cur_motor.stepper_now), 0, M_PI, -M_PI/4, M_PI * 3/4, -M_PI * 3/4, M_PI /2);

	//arm_servo_exit( obj[ WRIST].cur_motor.servo_now, obj[ Clip].cur_motor.servo_now);
}
