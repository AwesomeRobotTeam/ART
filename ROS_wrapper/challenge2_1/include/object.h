#ifndef OBJECT_H
#define OBJECT_H
/**/
#define ELEVATE 		0		  

/*Arm Structure (unit: CM)*/
#define BASE_H 			13.519
#define LOWER_L			14.294
#define UPPER_L 		12.884
#define WRIST_H			 2.334
#define WRIST_W			 3.872
#define WRIST_L			 2.597	 
#define CLIP_L			 0
#define CLIP_W			 0


typedef enum a{ Base, Lowwer, Upper, Wrist, Clip } ArmStruct;

///< NOTICE below will compile error
//char armStructName[][] = { "baseMtr", "lowwerMrt", "UpperMrt", "wristMrt", "clipMrt"};
char *armStructName[] = { "baseMtr", "lowwerMrt", "UpperMrt", "wristMrt", "clipMrt"};

typedef enum b{ xy, yz} _flat;

typedef struct arm obj_arm;

typedef enum c{ Servo, Stepper} Motor;

struct arm{
	const ArmStruct tag;
	const _flat flat;
	const Motor m_type;

	union u{
		int servo_now;
		int stepper_now;
	} cur_motor;

	double l_coord[3];
	double g_coord[3];

	const float length;
	const float width;
	const float height;
};


obj_arm obj[] = {
/**
 *		v
 *		*---
 */

{ .tag = Base, .flat = xy, .m_type = Stepper, .cur_motor.stepper_now = 0, .l_coord = { 0., 0., 0.}, .g_coord = { 0., 0., ELEVATE}, .length = 0, .width = 0, .height = BASE_H},
/**
 * NOTICE:
 *	(X) l_coord[] = { 0., 0., 0.}
 *	(O) l_coord		= { 0., 0., 0.}
 */

/**
 *				v
 *		*---*---
 */

{ .tag = Lowwer, .flat = yz, .m_type = Stepper, .cur_motor.stepper_now = 0, .l_coord = { 0., 0., BASE_H}, .g_coord = { 0., 0., ELEVATE + BASE_H}, .length = LOWER_L, .width = 0, .length = 0},

/**
 *					 v
 *	 *---*---*---
 */

{ .tag = Upper, .flat = yz, .m_type = Stepper, .cur_motor.stepper_now = 0, .l_coord = { LOWER_L, 0., BASE_H}, .g_coord = { LOWER_L, 0., ELEVATE + BASE_H}, .length = UPPER_L, .width = 0, .height = 0},

/**
 *							 v
 *	 *---*---*---*---*
 *							 |	 |
 *							 *---*
 */

{ .tag = Wrist, .flat = xy, .m_type = Servo, .cur_motor.servo_now = 0, .l_coord = { LOWER_L + UPPER_L, 0., BASE_H}, .g_coord = { LOWER_L + UPPER_L, 0., ELEVATE + BASE_H}, .length = WRIST_L, .width = WRIST_W, .height = WRIST_H},

/**
 *							 
 *	 *---*---*---*---*
 *							 |	 |
 *							 *---*
 *									 ^\
 *										 \
 */

{ .tag = Clip, .flat = xy, .m_type = Servo, .cur_motor.servo_now = 0, .l_coord = {LOWER_L + UPPER_L + WRIST_L, WRIST_W, BASE_H - WRIST_H}, .g_coord = { LOWER_L + UPPER_L + WRIST_L, WRIST_W, ELEVATE + BASE_H}, .length = CLIP_L, .width = CLIP_W, .height = 0}

};
#endif
