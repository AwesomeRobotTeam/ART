/*
 * rosserial PubSub Example
 * Prints "hello world!" and toggles led
 */
#include <ros.h>
#include <rosserial_arduino/Adc.h>
//#include <std_msgs/String.h>
//#include <std_msgs/Empty.h>

ros::NodeHandle  nh;

void messageCb( const rosserial_arduino::Adc& msg){
  int i= msg.adc0;
  Serial.print("I got it ");
  Serial.println( i);
  digitalWrite(13, HIGH-digitalRead(13));
}

ros::Subscriber< rosserial_arduino::Adc> sub("PC", messageCb );



//std_msgs::String str_msg;
//ros::Publisher chatter("chatter", &str_msg);

//char hello[13] = "hello world!";

void setup()
{
  pinMode(13, OUTPUT);
  nh.initNode();
//  nh.advertise(chatter);
  nh.subscribe(sub);
}

void loop()
{
//  str_msg.data = hello;
//  chatter.publish( &str_msg );
  nh.spinOnce();
  delay(1);
}
