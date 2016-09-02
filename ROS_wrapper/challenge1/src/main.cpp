#include "ros_api.h"

#include "motor.cpp"
#include "challenge1/Ultrasonic.h"
#include "challenge1/trafficLight.h"

#define NODE_NAME Center

void trafColor( const challenge1::trafficLight::ConstPtr &msg);
challenge1::Motor mot;

int main( int argc, char** argv)
{
	///< Init
	r_init( NODE_NAME);
	r_hdlr( hdl);
	ros::Rate loop_rate( 10);

	///< Publisher and Subscriber
	r_newPub( motpub, hdl, challenge1::Motor, Arduino_Motor, 1000);
	
	r_newSub( trafsub, hdl, traf_color, 1000, trafColor);

	///< Msg init
	challenge1::trafficLight traf;
	traf.color = 0;

	///< while loop
	while( ros::ok())
	{
		motpub.publish( mot);
		ros::spinOnce();
		loop_rate.sleep();
	}
}

void trafColor( const challenge1::trafficLight::ConstPtr &msg)
{
	ROS_DEBUG("color = %d", (int) msg->color);
	if( (int) msg->color == 0)
		optMotor( mot, front0);
	else if( (int) msg->color == 1)
		optMotor( mot, stop);
}
