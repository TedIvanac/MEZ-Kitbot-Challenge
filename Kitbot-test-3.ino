/*
 * Code pulled from Sparkfun Redbot example code and cs.columbia.edu example code 
 * for the VKMaker 2WD Smart Robot
*/
#include <RedBot.h>
RedBotMotors motors;

RedBotEncoder encoder = RedBotEncoder(A4,A5);  // initializes encoder on pins A4 and A5
int countsPerRev = 4;   // 4 pairs of N-S x 48:1 gearbox = 192 ticks per wheel rev

// variables used to store the left and right encoder counts.
int lCount;
int rCount;

// Motor control pins : L298N H bridge
const int enAPin = 6; // Left motor PWM speed control
const int in1Pin = 7; // Left motor Direction 1
const int in2Pin = 5; // Left motor Direction 2
const int in3Pin = 4; // Right motor Direction 1
const int in4Pin = 2; // Right motor Direction 2
const int enBPin = 3; // Right motor PWM speed control
enum Motor { L, R };

// Set motor speed: 255 full ahead, −255 full reverse , 0 stop
void go( enum Motor m, int speed)
{
  digitalWrite (m == L ? in1Pin : in3Pin , speed > 0 ? HIGH : LOW );
  digitalWrite (m == L ? in2Pin : in4Pin , speed <= 0 ? HIGH : LOW );
  analogWrite(m == L ? enAPin : enBPin, speed < 0 ? -speed : speed);
}

void testMotors ()
{
  static int speed[8] = {128,255,128,0,-128,-255,-128,0};
  go(R, 0);
  for (unsigned char i = 0 ; i < 8 ; i++)
  go(L, speed[i ]), delay (200);
  for (unsigned char i = 0 ; i < 8 ; i++)
  go(R, speed[i ]), delay (200);
}

void setup() {
  Serial.begin(9600);
  pinMode(enAPin, OUTPUT);
  pinMode(in1Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);
  pinMode(in3Pin, OUTPUT);
  pinMode(in4Pin, OUTPUT);
  pinMode(enBPin, OUTPUT);

  go(L, 0);
  go(R, 0);

}

void loop() {
  
  go(L,255);
  go(R,255);
  lCount = encoder.getTicks(LEFT);    // read the left motor encoder
  rCount = encoder.getTicks(RIGHT);   // read the right motor encoder

  Serial.print(lCount);
  Serial.print("\t");
  Serial.println(rCount);

  if ((lCount > countsPerRev) || (rCount > countsPerRev) )
   {
    encoder.clearEnc(BOTH);  // Reset the counters.
   }
}
