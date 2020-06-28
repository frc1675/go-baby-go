const int joyStickXPin (0);
const int joyStickYPin (1);
const int rightMotorPin (6);
const int leftMotorPin (5);

unsigned long currentTime = 0;
unsigned long previousTime = 0;

// This function will scale a value between -1 and 1 according to a deadzone:
float correctForDeadzone(float axis, float deadzone) {
  if (abs(axis) <= deadzone){
    return 0;
  }
  else {
    if (axis > deadzone) {
      return (axis - 1)/(1 - deadzone) + 1; // modified point-slope form of a line
    }
    else {
      return (axis + 1)/(1 - deadzone) - 1;
    }
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(rightMotorPin, OUTPUT);
  pinMode(leftMotorPin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int joyXVal;
  float scaledJoyXVal;
  int joyYVal;
  float scaledJoyYVal;
  
  float rightMotorVal;
  float leftMotorVal;
  float scaler;

  const float deadzone = 0.1;
  const float backwardPower = 0.4;
  const float turnScaler = 0.5;
  
  joyXVal = analogRead(joyStickXPin);
  scaledJoyXVal = 2/1023 * joyXVal - 1;
  scaledJoyXVal = correctForDeadzone(scaledJoyXVal, deadzone);
  
  joyYVal = analogRead(joyStickYPin);
  scaledJoyYVal = 2/1023 * joyYVal -1;
  scaledJoyYVal = correctForDeadzone(scaledJoyYVal, deadzone);
  
  if (scaledJoyYVal < 0) {
    scaledJoyYVal *= backwardPower;
    scaledJoyXVal -= scaledJoyYVal * turnScaler;
  }

  // Cheesy drive logic
  leftMotorVal = scaledJoyYVal + scaledJoyXVal;
  rightMotorVal = scaledJoyYVal - scaledJoyXVal;

  if (abs(leftMotorVal) > 1 or abs(rightMotorVal) > 1) {  // the value of either motor can never be outside the range -1 to 1
    scaler = max(abs(leftMotorVal), abs(rightMotorVal));
    leftMotorVal = leftMotorVal / scaler;
    rightMotorVal = rightMotorVal / scaler;
  }
  
  analogWrite(leftMotorPin, leftMotorVal * 127.5 + 127.5);    // converting from -1-1 to 0-255
  analogWrite(rightMotorPin, rightMotorVal * 127.5 + 127.5);
  

  // "Loop" that happens twice every second; used for printing values
  currentTime = millis();
  if (currentTime - previousTime >= 500) {
    previousTime = currentTime;
  
    Serial.print("X joystick value: ");
    Serial.println(scaledJoyXVal);
    Serial.print("Y joystick value: ");
    Serial.println(scaledJoyYVal);
    Serial.print("Left motor value: ");
    Serial.println(leftMotorVal);
    Serial.print("Right motor value: ");
    Serial.println(rightMotorVal);
  }
}
