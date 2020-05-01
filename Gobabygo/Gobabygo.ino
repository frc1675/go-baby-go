const int joyStickXPin (0);
const int joyStickYPin (1);
const int rightMotorPin (6);
const int leftMotorPin (4);
const float deadzone = 0.1;

unsigned long currentTime = 0;
unsigned long previousTime = 0;

float joyXVal = 0;
float scaledJoyXVal = 0.0;
int joyYVal = 0;
float scaledJoyYVal = 0.0;
int ledVal = 0;
float rightMotorVal = 0.0;
float leftMotorVal = 0.0;

void correctForDeadzone(float axis, float deadzone) {
  if (abs(axis) <= deadzone){
    axis = 0;
  }
  else {
    if (axis > deadzone) {
      axis = (axis - 1)/(1 - deadzone) + 1; //modified point-slope form of a line
    }
    else {
      axis = (axis + 1)/(1 - deadzone) - 1;
    }
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(rightMotorPin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  joyXVal = analogRead(joyStickXPin);
  scaledJoyXVal = joyXVal/1023*2 - 1;
  correctForDeadzone(scaledJoyXVal, deadzone);
  
  ledVal = map(joyXVal, 0, 1023, 0, 255);
  analogWrite(rightMotorPin, ledVal);
  
  joyYVal = analogRead(joyStickYPin);
  scaledJoyYVal = 2/1023 * joyYVal -1;
  correctForDeadzone(scaledJoyYVal, deadzone);

  // "Loop" that happens twice every second
  currentTime = millis();
  if (currentTime - previousTime >= 500) {
    previousTime = currentTime;
  
    Serial.print("X joystick value: ");
    Serial.print(scaledJoyXVal);
    Serial.print(" ");
    Serial.print(joyXVal);
    Serial.println();
    // Serial.print("Y joystick value: ");
    // Serial.print(scaledJoyYVal);
    // Serial.println();
  }
}
