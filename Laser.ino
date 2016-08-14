#include <Adafruit_PWMServoDriver.h>
#include<Wire.h>
#include<math.h>
#include<Servo.h>
Adafruit_PWMServoDriver ser0 = Adafruit_PWMServoDriver(0x40);

//Serial_int
String R = "";
int I[4];
int d1, d2;

Servo S0,S1;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  //16 Channel Servo
  ser0.begin();
  ser0.setPWMFreq(60);
  pinMode(8, OUTPUT);
  
  S0.attach(10);
  S1.attach(11);
}

/**
0(yaw):   500-50
1(pitch): 230-90, 150-47


**/

int pitch = 0;
int yaw = 0;
int t = 300;
int dir = 0;

void loop() {
  
  int L1=30;
  
  Read_int();
  double L2=sqrt(L1*L1+d1*d1);
  
  double yaw = (atan2(d1, L1)) * 180 / 3.14159;
  double pitch = atan2(d2, L2) * 180 / 3.14159;

  yaw = -yaw + 90;
  pitch = -pitch + 90;

  Serial.print(yaw);
  Serial.print("|");
  Serial.println(pitch);
  int pitch_buff = map( pitch, 70, 90, 198, 230);
  int yaw_buff = map(yaw, 60, 90, 470, 405);
  
  digitalWrite(8, LOW);
  ser0.setPWM(0, 0, yaw_buff);
  ser0.setPWM(1, 0, pitch_buff);
  
  delay(1000);
  digitalWrite(8, HIGH);
  delay(1000);
  





}

void Read_int()
{
  if (Serial.available())
  {
    
    R = R + Received();
    Serial.println(R);
    if (Verified() == 1)
    {
      Decode();
      Serial.print("d1: ");
      Serial.print(d1);
      Serial.print(" d2: ");
      Serial.println(d2);
    }
 
    else
    {Serial.println("booo");}


  }

  R = "";
}

String Received()
{
  int receiving = 1;
  String r = "";
  while (receiving == 1)
  {
    char buff = Serial.read();

    if (buff == 'b')
    {
      r = r + '@';
    }

    if (buff != 'b' && buff != 'c' && buff > 0)
    {
      r = r + buff;
    }

    if (buff == 'c')
    {
      receiving = 0;
      return r;
    }


  }


}

int Verified()
{
  I[0] = R.indexOf("@");
  for (int i = 1; i < 4; i = i + 1)
  {
    I[i] = R.indexOf("@", I[i - 1] + 1);
  }
  if
  (
    I[0] != -1
    &&
    I[2] != -1
    &&
    I[3] == -1
  )
  {
    return 1;
  }

  else
  {
    Serial.println("Format unknow...");
    return 0;

  }

}

void Decode()
{
  d1 = R.substring(I[0] + 1, I[1]).toInt();
  d2 = R.substring(I[1] + 1, I[2]).toInt();

}
