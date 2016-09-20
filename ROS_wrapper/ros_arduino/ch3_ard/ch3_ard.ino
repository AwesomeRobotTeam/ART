#include <ros.h>
#include <challenge3/Laser_fort.h>
#include <Stepper.h>

#define lacer 7

float Speed = 1500;
int currentstate = 0;

const int stepsPerRevolution = 2048;  // change this to fit the number of steps per revolution

//Stepper Motor
//Stepper stp1(stepsPerRevolution, 5, 3, 4, 2);
//Stepper stp2(stepsPerRevolution, 11, 9, 10, 8);

void wrtFort( const challenge3::Laser_fort &msg)
{
   //stp1.step( msg.rotsteps1);
   Move( abs(msg.rotsteps1), msg.rotsteps1 > 0 ? 0 : 1, 2, 3, 4, 5);
   delay( 100);
   //stp2.step( msg.rotsteps2);
   Move( abs(msg.rotsteps2), msg.rotsteps2 > 0 ? 0 : 1, 8, 9, 10, 11);
   delay( 100);
   digitalWrite( lacer, msg.hit);
   if( msg.delay != 0)
     delay( msg.delay);
}

void Move(int steps, int dir, int IN1, int IN2, int IN3, int IN4)
{
  int S = 0;
  if (dir == 1)
  {
    while (steps != S)
    {
      State(currentstate, IN1, IN2, IN3, IN4);
      S = S + 1;
      currentstate = currentstate + 1;
      if (currentstate > 8)
      {
        currentstate = 1;
      }
    }
  }

  if (dir == 0)
  {
    while (steps != S)
    {
      State(currentstate, IN1, IN2, IN3, IN4);
      S = S + 1;
      currentstate = currentstate - 1;
      if (currentstate < 1)
      {
        currentstate = 8;
      }
    }
  }
}


void State(int state, int IN1, int IN2, int IN3, int IN4)
{
  switch (state)
  {
    case 1:
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, LOW);
      delayMicroseconds(Speed);
      break;

    case 2:
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, LOW);
      delayMicroseconds(Speed);
      break;

    case 3:
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, LOW);
      delayMicroseconds(Speed);
      break;

    case 4:
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      delayMicroseconds(Speed);
      break;

    case 5:
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      delayMicroseconds(Speed);
      break;

    case 6:
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, HIGH);
      delayMicroseconds(Speed);
      break;

    case 7:
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
      delayMicroseconds(Speed);
      break;

    case 8:
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
      delayMicroseconds(Speed);
      break;

  }

}

ros::NodeHandle nh;

ros::Subscriber< challenge3::Laser_fort > subfort("Arduino_Laser_fort", wrtFort);

void setup()
{
  pinMode( lacer, OUTPUT);
  // set the speed at 60 rpm:
  //stp1.setSpeed(5);
  //stp2.setSpeed(5);
  // initialize the serial port:
  //Serial.begin(57600);
  pinMode( 2, OUTPUT);
  pinMode( 3, OUTPUT);
  pinMode( 4, OUTPUT);
  pinMode( 5, OUTPUT);
  pinMode( 8, OUTPUT);
  pinMode( 9, OUTPUT);
  pinMode( 10, OUTPUT);
  pinMode( 11, OUTPUT);
  
  
  nh.initNode();
  nh.subscribe( subfort);
}

void loop()
{
  nh.spinOnce();
}
