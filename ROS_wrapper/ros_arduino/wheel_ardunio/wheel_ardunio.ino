#include <ros.h>
#include <rosserial_arduino/Adc.h>

#define leftwheel0 5
#define leftwheel1 6
#define rightwheel0 10
#define rightwheel1 11

ros::NodeHandle  nh;

void writeAdc( const rosserial_arduino::Adc& msg){
  analogWrite( leftwheel0, msg.adc0);
  analogWrite( leftwheel1, msg.adc1);
  analogWrite( rightwheel0, msg.adc2);
  analogWrite( rightwheel1, msg.adc3);
}

ros::Subscriber< rosserial_arduino::Adc> subard("man_ard", writeAdc );

void setup()
{
  pinMode( leftwheel0, OUTPUT);
  pinMode( leftwheel1, OUTPUT);
  pinMode( rightwheel0, OUTPUT);
  pinMode( rightwheel1, OUTPUT);
  nh.initNode();
  nh.subscribe(subard);
}

void loop()
{
  nh.spinOnce();
  delay(1);
}
