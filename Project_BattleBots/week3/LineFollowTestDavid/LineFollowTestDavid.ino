#include <Adafruit_NeoPixel.h>
#define DEBUG

#define NEO_LED 13
#define NUM_PIXELS 4
#define DELAY_TIME 500

Adafruit_NeoPixel strip(NUM_PIXELS, NEO_LED, NEO_GRB + NEO_KHZ800);

// Motor pins match exactly with the original code
const int MOTOR_A1 = 10;  // Original left motor pin 1
const int MOTOR_A2 = 5;   // Original left motor pin 2
const int MOTOR_B1 = 6;   // Original right motor pin 1
const int MOTOR_B2 = 9;   // Original right motor pin 2

const int BUTTON_ON = 7;
const int BUTTON_OFF = 4;
const int ECHO = 9;
const int TRIG = 3;
float duration, distance;
#define MAX_DISTANCE 200  
#define NUM_MEASUREMENTS 5 
const int MOTOR_SENSOR1 = A4;
const int MOTOR_SENSOR2 = A5;
int cws1 = 0, cws2 = 0;  
unsigned long previousMillis = 0;
const long INTERVAL = 1000;  
const int SERVO_PIN = 12;
const int TRANSMIT_PIN = 11;  // Added from old code

// Maximum speed from old code
const int MAX_SPEED = 240;

// Sensor array in the same order as the original code
const int NUM_SENSORS = 8;
const int SENSOR_PINS[NUM_SENSORS] = {A7, A6, A5, A4, A3, A2, A1, A0};

int minValues[NUM_SENSORS];
int maxValues[NUM_SENSORS];

void setup() {
  pinMode(MOTOR_A1, OUTPUT);
  pinMode(MOTOR_A2, OUTPUT);
  pinMode(MOTOR_B1, OUTPUT);
  pinMode(MOTOR_B2, OUTPUT);
  pinMode(BUTTON_ON, INPUT);
  pinMode(BUTTON_OFF, INPUT);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(MOTOR_SENSOR1, INPUT);
  pinMode(MOTOR_SENSOR2, INPUT);
  pinMode(SERVO_PIN, OUTPUT);
  pinMode(TRANSMIT_PIN, OUTPUT);

  Serial.begin(9600);

  for (int i = 0; i < NUM_SENSORS; i++) {
    pinMode(SENSOR_PINS[i], INPUT);
    minValues[i] = 1023;
    maxValues[i] = 0;
  }

  Serial.println("Beweeg de robot over de lijn voor kalibratie...");
  strip.begin();
  strip.show();
  stop();  
}

// Drive function using EXACTLY the same pin assignments and logic as the original code
void drive(int speedLeft, int speedRight) {
  speedLeft = constrain(speedLeft, 0, MAX_SPEED);
  speedRight = constrain(speedRight, 0, MAX_SPEED);

  // Use the original code's exact pin assignments for consistent direction
  analogWrite(MOTOR_A1, 0);
  analogWrite(MOTOR_A2, speedLeft);
  analogWrite(MOTOR_B1, speedRight);
  analogWrite(MOTOR_B2, 0);
}

// Stop motors with original pin names
void stop() {
  analogWrite(MOTOR_A1, 0);
  analogWrite(MOTOR_A2, 0);
  analogWrite(MOTOR_B1, 0);
  analogWrite(MOTOR_B2, 0);
}

// Using original pin names for all motor functions
void turnLeft() {
  analogWrite(MOTOR_A1, 150);  
  analogWrite(MOTOR_A2, 0);
  analogWrite(MOTOR_B1, 150);  
  analogWrite(MOTOR_B2, 0);
}

void turnRight() {
  analogWrite(MOTOR_A1, 0);  
  analogWrite(MOTOR_A2, 150);
  analogWrite(MOTOR_B1, 0);  
  analogWrite(MOTOR_B2, 150);
}

void moveForward() {  
  analogWrite(MOTOR_A1, 0);  
  analogWrite(MOTOR_A2, MAX_SPEED);
  analogWrite(MOTOR_B1, MAX_SPEED);  
  analogWrite(MOTOR_B2, 0);
}

