#include <stdio.h>
#include "arm.hpp"
#include "arm_ctl.hpp"
#include "rad_trf.hpp"

#include "ros_api.h"
#include "challenge2_1/Arm.h"
#include "challenge2_1/Ultrasonic.h"

///< ROS init Topic msg
challenge2_1::Ultrasonic usonic;
challenge2_1::Arm arm;

void manul_assign( challenge2_1::Arm &msg);

void closeArot( const challenge2_1::Ultrasonic::ConstPtr &msg);

///< TODO: not measurement
double locate[3] = { 30, 0, 0};

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
	r_newSub( usonicsub, hdl, Arduino_Ultrasonic, 1000, closeArot);


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

	arm_ctl( msg, tmp[0] + UPPER_W - WRIST_W , tmp[1] - WRIST_L, tmp[2]);// + WRIST_H);

	///< assign location
	for( int j = 0; j < 4; j++)
		locate[ j] = tmp[ j];
	
	//Real
	//arm_ctl( msg, tmp[0], tmp[1] - XXX, tmp[2] + XXX);

	/*msg.baseSteps  = rad[0];
	msg.lowerSteps = rad[1];
	msg.upperSteps = rad[2];*/
}

#define TOLERANCE 1 // cm
#define DOORKNOB_DIST 3
#define MOVEGRAD 0.5 //movement graduation

void
closeArot( const challenge2_1::Ultrasonic::ConstPtr &msg)
{
	float diff = msg->u_dst - msg->d_dst;
	
	if( diff > TOLERANCE)
		arm_ctl( arm, locate[0], locate[1], locate[2] + MOVEGRAD);
	else if( diff < -TOLERANCE)
		arm_ctl( arm, locate[0], locate[1], locate[2] - MOVEGRAD);
	else if( msg->u_dst > DOORKNOB_DIST || msg->d_dst > DOORKNOB_DIST)
		arm_ctl( arm, locate[0], locate[1] + MOVEGRAD, locate[2]);
	else{
		zero_arm( arm);
		arm_clip_rot( arm, 0);//because the servo is begin at -90
	}
}
