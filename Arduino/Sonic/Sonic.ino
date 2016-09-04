#include <SoftwareSerial.h>
#define trig 3
#define echo 4
#define hit 15
SoftwareSerial Sonic (5, 6);
int count=0;
void setup() {
  // put your setup code here, to run once:
  Sonic.begin(115200);
  Serial.begin(115200);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  float duration, distance;
  
  digitalWrite(trig, HIGH);
  delayMicroseconds(1000);
  digitalWrite(trig, LOW);
  duration = pulseIn(echo, HIGH);
  distance = duration/2/29.1;

  if(distance<=hit)
  {
    Sonic.write('h');
    Serial.print("fuck yon");
    count=count+1;
    Serial.println(count);
    }
}
