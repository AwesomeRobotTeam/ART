#include "ros_api.h"

#include "motor.cpp"
#include "challenge1/Ultrasonic.h"

#define NODE_NAME Center

int main( int argc, char** argv)
{
	///< Init
	r_init( NODE_NAME);
	r_hdlr( hdl);
	ros::Rate loop_rate( 10);

	///< Publisher and Subscriber
	r_newPub( motpub, hdl, challenge1::Motor, Arduino_Motor, 1000);

	///< Msg init
	challenge1::Motor mot;

	///< while loop
	while( ros::ok())
	{
		optMotor( mot, front0);

		motpub.publish( mot);
		ros::spinOnce();
		loop_rate.sleep();
	}
}
