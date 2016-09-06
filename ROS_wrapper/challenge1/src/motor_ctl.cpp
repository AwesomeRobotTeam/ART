#include "motor_ctl.hpp"

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
			wrtMotor( mot, 40, 0, 40, 0);
			break;
		case( front1):
			wrtMotor( mot, 50, 0, 50, 0);
			break;
		case( behind0):
			wrtMotor( mot, 0, 40, 0, 40);
			break;
		case( behind1):
			wrtMotor( mot, 0, 50, 0, 50);
			break;
		case( right0):
			wrtMotor( mot, 0, 0, 40, 0);
			break;
		case( right1):
			wrtMotor( mot, 0, 0, 50, 0);
			break;
		case( left0):
			wrtMotor( mot, 40, 0, 0, 0);
			break;
		case( left1):
			wrtMotor( mot, 50, 0, 0, 0);
			break;
		case( stop):
			wrtMotor( mot, 0, 0, 0, 0);
		default:
			break;
	}
}
