#include "challenge1/Motor.h"

#define HIGH_SP 30
#define LOW_SP  30

typedef enum tow{ front0, behind0, right0, left0, front1, behind1, right1, left1, stop} toward;

/**
 * We can also use #define str(x) #x,
 *	but i guest it isn't worked for function parameter
 */
static const char *toward_enumstr[] = { "front0", "behind0", "right0", "left0", "front1", "behind1", "right1", "left1", "stop"};


void wrtMotor( challenge1::Motor &mot, uint16_t l_c, uint16_t l_ac, uint16_t r_c, uint16_t r_ac);

void optMotor( challenge1::Motor &mot, toward t);
