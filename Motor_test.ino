#define M1a 2
#define M1b 3
#define M2a 4
#define M2b 5
byte Speed = 120;

float c=0.6;
void setup() {
  // put your setup code here, to run once:
  pinMode(M1a, OUTPUT);
  pinMode(M1b, OUTPUT);
  pinMode(M2a, OUTPUT);
  pinMode(M2b, OUTPUT);
  Serial.begin(115200);

}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available())
  {
    Motor();

  }
  Test();
}

void Motor()
{

  char c = Serial.read();

  switch (c)
  {
    case 'f':
      analogWrite(M1a, Speed);
      analogWrite(M1b, 0);
      analogWrite(M2a, 0);
      analogWrite(M2b, Speed);
      Serial.println("forward");
      break;

    case 'r':
      analogWrite(M1a, 0);
      analogWrite(M1b, Speed*c);
      analogWrite(M2a, 0);
      analogWrite(M2b, Speed*c);
      Serial.println("turn right");
      break;

    case 'l':
      analogWrite(M1a, Speed*c);
      analogWrite(M1b, 0);
      analogWrite(M2a, Speed*c);
      analogWrite(M2b, 0);
      Serial.println("turn left");
      break;

    case 's':
      analogWrite(M1a, 0);
      analogWrite(M1b, 0);
      analogWrite(M2a, 0);
      analogWrite(M2b, 0);
      Serial.println("stop");
      break;


  }
}

void Test()
{
  analogWrite(M1a, Speed);
  analogWrite(M1b, 0);
  analogWrite(M2a, 0);
  analogWrite(M2b, Speed);
  Serial.println("forward");
  delay(3000);

  analogWrite(M1a, 0);
  analogWrite(M1b, Speed*c);
  analogWrite(M2a, 0);
  analogWrite(M2b, Speed*c);
  Serial.println("turn right");
  delay(3000);

  analogWrite(M1a, Speed*c);
  analogWrite(M1b, 0);
  analogWrite(M2a, Speed*c);
  analogWrite(M2b, 0);
  Serial.println("turn left");
  delay(3000);

  analogWrite(M1a, 0);
  analogWrite(M1b, 0);
  analogWrite(M2a, 0);
  analogWrite(M2b, 0);
  Serial.println("stop");
  delay(5000);
}
