#include <ros.h>
#include <challenge3/Stepper.h>
#include <Stepper.h>

#define lacer 7

const int stepsPerRevolution = 2048;  // change this to fit the number of steps per revolution

//Stepper Motor
Stepper stp1(stepsPerRevolution, 5, 3, 4, 2);
Stepper stp2(stepsPerRevolution, 11, 9, 10, 8);

void wrtStepper( const challenge3::Stepper &msg)
{
   stp1.step( msg.rosteps1);
   stp2.step( msg.rosteps2);
   digitalWrite( lacer, HIGH);
   delay( 1000);
   digitalWrite( lacer, LOW);
}

ros::NodeHandle nh;

ros::Subscriber< challenge3::Stepper > substepper("Arduino_Stepper", wrtStepper);

void setup()
{
  pinMode( lacer, OUTPUT);
  // set the speed at 60 rpm:
  stp1.setSpeed(5);
  stp2.setSpeed(5);
  // initialize the serial port:
  //Serial.begin(57600);
  
  nh.initNode();
  nh.subscribe( substepper);
}

void loop()
{
  nh.spinOnce();
}
