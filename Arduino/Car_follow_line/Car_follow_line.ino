#define RW_1 8
#define RW_2 9
#define LW_1 10
#define LW_2 11

int s1=60;
int s2=60;

void setup() {
  pinMode(53, INPUT);
  pinMode(52, INPUT);
  pinMode(51, INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  int Right = digitalRead(52);
  int Left = digitalRead(53);
  int Center = digitalRead(51);
  Serial.println(Right);
  Serial.println(Left);
  Serial.println(Center);

  if (Right == 1 && Center == 0 && Left == 1 )
  {
    Forward();
  }

  if (Right == 0 && Center == 0 && Left == 1)
  {
    R();
  }

  if (Right == 0 && Center == 1  && Left == 1)
  {
    fast_R();
  }

  if (Right == 1 && Center == 0 && Left == 0)
  {
    L();
  }

  if (Right == 1 && Center == 1  && Left == 0)
  {
    fast_L();
  }

  if (Right == 1 && Center == 1  && Left == 1)
  {
    Back();
  }

}

void Forward()
{
  analogWrite(RW_1, s1);
  analogWrite(RW_2, 0);

  analogWrite(LW_1, s1);
  analogWrite(LW_2, 0);
}

void Back()
{
  analogWrite(RW_1, 0);
  analogWrite(RW_2, s1);

  analogWrite(LW_1, 0);
  analogWrite(LW_2, s1);
}

void L()
{
  analogWrite(RW_1, s1);
  analogWrite(RW_2, 0);

  analogWrite(LW_1, 0);
  analogWrite(LW_2, s2);
}

void fast_L()
{
  analogWrite(RW_1, s1);
  analogWrite(RW_2, 0);

  analogWrite(LW_1, 0);
  analogWrite(LW_2, 0);
}

void R()
{
  analogWrite(RW_1, 0);
  analogWrite(RW_2, s2);

  analogWrite(LW_1, s1);
  analogWrite(LW_2, 0);
}

void fast_R()
{
  analogWrite(RW_1, 0);
  analogWrite(RW_2, 0);

  analogWrite(LW_1, s1);
  analogWrite(LW_2, 0);
}

