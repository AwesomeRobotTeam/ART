#include <stdio.h>
#include "arm.hpp"
#include "arm_ctl.hpp"
#include "rad_trf.hpp"

#include "ros_api.h"
#include "challenge2_1/Arm.h"

void manul_assign( challenge2_1::Arm &msg);

int
main( int argc, char** argv)
{
	///< Init
	r_init( Challenge2_1_Commander);
	r_hdlr( hdl);
	ros::Rate loop_rate( 10);

	///< Publisher
	r_newPub( armpub, hdl, challenge2_1::Arm, Arduino_Arm, 1000);
	
	///< Subscriber
	//r_newSub( irtsub, hdl, Arduino_IR_trigger, 1000, btracker);

	challenge2_1::Arm arm;

	while( ros::ok())
	{
		manul_assign( arm);
		armpub.publish( arm);

		ros::spinOnce();
		loop_rate.sleep();
	}
}

void
manul_assign( challenge2_1::Arm &msg)
{
	printf("Please input the 3 dim point\n\n");
	double tmp[3];
	scanf("%lf %lf %lf", tmp, tmp + 1, tmp + 2);

	//double* rad = coord2armrad( tmp[0], tmp[1], tmp[2], BASE_H, LOWER_L, UPPER_L);
	double* rad = coord2armrad( tmp[0], tmp[1], tmp[2], LOWER_L, UPPER_L, WRIST_L);
	printf("main rad x = %lf, y = %lf, z = %lf\n", rad2ang( rad[0]), rad2ang( rad[1]), rad2ang( rad[2]));

	arm_ctl( msg, tmp[0] - UPPER_W - WRIST_W , tmp[1] - WRIST_L, tmp[2]);// + WRIST_H);

	//Real
	//arm_ctl( msg, tmp[0], tmp[1] - XXX, tmp[2] + XXX);

	/*msg.baseSteps  = rad[0];
	msg.lowerSteps = rad[1];
	msg.upperSteps = rad[2];*/
}
