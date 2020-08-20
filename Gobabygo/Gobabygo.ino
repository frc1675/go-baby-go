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
  pinMode(rightMotorPin, OUTPUT);
  pinMode(leftMotorPin, OUTPUT);
}


void loop() {
  // put your main code here, to run repeatedly:
  currentTime = millis();
  timeSincePrint += currentTime - previousTime;
  timeSinceAcceleration = currentTime - previousTime;
  previousTime = currentTime;
  
  joyXVal = analogRead(joyStickXPin);
  joyXVal = 2/1023 * joyXVal - 1;
  joyXVal = correctForDeadzone(joyXVal, deadzone);
  joyXVal = accelerateToValue(joyXVal, previousJoyXVal, timeSinceAcceleration, accelerationPerMs);
  previousJoyXVal = joyXVal;
  
  joyYVal = analogRead(joyStickYPin);
  joyYVal = 2/1023 * joyYVal -1;
  joyYVal = correctForDeadzone(joyYVal, deadzone);
  joyYVal = accelerateToValue(joyYVal, previousJoyYVal, timeSinceAcceleration, accelerationPerMs);
  previousJoyYVal = joyYVal;
  
  if (joyYVal < 0) {
    joyYVal *= backwardPower;
  }
  else {
    joyXVal -= joyYVal * turnScaler;
  }

  // Cheesy drive logic
  leftMotorVal = joyYVal + joyXVal;
  rightMotorVal = joyYVal - joyXVal;

  if (abs(leftMotorVal) > 1 or abs(rightMotorVal) > 1) {  // the value of either motor can never be outside the range -1 to 1
    scaler = max(abs(leftMotorVal), abs(rightMotorVal));
    leftMotorVal = leftMotorVal / scaler;
    rightMotorVal = rightMotorVal / scaler;
  }
  
  analogWrite(leftMotorPin, leftMotorVal * 127.5 + 127.5);    // converting from -1-1 to 0-255
  analogWrite(rightMotorPin, rightMotorVal * 127.5 + 127.5);
  

  // "Loop" that happens twice every second; used for printing values
  if (timeSincePrint >= 500) {
    timeSincePrint = 0;
  
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
