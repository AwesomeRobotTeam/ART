#include "challenge1/Motor.h"

typedef enum tow{ front0, behind0, right0, left0, front1, behind1, right1, left1, stop} toward;

/**
 * We can also use #define str(x) #x,
 *	but i guest it isn't worked for function parameter
 */
static const char *toward_enumstr[] = { "front0", "behind0", "right0", "left0", "front1", "behind1", "right1", "left1", "stop"};

void wrtMotor( challenge1::Motor &mot, uint16_t l_c, uint16_t l_ac, uint16_t r_c, uint16_t r_ac)
{
	mot.l_clk = l_c;
	mot.l_aclk = l_ac;
	mot.r_clk = r_c;
	mot.r_aclk = r_ac;
}

void optMotor( challenge1::Motor &mot, toward t)
{
	ROS_DEBUG("operateMotor(): toward = %s", toward_enumstr[t]);
	switch( t){
		case( front0):
			wrtMotor( mot, 50, 0, 50, 0);
			break;
		case( front1):
			wrtMotor( mot, 200, 0, 200, 0);
			break;
		case( behind0):
			wrtMotor( mot, 0, 100, 0, 100);
			break;
		case( behind1):
			wrtMotor( mot, 0, 200, 0, 200);
			break;
		case( right0):
			wrtMotor( mot, 0, 0, 120, 0);
			break;
		case( right1):
			wrtMotor( mot, 0, 0, 255, 0);
			break;
		case( left0):
			wrtMotor( mot, 120, 0, 0, 0);
			break;
		case( left1):
			wrtMotor( mot, 255, 0, 0, 0);
			break;
		case( stop):
			wrtMotor( mot, 0, 0, 0, 0);
		default:
			break;
	}
}
