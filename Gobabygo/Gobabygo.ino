const int joyStickXPin (0);
// const int joyStickYPin (1);
const int ledPin (6);
// const int leftMotor (4);
const float deadzone = 0.1;

unsigned long currentTime = 0;
unsigned long previousTime = 0;

float joyXVal = 0;
float scaledJoyXVal = 0.0;
// int joyYVal = 0;
// float scaledJoyYVal = 0.0;
int ledVal = 0;
float rightMotorVal = 0.0;
float leftMotorVal = 0.0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  joyXVal = analogRead(joyStickXPin);
  scaledJoyXVal = joyXVal/1023*2 - 1;
  if (abs(scaledJoyXVal) <= deadzone){
    scaledJoyXVal = 0;
  }
  else {
    if (scaledJoyXVal > deadzone) {
      scaledJoyXVal = (scaledJoyXVal - 1)/(1 - deadzone) + 1; //modified point-slope form of a line
    }
    else {
      scaledJoyXVal = (scaledJoyXVal + 1)/(1 - deadzone) - 1;
    }
  }
  
  ledVal = map(joyXVal, 0, 1023, 0, 225);
  analogWrite(ledPin, ledVal);
  
  // joyYVal = analogRead(joyStickYPin);
  // scaledJoyYVal = 2/1023 * joyYval -1;
  // if (abs(scaledJoyYVal) <= deadzone){
  //   scaledJoyYVal = 0;
  // }
  // else {
  //   if (scaledJoyYVal > deadzone) {
  //     scaledJoyYVal = (scaledJoyYVal - 1)/(1 - deadzone) - 1;
  //   }
  //   else {
  //     scaledJoyYVal = (scaledJoyYVal + 1)/(1 - deadzone) - 1;
  //   }
  // }

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

    analogWrite(ledPin, ledVal);
  }
}
