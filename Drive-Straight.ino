#include <RedBot.h>

RedBotEncoder encoder = RedBotEncoder(A4, A5); //Initializes encoders on A4 and A5 ports
int countsPerRev = 4; //Number of counts per revolution

float wheelDiam = 2.56;  // diam = 65mm / 25.4 mm/in
float wheelCirc = PI*wheelDiam;  // Redbot wheel circumference = pi*D

//The beginning of the new drive code
// Motor control pins : L298N H bridge
const int enAPin = 6; // Left motor PWM speed control
const int in1Pin = 7; // Left motor Direction 1
const int in2Pin = 5; // Left motor Direction 2
const int in3Pin = 4; // Right motor Direction 1
const int in4Pin = 2; // Right motor Direction 2
const int enBPin = 3; // Right motor PWM speed control
enum Motor { L, R };

//Function makes motor m move at a certain speed
void go( enum Motor m, int speed) {
  digitalWrite (m == L ? in1Pin : in3Pin , speed > 0 ? HIGH : LOW );
  digitalWrite (m == L ? in2Pin : in4Pin , speed <= 0 ? HIGH : LOW );
  analogWrite(m == L ? enAPin : enBPin, speed < 0 ? -speed : speed);
}

void setup()
{
  Serial.begin(9600);
}

void loop(void)
{
    driveStraight(40, 190); //If you start of with a really high speed (210+), any tilt your robot has will be more pronounced on the first move    
}

void driveStraight(float distance, int motorPower) //I've been thinking that maybe offset should be an input too? They're gonna have to change it
{
  long lCount = 0;
  long rCount = 0;
  long targetCount;
  float numRev;

  // variables for tracking the left and right encoder counts
  long prevlCount = 0;
  long prevrCount = 0;;

  long lDiff, rDiff;  // diff between current encoder count and previous count

  // variables for setting left and right motor power
  int leftPower = motorPower;
  int rightPower = motorPower;

  // variable used to offset motor power on right vs left to keep straight.
  int offset = 10;  // offset amount to compensate Right vs. Left drive
  //This needs to be calibrated keeping in mind what speed you use. If you use a low speed and a high offset, there's a good chance a motor will end up flipping direction

  numRev = distance / wheelCirc;  // calculate the target # of rotations
  targetCount = numRev * countsPerRev;    // calculate the target count

  // debug
  Serial.print("driveStraight() ");
  Serial.print(distance);
  Serial.print(" inches at ");
  Serial.print(motorPower);
  Serial.println(" power.");

  Serial.print("Target: ");
  Serial.print(numRev, 3);
  Serial.println(" revolutions.");
  Serial.println();

  // print out header
  Serial.print("Left Count\t");   // "Left Count" and tab
  Serial.print("Left Speed\t");  // "Left Speed" and tab
  Serial.print("Right Count\t");  // "Right Count" and tab
  Serial.print("Right Speed\t");  // "Right Speed" and tab
  Serial.println("Target count");
  Serial.println("====================================================================================");

  encoder.clearEnc(BOTH);    // clear the encoder count
  delay(100);  // short delay before starting the motors.

  //Big difference from reference code. I had to turn on each motor at a time. I don't think that's the issue though.
  go(R, motorPower);
  go(L, motorPower);

  while (lCount < targetCount)
  {
    // while the right encoder is less than the target count -- debug print 
    // the encoder values and wait -- this is a holding loop.
    lCount = encoder.getTicks(LEFT);
    rCount = encoder.getTicks(RIGHT);
    Serial.print(lCount);
    Serial.print("\t");
    Serial.print("\t");
    Serial.print(leftPower);
    Serial.print("\t");
    Serial.print("\t");
    Serial.print(rCount);
    Serial.print("\t");
    Serial.print("\t");
    Serial.print(rightPower);
    Serial.print("\t");
    Serial.print("\t");
    Serial.println(targetCount);

    //This is how it was written in the reference code
    go(L, leftPower);
    go(R, rightPower);

    // calculate the rotation "speed" as a difference in the count from previous cycle.
    lDiff = (lCount - prevlCount);
    rDiff = (rCount - prevrCount);

    // store the current count as the "previous" count for the next cycle.
    prevlCount = lCount;
    prevrCount = rCount;

    // if right is faster than the left, speed up the left / slow down right
    if (lDiff < rDiff) 
    {
      leftPower = leftPower + offset;  
      rightPower = rightPower - offset;
    }
    
    // if left is faster than the right, slow down the left / speed up right
    else if (lDiff > rDiff) 
    {
      leftPower = leftPower - offset;
      rightPower = rightPower + offset;
    }
    else
    {
      delay(50);
      continue;
    }
    delay(50);  // short delay to give motors a chance to respond.
  }
  // now apply "brakes" to stop the motors.
  //reference code uses a brake function, but we don't have that. We can only braka by setting speed to 0
  go(L, 0);
  go(R, 0); 
  delay(7000); //Gives me time to reset in between drives
}
