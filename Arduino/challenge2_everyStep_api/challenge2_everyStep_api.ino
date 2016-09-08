const byte EN[3] = {8, 43, 49};
const byte DIR[3] = {9, 45, 51};
const byte CLK[3] = {10, 47, 53};
void setup() {
  // put your setup code here, to run once:
  pinMode(EN[2], OUTPUT);
  pinMode(CLK[2], OUTPUT);
  pinMode(DIR[2], OUTPUT);
  pinMode(EN[1], OUTPUT);
  pinMode(CLK[1], OUTPUT);
  pinMode(DIR[1], OUTPUT);
  pinMode(EN[0], OUTPUT);
  pinMode(CLK[0], OUTPUT);
  pinMode(DIR[0], OUTPUT);
  Serial.begin(115200);
}
void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(EN[2], LOW);
  digitalWrite(EN[1], LOW);
  digitalWrite(EN[0], LOW);
  //  Serial.println(a);
  if (Serial.available())
  {
    char c = Serial.read();
    int dis = 500;
    switch (c)
    {
      case '1':
        StepperControl(dis, 1, 0);
        break;
      case '2':
        StepperControl(dis, 0, 0);
        break;
      case '3':
        StepperControl(dis, 1, 1);
        break;
      case '4':
        StepperControl(dis, 0, 1);
        break;
      case '5':
        StepperControl(dis, 1, 2);
        break;
      case '6':
        StepperControl(dis, 0, 2);
        break;
      case '0':
        delay(100);
        break;
    }
  }
}
float ac[3] = {0.008, 0.008, 0.008};
int speedMax[3] = {400, 400, 400};
const float speedMin = 5000.0;
float t[3] = {speedMin, speedMin, speedMin};
void StepperControl(int a, boolean b, byte c)
{
  float t0 = micros();
  if (b)
  {
    digitalWrite(DIR[c], LOW);
  }
  else
  {
    digitalWrite(DIR[c], HIGH);
  }
  float Ac_interval = (1000 / speedMax[c] - 1000 / speedMin) / ac[c];
  if (Ac_interval > (a / 2))
  {
    Ac_interval = a / 2;
  }
  int Cs_interval = a - 2 * Ac_interval;
  for (int i = 0; i < Ac_interval ; i = i + 1)
  {
    digitalWrite(CLK[c], HIGH);
    delayMicroseconds(t[c]);
    digitalWrite(CLK[c], LOW);
    delayMicroseconds(t[c]);
    double buff = 1000 / t[c];
    buff = buff + ac[c];
    t[c] = 1 / buff * 1000;
    Serial.println(t[c]);
  }
  for (int i = 0; i < Cs_interval ; i = i + 1)
  {
    digitalWrite(CLK[c], HIGH);
    delayMicroseconds(t[c]);
    digitalWrite(CLK[c], LOW);
    delayMicroseconds(t[c]);
    Serial.println(t[c]);
  }
  for (int i = 0; i < Ac_interval ; i = i + 1)
  {
    digitalWrite(CLK[c], HIGH);
    delayMicroseconds(t[c]);
    digitalWrite(CLK[c], LOW);
    delayMicroseconds(t[c]);
    double buff = 1000 / t[c];
    buff = buff - ac[c];
    t[c] = 1 / buff * 1000;
    Serial.println(t[c]);
  }
  Serial.println(micros() - t0);
}
