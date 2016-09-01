#include "ros_api.h"
#include "std_msgs/UInt16.h"
#include "rosserial_arduino/Adc.h"

#include <sys/unistd.h>
#include <sys/fcntl.h>

#define UP0 'd'
#define DOWN0 'c'
#define LEFT0 'x'
#define RIGHT0 'v'
#define UP1 'h'
#define DOWN1 'n'
#define LEFT1 'b'
#define RIGHT1 'm'
#define STOP 's'

#define ngetc( c) ( read ( 0, ( c), 1))

void key2ard( rosserial_arduino::Adc &in);
void inadc( rosserial_arduino::Adc &adc, uint16_t a, uint16_t b, uint16_t c , uint16_t d);

int main( int argc, char** argv)
{
	fcntl( 0, F_SETFL, O_NONBLOCK);/*///< make the stdin be nonblocking*/
	r_init( manual_ard);
	r_hdlr( hdl);

	r_newPub( ardpub, hdl, rosserial_arduino::Adc, man_ard, 100);

	ros::Rate loop_rate( 10);

	rosserial_arduino::Adc tmp;

	while( ros::ok())
	{
		key2ard( tmp);

		ardpub.publish( tmp);
		ros::spinOnce();
		loop_rate.sleep();
	}
}

void key2ard( rosserial_arduino::Adc &in)
{
	char* key = ( char*) malloc( sizeof( char) );
	memset( key, '\0', 1);
	
	if( 0 == ngetc( key) || *key == '\n'){
		ROS_INFO("Noting in keyborad");
		return;
	}

	
	ROS_INFO("key = %c", *key);

	switch( *key){
		case( UP0):
			inadc( in, 100, 0, 100, 0);
			break;
		case( UP1):
			inadc( in, 200, 0, 200, 0);
			break;
		case( DOWN0):
			inadc( in, 0, 100, 0, 100);
			break;
		case( DOWN1):
			inadc( in, 0, 200, 0, 200);
			break;
		case( LEFT0):
			inadc( in, 0, 0, 120, 0);
			break;
		case( LEFT1):
			inadc( in, 0, 0, 255, 0);
			break;
		case( RIGHT0):
			inadc( in, 120, 0, 0, 0);
			break;
		case( RIGHT1):
			inadc( in, 255, 0, 0, 0);
			break;
		case( STOP):
			inadc( in, 0, 0, 0, 0);
		default:
			break;
	}
}

void inadc( rosserial_arduino::Adc &adc, uint16_t a, uint16_t b, uint16_t c , uint16_t d)
{
	adc.adc0 = a;// right forward
	adc.adc1 = b;// right behind
	adc.adc2 = c;// left forward
	adc.adc3 = d;// left behind
}
