#ifndef OBJECT_H
#define OBJECT_H
#include <stdint.h>

//extern "C" {
/**/
#define ELEVATE 		0		  

/*Arm Structure (unit: CM)*/
#define BASE_H 			13.519
#define LOWER_L			14.294
#define UPPER_L 		12.884
#define UPPER_W 		 1.6
#define WRIST_H			 3.872//2.334 3.872
#define WRIST_W			 3.2  //3.872 3.2
#define WRIST_L			 4.2  //2.597 4.2
#define CLIP_L			 0
#define CLIP_W			 0

#define CLIP_INIT_SERVO 	0
#define CLIP_FINAL_SERVO  180

#define CLIP_INIT_SERVO 	54
#define CLIP_FINAL_SERVO  140

/*#define WRIST*/

typedef enum a{ Base, Lower, Upper, Wrist, Clip } ArmStruct;

///< NOTICE below will compile error
//char armStructName[][] = { "baseMtr", "lowwerMrt", "UpperMrt", "wristMrt", "clipMrt"};
const char *armStructName[] = { "baseMtr", "lowwerMrt", "UpperMrt", "wristMrt", "clipMrt"};

typedef enum b{ xy, yz} _flat;

typedef struct arm obj_arm;

typedef enum c{ Servo, Stepper} Motor;

struct arm{
	const ArmStruct tag;
	const _flat flat;
	const Motor m_type;

	union u{
		int16_t servo_now;
		int16_t stepper_now;
	} cur_motor;

	double l_coord[3];
	double g_coord[3];

	const double length;
	const double width;
	const double height;
};


//obj_arm obj0 = 
/**
 *		|
 *		|
 *		*<
 */

obj_arm obj0 = { tag : Base, flat : xy, m_type : Stepper, cur_motor : { stepper_now : 0}, l_coord : { 0., 0., 0.}, g_coord : { 0., 0., ELEVATE}, length : 0, width : 0, height : BASE_H};
/**
 * NOTICE:
 *	(X) l_coord[] = { 0., 0., 0.}
 *	(O) l_coord		= { 0., 0., 0.}
 */

/**
 *		v
 *		*---
 *		|
 *		|
 *		*
 */

obj_arm obj1 = { tag : Lower, flat : yz, m_type : Stepper, cur_motor : { stepper_now : 0}, l_coord : { 0., 0., BASE_H}, g_coord : { 0., 0., ELEVATE + BASE_H}, length : LOWER_L, width : 0, height : 0};

/**
 *				v
 *	  *---*---
 *		|
 *		|
 *		*
 */

obj_arm obj2 = { tag : Upper, flat : yz, m_type : Stepper, cur_motor : { stepper_now : 0}, l_coord : { LOWER_L, 0., BASE_H}, g_coord : { LOWER_L, 0., ELEVATE + BASE_H}, length : UPPER_L, width : UPPER_W, height : 0};

/**
 *							 *---*
 *							 |	 |
 *	 *---*---*---*---*
 *	 |					 ^
 *	 |					 
 *	 *
 */
/* servo will 0 -> 180  UP to DOWN*/

obj_arm obj3 = { tag : Wrist, flat : xy, m_type : Servo, cur_motor : { .servo_now = 0}, l_coord : { LOWER_L + UPPER_L, UPPER_W, BASE_H}, g_coord : { LOWER_L + UPPER_L, UPPER_W, ELEVATE + BASE_H}, length : WRIST_L, width : WRIST_W, height : WRIST_H};

/**
 *								 \
 *									\v
 *							 *---*
 *							 |	 |	
 *	 *---*---*---*---*
 *	 |					 
 *	 |					 
 *	 *					
 *						
 */
/* servo will 54 -> 140  UP to DOWN*/
obj_arm obj4 = { tag : Clip, flat : xy, m_type : Servo, cur_motor : { servo_now : 54}, l_coord : { LOWER_L + UPPER_L + WRIST_L, UPPER_W + WRIST_W, BASE_H + WRIST_H}, g_coord : { LOWER_L + UPPER_L + WRIST_L, UPPER_W + WRIST_W, ELEVATE + BASE_H}, length : CLIP_L, width : CLIP_W, height : 0};
//}

obj_arm obj[] = { obj0, obj1, obj2, obj3, obj4};

#endif
