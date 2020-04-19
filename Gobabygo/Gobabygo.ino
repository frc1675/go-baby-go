const int joyStickXPin (0);
// const int joyStickYPin (1);
const int ledPin (6);
// const int leftMotor (4);

unsigned long currentTime = 0;
unsigned long previousTime = 0;

int joyXVal = 0;
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
  scaledJoyXVal = 2/1023 * joyXVal -1;
  ledVal = map(joyXVal, 0, 1023, 0, 225);
  analogWrite(ledPin, ledVal);
  // joyYVal = analogRead(joyStickYPin);
  // scaledJoyYVal = 2/1023 * joyYval -1;
  
  currentTime = millis();
  if (currentTime - previousTime >= 500) {
    previousTime = currentTime;
  
    Serial.print("X joystick value: ");
    Serial.print(scaledJoyXVal);
    Serial.println();
    // Serial.print("Y joystick value: ");
    // Serial.print(scaledJoyYVal);
    // Serial.println();

    analogWrite(ledPin, ledVal);
  }
}
