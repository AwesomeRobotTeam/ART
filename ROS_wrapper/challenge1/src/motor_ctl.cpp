#include "motor_ctl.hpp"

///< Speed flag: 0 = Normal, 1 = speed up 1, 2 = speed up 2
static int sf = 0;

static time_t start = 0, end = 0;

void wrtMotor( challenge1::Motor &mot, uint16_t l_c, uint16_t l_ac, uint16_t r_c, uint16_t r_ac)
{
	mot.l_clk = l_c;
	mot.l_aclk = l_ac;
	mot.r_clk = r_c;
	mot.r_aclk = r_ac;
}

void optMotor( challenge1::Motor &mot, toward t)
{
	uint16_t low = LOW_SP, high = HIGH_SP;
	if( sf != 0){
		if( ret_dur() > SPTIME)
			sf = 0;
		else if( sf == 1){
			low += SPEEDUP1;
			high += SPEEDUP1;
		}
		else if( sf == 2){
			low += SPEEDUP2;
			high += SPEEDUP2;
		}
	}

	ROS_DEBUG("speed up flag = %d", sf);

	ROS_DEBUG("operateMotor(): toward = %s", toward_enumstr[t]);
	switch( t){
		case( front0):
			wrtMotor( mot, low, 0, low * SP_TOR, 0);
			break;
		case( front1):
			wrtMotor( mot, high, 0, high * SP_TOR, 0);
			break;
		case( behind0):
			wrtMotor( mot, 0, low, 0, low * SP_TOR);
			break;
		case( behind1):
			wrtMotor( mot, 0, high, 0, high * SP_TOR);
			break;
		case( right0):
			wrtMotor( mot, 0, low, low * SP_TOR, 0);
			break;
		case( right1):
			wrtMotor( mot, 0, high, high * SP_TOR, 0);
			break;
		case( left0):
			wrtMotor( mot, low, 0, 0, low);
			break;
		case( left1):
			wrtMotor( mot, high, 0, 0, high * SP_TOR);
			break;
		case( stop):
			wrtMotor( mot, 0, 0, 0, 0);
			break;
		/*case( right2):
			wrtMotor( mot, high , 0, 0, 0);
			break;
		case( left2):
			wrtMotor( mot, 0, 0, high * SP_TOR, 0);
			break;*/
		default:
			break;
	}
}

int ret_sf( void)
{
	return sf;
}

void speed_flag( int i)
{
	sf = i;
	start = get_time();
	end		= get_time();
}

double ret_dur()
{
	return diff_time( start, (end = get_time()));
}
