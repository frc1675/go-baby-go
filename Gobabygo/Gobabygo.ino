#include <Servo.h>

const int joyStickXPin (0);
const int joyStickYPin (1);
const int rightMotorPin (6);
const int leftMotorPin (5);

unsigned long currentTime, previousTime = 0;
int timeSincePrint, timeSinceAcceleration = 0;

int rawJoyXVal, rawJoyYVal = 0;
float joyXVal, targetJoyXVal, joyYVal, targetJoyYVal = 0;

float rightMotorVal, leftMotorVal = 0;

float scaler = 0;

const float deadzone = 0.1;
const float backwardPower = 0.4;
const int motorLoopTime = 10;
const float accelerationPerMs = 0.002;

Servo leftMotorController;
Servo rightMotorController;

/*===================================================================================*/

// This function will scale a value between -1 and 1 according to a deadzone
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


// This function will cap the car's acceleration
float safeAcceleration(float targetVal, float currentVal, float maxAcceleration) {
  if (abs(targetVal - currentVal) <= maxAcceleration){
    return targetVal;
  }
  else if (targetVal - currentVal > maxAcceleration){
    return currentVal + maxAcceleration;
  }
  else {
    return currentVal - maxAcceleration;
  }
}

/*===================================================================================*/

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
  timeSinceAcceleration += currentTime - previousTime;
  previousTime = currentTime;
  
  if (timeSinceAcceleration >= motorLoopTime) {
    rawJoyXVal = analogRead(joyStickXPin);
    targetJoyXVal = 2.0/1023.0 * rawJoyXVal - 1;
    targetJoyXVal = correctForDeadzone(targetJoyXVal, deadzone);
    if (targetJoyXVal < 0) {
      targetJoyXVal *= backwardPower;
    }
    joyXVal = safeAcceleration(targetJoyXVal, joyXVal, motorLoopTime*accelerationPerMs);
    
    rawJoyYVal = analogRead(joyStickYPin);
    targetJoyYVal = 2.0/1023.0 * rawJoyYVal -1;
    targetJoyYVal = correctForDeadzone(targetJoyYVal, deadzone);
    joyYVal = safeAcceleration(targetJoyYVal, joyYVal, motorLoopTime*accelerationPerMs);
   
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
  }
  
  // "Loop" used for printing values
  if (timeSincePrint >= 2000) {
    timeSincePrint = 0;
    Serial.println("raw X joystick value: " + String(rawJoyXVal));
    Serial.println("raw Y joystick value: " + String(rawJoyYVal));
    Serial.println("Target X joystick value: " + String(targetJoyXVal));
    Serial.println("Target Y joystick value: " + String(targetJoyYVal));
    Serial.println("X joystick value: " + String(joyXVal));
    Serial.println("Y joystick value: " + String(joyYVal));
    Serial.println("Left motor value: " + String(leftMotorVal));
    Serial.println("Right motor value: " + String(rightMotorVal));
    Serial.println();
  }
}
