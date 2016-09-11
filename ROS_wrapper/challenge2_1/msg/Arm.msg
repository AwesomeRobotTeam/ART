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

# Wrist: Up and down rotation (Servo)
int16 wristAngular

# Clip: left and right rotation angular (Servo)
int16 clipAngular
