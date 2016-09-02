#define trig_R 2
#define echo_R 3

#define trig_C 4
#define echo_C 5

#define trig_L 6
#define echo_L 7

void setup() {
  // put your setup code here, to run once:
  pinMode(trig_R, OUTPUT);
  pinMode(echo_R, INPUT);
  
  pinMode(trig_C, OUTPUT);
  pinMode(echo_C, INPUT);

  pinMode(trig_L, OUTPUT);
  pinMode(echo_L, INPUT);

  Serial.begin(115200);

}
int count = 0;
void loop() {
  float duration_R, distance_R;
  float duration_C, distance_C;
  float duration_L, distance_L;
  int i,j,k;
    
  /*digitalWrite(trig_R, HIGH);
  delayMicroseconds(1000);
  digitalWrite(trig_R, LOW);
  duration_R = pulseIn(echo_R, HIGH);*/
  distance_C = sonic(trig_C,echo_C);
  distance_R = sonic(trig_R,echo_R);
  distance_L = sonic(trig_L,echo_L);
 
  /*Serial.print(distance_R);
  Serial.println(" cm_R");*/


/*  digitalWrite(trig_C, HIGH);
  delayMicroseconds(1000);
  digitalWrite(trig_C, LOW);
  duration_C = pulseIn(echo_C, HIGH);
  distance_C = duration_C/2/29.1;

  /*Serial.print(distance_C);
  Serial.println(" cm_C");*/

/*  digitalWrite(trig_L, HIGH);
  delayMicroseconds(1000);
  digitalWrite(trig_L, LOW);
  duration_L = pulseIn(echo_L, HIGH);
  distance_L = duration_L/2/29.1;*/

  /*Serial.print(distance_L);
  Serial.println(" cm_L");*/

 // delay(1000);

  if((distance_R < 20) || (distance_C < 20) || (distance_L < 20)){
    backward();
      if(distance_C <20) count = count + 1 ;
      if(count == 300){
        for(i=0;i<200;i++){
          for(j=0;j<200;j++){
            for(k=0;k<1;k++){
              analogWrite(8, 100);
              analogWrite(9, 0);
              analogWrite(10, 100);
              analogWrite(11, 0);
              }
            }
          }
        count = 0;
        }
    }
    
  else{
    forward();
    }
   
}

void forward()
{
  analogWrite(8, 100);
  analogWrite(9, 0);
  analogWrite(10, 100);
  analogWrite(11, 0);
}

void backward()
{
  analogWrite(8, 0);
  analogWrite(9, 100);
  analogWrite(10, 0);
  analogWrite(11, 100);
}

float sonic(int trig,int echo) 
{
  float duration;
  digitalWrite(trig, HIGH);
  delayMicroseconds(1000);
  digitalWrite(trig, LOW);
  duration = pulseIn(echo, HIGH);
  return duration/2/29.1;  
}