void moveBackwards() {
  analogWrite(MOTOR_A1, MAX_SPEED);  
  analogWrite(MOTOR_A2, 0);
  analogWrite(MOTOR_B1, 0);  
  analogWrite(MOTOR_B2, MAX_SPEED);
}

void slowTurnRight() {
  analogWrite(MOTOR_A1, 0);  
  analogWrite(MOTOR_A2, 125);  
  analogWrite(MOTOR_B1, 0);  
  analogWrite(MOTOR_B2, 125);
}

void slowTurnLeft() {
  analogWrite(MOTOR_A1, 150);  
  analogWrite(MOTOR_A2, 0);  
  analogWrite(MOTOR_B1, 150);  
  analogWrite(MOTOR_B2, 0);
}

// Measure distance
void measureDistance() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  duration = pulseIn(ECHO, HIGH);
  distance = (duration * 0.0343) / 2;
  Serial.print("Distance: ");
  Serial.println(distance);
}

// Read wheel sensor pulses
void readMotorSensors() {
  cws1 = pulseIn(MOTOR_SENSOR1, HIGH);
  cws2 = pulseIn(MOTOR_SENSOR2, HIGH);
}

int getPulseDifference() {
  unsigned long currentMillis = millis();
  
  if (currentMillis - previousMillis > INTERVAL) {
    previousMillis = currentMillis;
    readMotorSensors();

    int pulseDifference = cws1 - cws2;
    pulseDifference /= 1200;

    Serial.print("Pulse Difference: ");
    Serial.println(pulseDifference);
    return pulseDifference;
  }
  return 0;
}

// Store last known direction: -1 (left), 1 (right), 0 (center)
int lastDirection = 0;

void loop() {
  int sensorReadings[NUM_SENSORS];
  bool blackDetected = false;

  for (int i = 0; i < NUM_SENSORS; i++) {
      sensorReadings[i] = analogRead(SENSOR_PINS[i]);

      minValues[i] = min(sensorReadings[i], minValues[i]);
      maxValues[i] = max(sensorReadings[i], maxValues[i]);
  }

  int result = 0;
  for (int j = 0; j < NUM_SENSORS; j++) {
      result += (minValues[j] + maxValues[j]) / 2;
  }
  result /= NUM_SENSORS;
  
  int DEADZONE_LOW = result - 50;
  int DEADZONE_HIGH = result + 50;

  // Detect if any sensor sees black
  for (int i = 0; i < NUM_SENSORS; i++) {
      if (sensorReadings[i] <= DEADZONE_LOW) { 
          blackDetected = true;
          break;
      }
  }

  // Line Following Logic
  if (sensorReadings[4] >= DEADZONE_HIGH && sensorReadings[5] >= DEADZONE_HIGH) {
      drive(255, 255);  // Move forward
      lastDirection = 0;  // Reset direction when moving straight
  }
  else if (sensorReadings[5] >= DEADZONE_HIGH && sensorReadings[6] >= DEADZONE_HIGH) {
      drive(255, 170);  // Slight left
      lastDirection = -1;  // Remember last seen black was on the left
  } 
  else if (sensorReadings[6] >= DEADZONE_HIGH && sensorReadings[7] >= DEADZONE_HIGH) {
      drive(255, 10);   // More left
      lastDirection = -1;
  }  
  else if (sensorReadings[3] >= DEADZONE_HIGH && sensorReadings[4] >= DEADZONE_HIGH) {
      drive(170, 255);  // Slight right
      lastDirection = 1;  // Remember last seen black was on the right
  }  
  else if (sensorReadings[2] >= DEADZONE_HIGH && sensorReadings[3] >= DEADZONE_HIGH) {
      drive(60, 255);   // More right
      lastDirection = 1;
  }  
  else if (sensorReadings[1] >= DEADZONE_HIGH && sensorReadings[2] >= DEADZONE_HIGH) {
      drive(10, 255);   // Sharp right
      lastDirection = 1;
  }  
  else {
      // If line is lost, steer towards last known direction
      if (lastDirection == -1) {
          drive(255, 0);  // Turn left to search
      } 
      else if (lastDirection == 1) {
          drive(0, 255);  // Turn right to search
      } 
      else {
          drive(0, 0);  // Stop if no memory
      }
  }

  delay(50);  
}