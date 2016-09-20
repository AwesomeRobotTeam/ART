#include <ros.h>
#include <challenge3/Laser_fort.h>
#include <Stepper.h>

#define lacer 7

const int stepsPerRevolution = 2048;  // change this to fit the number of steps per revolution

//Stepper Motor
Stepper stp1(stepsPerRevolution, 5, 3, 4, 2);
Stepper stp2(stepsPerRevolution, 11, 9, 10, 8);

void wrtFort( const challenge3::Laser_fort &msg)
{
   stp1.step( msg.rotsteps1);
   //delay( 100);
   stp2.step( msg.rotsteps2);
   //delay( 100);
   digitalWrite( lacer, msg.hit);
   if( msg.delay != 0)
     delay( msg.delay);
}

ros::NodeHandle nh;

ros::Subscriber< challenge3::Laser_fort > subfort("Arduino_Laser_fort", wrtFort);

void setup()
{
  pinMode( lacer, OUTPUT);
  // set the speed at 60 rpm:
  stp1.setSpeed(5);
  stp2.setSpeed(5);
  // initialize the serial port:
  //Serial.begin(57600);
  
  nh.initNode();
  nh.subscribe( subfort);
}

void loop()
{
  nh.spinOnce();
}
