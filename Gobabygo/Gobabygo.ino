#include <Servo.h>

const int joyStickXPin (0);
const int joyStickYPin (1);
const int rightMotorPin (6);
const int leftMotorPin (5);

unsigned long currentTime = 0;
unsigned long previousTime = 0;
int timeSincePrint = 0;

float joyXVal = 0;
float previousJoyXVal = 0;
float joyYVal = 0;
float previousJoyYVal = 0;

float rightMotorVal = 0;
float leftMotorVal = 0;

float scaler = 0;

Servo leftMotorController;
Servo rightMotorController;

int timeSinceAcceleration = 0;
const float deadzone = 0.1;
const float backwardPower = 0.4;
const float turnScaler = 0.5;
const float accelerationPerMs = 0.0005;


// This function will scale a value between -1 and 1 according to a deadzone:
float correctForDeadzone(float axis, float deadzone) {
  if (abs(axis) <= deadzone){
    return 0;
  }
  else if (axis > deadzone){
    return (axis - 1)/(1 - deadzone) + 1; // modified point-slope form of a line
  }
  else {
    return (axis + 1)/(1 - deadzone) - 1;
  }
}

// This funtion gradually accelerates a value toward a target
float accelerateToValue(float currentVal, float targetVal, int timeElapsed, float acceleration) {
  return currentVal + (targetVal - currentVal) * timeElapsed * acceleration;
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(joyStickXPin, INPUT);
  pinMode(joyStickYPin, INPUT);
  pinMode(rightMotorPin, OUTPUT);
  pinMode(leftMotorPin, OUTPUT);

  rightMotorController.attach(rightMotorPin);
  rightMotorController.writeMicroseconds(1500);
  leftMotorController.attach(leftMotorPin);
  leftMotorController.writeMicroseconds(1500);
  
  delay(3000);
}

void loop() {
  // put your main code here, to run repeatedly:
  currentTime = millis();
  timeSincePrint += currentTime - previousTime;
  timeSinceAcceleration = currentTime - previousTime;
  previousTime = currentTime;
  
  float rawJoyXValue = analogRead(joyStickXPin);
  joyXVal = 2.0/1023.0 * analogRead(joyStickXPin) - 1;
  joyXVal = correctForDeadzone(joyXVal, deadzone);
  // joyXVal = accelerateToValue(joyXVal, previousJoyXVal, timeSinceAcceleration, accelerationPerMs);
  previousJoyXVal = joyXVal;
  
  float rawJoyYValue = analogRead(joyStickYPin);
  joyYVal = 2.0/1023.0 * analogRead(joyStickYPin) -1;
  joyYVal = correctForDeadzone(joyYVal, deadzone);
  // joyYVal = accelerateToValue(joyYVal, previousJoyYVal, timeSinceAcceleration, accelerationPerMs);
  previousJoyYVal = joyYVal;
  
  if (joyYVal < 0) {
    joyYVal *= backwardPower;
  }
 
  // Cheesy drive logic
  leftMotorVal = joyYVal + joyXVal;
  rightMotorVal = joyYVal - joyXVal;
  
  if (abs(leftMotorVal) > 1 or abs(rightMotorVal) > 1) {  // the value of either motor can never be outside the range -1 to 1
    scaler = max(abs(leftMotorVal), abs(rightMotorVal));
    leftMotorVal = leftMotorVal / scaler;
    rightMotorVal = rightMotorVal / scaler;
  }
  
  leftMotorController.writeMicroseconds(int(leftMotorVal * 500 + 1500));    // converting from -1 - 1 to 1000 - 2000
  rightMotorController.writeMicroseconds(int(rightMotorVal * 500 + 1500));
  
  // "Loop" used for printing values
  if (timeSincePrint >= 2000) {
    timeSincePrint = 0;
    Serial.print("raw X joystick value: ");
    Serial.println(rawJoyXValue);
    Serial.print("raw Y joystick value: ");
    Serial.println(rawJoyYValue);
    Serial.print("X joystick value: ");
    Serial.println(joyXVal);
    Serial.print("Y joystick value: ");
    Serial.println(joyYVal);
    Serial.print("Left motor value: ");
    Serial.println(leftMotorVal);
    Serial.print("Right motor value: ");
    Serial.println(rightMotorVal);
  }
}
