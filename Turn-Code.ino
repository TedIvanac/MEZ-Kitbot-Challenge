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

void turnL(){
  go(L,200);
  go(R,0);
  delay(750);
  go(R,0);
  go(L,0);
}

void turnR(){
  go(R,200);
  go(L,0);
  delay(750);
  go(R,0);
  go(L,0);
}

void brake(){
  go(R,0);
  go(L,0);
}

void setup() {
  // put your setup code here, to run once:
  //turnR();
}

void loop() {
  // put your main code here, to run repeatedly:
  //turnR();
  //delay(2000);
  turnL();
  delay(2000);
}
