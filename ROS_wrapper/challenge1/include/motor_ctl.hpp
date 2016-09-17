#include "challenge1/Motor.h"
#include "_time.h"

#define HIGH_SP 120
#define LOW_SP  50

#define SPTIME	 3
#define SPEEDUP1 50
#define SPEEDUP2 80

typedef enum tow{ front0, behind0, right0, left0, front1, behind1, right1, left1, stop, right2, left2} toward;

/**
 * We can also use #define str(x) #x,
 *	but i guest it isn't worked for function parameter
 */
static const char *toward_enumstr[] = { "front0", "behind0", "right0", "left0", "front1", "behind1", "right1", "left1", "stop", "right2", "left2"};


void wrtMotor( challenge1::Motor &mot, uint16_t l_c, uint16_t l_ac, uint16_t r_c, uint16_t r_ac);

void optMotor( challenge1::Motor &mot, toward t);

int ret_sf( void);

void speed_flag( int i);

double ret_dur( void);
