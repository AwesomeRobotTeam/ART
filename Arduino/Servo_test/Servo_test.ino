#include <Servo.h>

#define SERVO 9

Servo myservo;

int pos = 0;

void setup()
{
    myservo.attach( SERVO);
}

void loop()
{
    myservo.write( 0);
    delay( 1000);
    myservo.write( 90);
    delay( 1000);
    myservo.write( 180);
    delay( 1000);
     
     for( pos = 0; pos < 181; pos += 30)
     {
        myservo.write( pos);
        delay( 1000);
     }
}
