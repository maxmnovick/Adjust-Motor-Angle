const int p_fl = 2;
const int p_fr = 3;
const int p_l = 4;
const int p_r = 5;
const int lMotor = 9;
const int rMotor = 10;
const int lcdPin = 12;
const int analogPin = 0;
#include <SoftwareSerial.h>;
SoftwareSerial LCD_Serial = SoftwareSerial(255, lcdPin);

float analogInput;
float duration = 2000;
float scaledDC; //duty cycle after accounting for battery drain

void setup() {
  pinMode(lMotor, OUTPUT);
  pinMode(rMotor, OUTPUT);
  pinMode(lcdPin, OUTPUT);
  digitalWrite(lcdPin, HIGH);
  LCD_Serial.begin(9600);
  delay(100); //Required delay after starting serial communication.
  LCD_Serial.write(12);
  delay(10);
}

void loop() {
  delay(8000);
  turnRight(60.0);
  delay(duration);
  pause();
  delay(8000);
  turnRight(70.0);
  delay(duration);
  pause();
  delay(8000);
  turnRight(80.0);
  delay(duration);
  pause();
  delay(8000);
  turnRight(90.0);
  delay(duration);
  pause();
  delay(8000);
  turnLeft(60.0);
  delay(duration);
  pause();
  delay(8000);
  turnLeft(70.0);
  delay(duration);
  pause();
  delay(8000);
  turnLeft(80.0);
  delay(duration);
  pause();
  delay(8000);
  turnLeft(90.0);
  delay(duration);
  pause();
}

void turnRight(float dc) {
  scaledDC = scaleDutyCycle(dc);
  analogInput = scaledDC * 255.0 / 100.0;
  analogWrite(rMotor, 0);
  analogWrite(lMotor, analogInput / 1.1);
}

void turnLeft(float dc) {
  scaledDC = scaleDutyCycle(dc);
  analogInput = scaledDC * 255.0 / 100.0;
  analogWrite(lMotor, 0);
  analogWrite(rMotor, analogInput);
}

void pause() {
  analogWrite(lMotor, 0);
  analogWrite(rMotor, 0);
}

//account for battery drain to determine appropriate duty cycle
float scaleDutyCycle(float dc) {
  const int numSamples = 10;
  const float divFactor = 1.9; //found from experimentation
  float voltage;
  float sdc; //scaled duty cycle
  int analogVal;
  int sum;
  //take 10 analog samples
  for (int i = 0; i < numSamples; i++) {
    analogVal = analogRead(analogPin);
    sum += analogVal;
    delay(10); //avoid potentially jumbled readings
  }

  //calculate voltage
  voltage = ((float)sum / (float)numSamples * 5.0 / 1023.0) * (float)divFactor;
  //  Serial.print("V = ");
  //  Serial.println(voltage);
  //calculate scaled duty cycle
  const float dcToVFactor = 1.067; //found from experimentation
  sdc = constrain(dcToVFactor * 9.0 * dc / voltage, 0.0, 100.0);
  //  Serial.print("DC = ");
  //  Serial.println(scaledDC);
  sum = 0;

  return sdc;
}
