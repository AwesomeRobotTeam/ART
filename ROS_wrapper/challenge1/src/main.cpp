#include "ros_api.h"

#include "motor_ctl.hpp"
#include "challenge1/Ultrasonic.h"
#include "challenge1/trafficLight.h"
#include "challenge1/IR_trigger.h"

///< ROS init Topic msg
challenge1::Motor mot;
challenge1::IR_trigger irt;
challenge1::trafficLight traf;
challenge1::Ultrasonic usonic;

///< Subscribe Call Back funct
void btracker( const challenge1::IR_trigger::ConstPtr &msg);
void trafColor( const challenge1::trafficLight::ConstPtr &msg);
void avoidance( const challenge1::Ultrasonic::ConstPtr &msg);

int main( int argc, char** argv)
{
	///< Init
	r_init( Center_Node);
	r_hdlr( hdl);
	ros::Rate loop_rate( 10);

	///< Publisher
	r_newPub( motpub, hdl, challenge1::Motor, Arduino_Motor, 1000);
	
	///< Subscriber
	r_newSub( irtsub, hdl, Arduino_IR_trigger, 1000, btracker);
	r_newSub( trafsub, hdl, traf_color, 1000, trafColor);
	r_newSub( usonicsub, hdl, Arduino_Ultrasonic, 1000, avoidance);

	///< Msg init
	//challenge1::trafficLight traf;
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
	if( (int) msg->color == 0){
		optMotor( mot, front0);
		ROS_DEBUG("trafColor do nothing");
	}
	else if( (int) msg->color == 1)
		optMotor( mot, stop);
}

void btracker( const challenge1::IR_trigger::ConstPtr &msg)
{
	/*ROS_DEBUG("IR_trigger: trig_c %d , trig_r %d, trig_l %d",
			( int) msg->trig_l, ( int) msg->trig_c, ( int) msg->trig_r);
	if( ( int) msg->trig_l == 0 && ( int) msg->trig_c == 1 && ( int) msg->trig_r == 0)
		optMotor( mot, front0);
	else if( ( int) msg->trig_l == 1 && ( int) msg->trig_c == 0 && ( int) msg->trig_r == 0)
		optMotor( mot, right0);
	else if( ( int) msg->trig_l == 1 && ( int) msg->trig_c == 1 && ( int) msg->trig_r == 0)
		optMotor( mot, right1);
	else if( ( int) msg->trig_l == 0 && ( int) msg->trig_c == 0 && ( int) msg->trig_r == 1)
		optMotor( mot, left0);
	else if( ( int) msg->trig_l == 0 && ( int) msg->trig_c == 1 && ( int) msg->trig_r == 1)
		optMotor( mot, left1);
	else if( ( int) msg->trig_l == 0 && ( int) msg->trig_c == 0 && ( int) msg->trig_r == 0){
		ROS_DEBUG("black trace with 000");
		optMotor( mot, behind0);
	}
	else if( ( int) msg->trig_l == 1 && ( int) msg->trig_c == 1 && ( int) msg->trig_r == 1)
		ROS_DEBUG("black trace with 111");
	else if( ( int) msg->trig_l == 1 && ( int) msg->trig_c == 0 && ( int) msg->trig_r == 1)
		ROS_DEBUG("black trace with 101");
	else
		ROS_ERROR("Bug for black tracker: trig_c %d , trig_r %d, trig_l %d",
			( int) msg->trig_l, ( int) msg->trig_c, ( int) msg->trig_r);*/
	
	if( ( int) msg->trig_ll == 1)
		optMotor( mot, right1);
	else if( ( int) msg->trig_rr == 1)
		optMotor( mot, left1);
	else if( ( int) msg->trig_l == 1)
		optMotor( mot, right0);
	else if( ( int) msg->trig_r == 1)
		optMotor( mot, left0);
	else if( ( int) msg->trig_c == 1)
		optMotor( mot, front0);
	//else
		//optMotor( mot, front0);
}

void avoidance( const challenge1::Ultrasonic::ConstPtr &msg)
{
	if( 5 < ( int) msg->r_dst && ( int) msg->r_dst < 40)
		optMotor( mot, front1);
	else if( 5 < ( int) msg->l_dst && ( int) msg->l_dst < 40)
		optMotor( mot, front1);
	else if( 5 < ( int) msg->f_dst &&  ( int) msg->f_dst < 30)
		optMotor( mot, stop);
	else if( 5 < ( int) msg->rf_dst &&  ( int) msg->rf_dst < 35)
		optMotor( mot, stop);
	else if(  5 < ( int) msg->lf_dst && ( int) msg->lf_dst < 35)
		optMotor( mot, stop);
	else
		optMotor( mot, front0);
}
