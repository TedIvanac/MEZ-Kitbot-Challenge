/*
 * This code will set the robot to drive forward a certain distance.
 * Whatever distance is put into the driveDistance function
 */

#include <RedBot.h>

RedBotEncoder encoder = RedBotEncoder(A4, A5);
int countsPerRev = 4;

float wheelDiam = 2.56;  // diam = 65mm / 25.4 mm/in
float wheelCirc = PI*wheelDiam;  // Redbot wheel circumference = pi*D

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

void setup() {
  Serial.begin(9600);
  
}

void loop() {
  driveDistance(12, 150);

}

void go( enum Motor m, int speed)
{
  digitalWrite (m == L ? in1Pin : in3Pin , speed > 0 ? HIGH : LOW );
  digitalWrite (m == L ? in2Pin : in4Pin , speed <= 0 ? HIGH : LOW );
  analogWrite(m == L ? enAPin : enBPin, speed < 0 ? -speed : speed);
}

void driveDistance(float distance, int motorSpeed)
{
  long lCount = 0;
  long rCount = 0;
  float numRev;
  // debug
  Serial.print("driveDistance() ");
  Serial.print(distance);
  Serial.print(" inches at ");
  Serial.print(motorSpeed);
  Serial.println(" speed.");

  numRev = (float) distance / wheelCirc;
  Serial.println(numRev, 3);
  encoder.clearEnc(BOTH);  // clear the encoder count
  go(L, motorSpeed);
  go(R, motorSpeed);

  while (rCount < numRev*countsPerRev)
  {
    // while the left encoder is less than the target count -- debug print 
    // the encoder values and wait -- this is a holding loop.
    lCount = encoder.getTicks(LEFT);
    rCount = encoder.getTicks(RIGHT);
    Serial.print(lCount);
    Serial.print("\t");
    Serial.print(rCount);
    Serial.print("\t");
    Serial.println(numRev*countsPerRev);
  }
  // now apply "brakes" to stop the motors.
  go(L, 0);
  go(R, 0);
}
