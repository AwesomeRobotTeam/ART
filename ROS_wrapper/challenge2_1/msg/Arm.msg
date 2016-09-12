#Arm include: Base, Lowwer Arm, Upper Arm and Clip
#
#			Upper Arm
#			/				\
#	Lowwer Arm	 Wrist --Clip
#			|
#		Base

# > 0 : right, Up
# < 0 : left , Down

# Base: left and right rotation (Stepper)
int16 baseSteps

# Lowwer Arm: Up and down rotation (Stepper)
int16 lowerSteps

# Upper Arm: Up and down rotation (Stepper)
int16 upperSteps

#Servo can only be represented from 0 to 180
# Wrist: Up and down rotation (Servo)
uint16 wristAngle

# Clip: left and right rotation angular (Servo)
uint16 clipAngle
