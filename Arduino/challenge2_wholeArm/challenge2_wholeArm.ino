const byte EN[3] = {8, 43, 49};
const byte DIR[3] = {9, 45, 51};
const byte CLK[3] = {10, 47, 53};
float ac[3] = {0.008, 0.008, 0.008};
int speedMax[3] = {400, 400, 400};
const float speedMin = 5000.0;
float t[3] = {speedMin, speedMin, speedMin};
int CurrentSteps[3] = {0, 0, 0};
boolean checking[3] = {0, 0, 0};
boolean checked;
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
        SetStepper(100, 100, 100);
        break;
      case '2':
        SetStepper(500, 500, 500);
        break;
      case '3':
        SetStepper(500, 200, 300);
        break;
      case '4':
        SetStepper(100, 200, 100);
        break;
      case '5':
        SetStepper(100, 100, 100);
        break;
      case '6':
        SetStepper(100, 100, 100);
        break;
      case '0':
        delay(100);
        break;
    }
  }
  Serial.print("current");
  for (int i = 0; i < 2; i = i + 1)
  {
    Serial.print(CurrentSteps[i]);
    Serial.print("|");
  }
  Serial.println(CurrentSteps[2]);
}
void SetStepper(int a0, int a1, int a2)
{
  int DesiredSteps[3] = {a0, a1, a2};
  int stepdifference[3] = {a0 - CurrentSteps[0], a1 - CurrentSteps[1], a2 - CurrentSteps[2]};
  if (stepdifference[0] == 0 && stepdifference[1] == 0 && stepdifference[2] == 0)
  {
    checked = 1;
  }
  else
  {
    checked = 0;
  }
  for (int i = 0; i < 3; i = i + 1)
  {
    if (stepdifference[i] > 0)
    {
      digitalWrite(DIR[i], HIGH);
    }
    if (stepdifference[i] < 0)
    {
      digitalWrite(DIR[i], LOW);
      stepdifference[i] = -stepdifference[i];
    }
  }
  float Ac_interval[3];
  int Cs_interval[3];
  for (int i = 0; i < 3; i = i + 1)
  {
    Ac_interval[i] = (1000 / speedMax[i] - 1000 / speedMin) / ac[i];
    if ( Ac_interval[i] > (stepdifference[i] / 2))
    {
      Ac_interval[i] = stepdifference[i] / 2;
    }
    Cs_interval[i] = stepdifference[i] - 2 * Ac_interval[i];
  }
  while (checked == 0)
  {
    char c[3] = {'0', '1', '2'};
    for (int i = 0; i < 3; i = i + 1)
    {
      if ((stepdifference[i] >= (Cs_interval[i] + Ac_interval[i])) && stepdifference[i] > 0)
      {
        digitalWrite(CLK[i], HIGH);
        delayMicroseconds(t[i]);
        digitalWrite(CLK[i], LOW);
        delayMicroseconds(t[i]);
        double buff = 1000 / t[i];
        buff = buff + ac[i];
        t[i] = 1 / buff * 1000;
        stepdifference[i] = stepdifference[i] - 1;
        Serial.print(c[i]);
        Serial.print(" : ");
        Serial.println(t[i]);
      }
      if ((stepdifference[i] > Ac_interval[i]) && stepdifference[i] < (Cs_interval[i] + Ac_interval[i]))
      {
        digitalWrite(CLK[i], HIGH);
        delayMicroseconds(t[i]);
        digitalWrite(CLK[i], LOW);
        delayMicroseconds(t[i]);
        stepdifference[i] = stepdifference[i] - 1;
        Serial.print(c[i]);
        Serial.print(" : ");
        Serial.println(t[i]);
      }
      if ((stepdifference[i] <= Ac_interval[i]) && stepdifference[i] > 0)
      {
        digitalWrite(CLK[i], HIGH);
        delayMicroseconds(t[i]);
        digitalWrite(CLK[i], LOW);
        delayMicroseconds(t[i]);
        double buff = 1000 / t[i];
        buff = buff - ac[i];
        t[i] = 1 / buff * 1000;
        stepdifference[i] = stepdifference[i] - 1;
        Serial.print(c[i]);
        Serial.print(" : ");
        Serial.println(t[i]);
      }
      Serial.print("diff");
      for (int i = 0; i < 2; i = i + 1)
      {
        Serial.print(stepdifference[i]);
        Serial.print("|");
      }
      Serial.println(stepdifference[2]);
    }
    if (stepdifference[0] == 0 && stepdifference[1] == 0 && stepdifference[2] == 0)
    {
      checked = 1;
      for (int i = 0; i < 3; i = i + 1)
      {
        CurrentSteps[i] = DesiredSteps[i];
      }
      Serial.println("All motor in position!!");
    }
  }
}
