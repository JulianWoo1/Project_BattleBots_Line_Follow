#include <Adafruit_NeoPixel.h>
#define DEBUG

#define NEO_PIXEL_PIN 8 //Attached to digital pin 8
#define NUM_PIXELS 4 // Number of NeoPixels

Adafruit_NeoPixel pixels(NUM_PIXELS, NEO_PIXEL_PIN, NEO_GRB + NEO_KHZ800);

const int SERVO_PIN = 3; 

// Motor A1 is pin 10
const int MOTOR_A1 = 10;
// Motor A2 is pin 5
const int MOTOR_A2 = 5;
// Motor B1 is pin 6
const int MOTOR_B1 = 6;
// Motor B2 is pin 9
const int MOTOR_B2 = 9;

const int MAX_SPEED = 240;
const int MID_SPEED = 190;

//range how the gripper open and closes. 360 degrees
int OPENGRIP_VALUE = 120;
int CLOSEGRIP_VALUE = 50;

unsigned long currentMillis = 0;
unsigned long previousActionMillis = 0;
unsigned long previousServoMillis = 0;
const long SERVO_INTERVAL = 20; // 20 ms delay

const long OPEN_TIME = 0;            // Open grip at start
const long CLOSE_TIME = 1000;        // Close grip after 1s
const long OPEN_AGAIN_TIME = 2000;    // Open grip again after 2s
const long DRIVE_START_TIME = 3000;   // Start driving after 3s
const long GRAB_TIME = 4000;         // Grab cone after 4s (after driving)
const long DRIVE_AGAIN_TIME = 5000;   // Drive again after 5s
const long HOLD_TIME = 7000;         // Stop and hold after 7s
const long DROP_TIME = 10000;        // Drop after 10s

boolean isGripClosed = false;

void generatePulse(int angle) {
  int pulseWidth = map(angle, 0, 180, 544, 2400);
  digitalWrite(SERVO_PIN, HIGH);
  delayMicroseconds(pulseWidth);
  digitalWrite(SERVO_PIN, LOW);
}

void setup() 
{
  pixels.begin(); // INITIALIZE NeoPixel strip object
  
  // initialize serial communication:
  Serial.begin(9600);

  //motor A have output
  pinMode(MOTOR_A1, OUTPUT);
  pinMode(MOTOR_A2, OUTPUT);

  //motor B have output
  pinMode(MOTOR_B1, OUTPUT);
  pinMode(MOTOR_B2, OUTPUT);

  pinMode(SERVO_PIN, OUTPUT);

}


void loop() 
{ 
  pickUp();
}

void pickUp() {
    currentMillis = millis();
    unsigned long elapsedTime = currentMillis - previousActionMillis;
    
    // Keep servo active
    if(currentMillis - previousServoMillis >= SERVO_INTERVAL) {
      previousServoMillis = currentMillis;
      if(isGripClosed) {
        generatePulse(CLOSEGRIP_VALUE);
      } else {
        generatePulse(OPENGRIP_VALUE);
      }
    }
    
    // Open the grip
    if(elapsedTime >= OPEN_TIME && elapsedTime < CLOSE_TIME && isGripClosed) {
      isGripClosed = false;
    }
    
    // Close it after 1 second
    else if(elapsedTime >= CLOSE_TIME && elapsedTime < OPEN_AGAIN_TIME && !isGripClosed) {
      isGripClosed = true;
    }
    
    // Open it again after 1 more second
    else if(elapsedTime >= OPEN_AGAIN_TIME && elapsedTime < DRIVE_START_TIME && isGripClosed) {
      isGripClosed = false;
    }
    
    // Drive forward to the cone
    else if(elapsedTime >= DRIVE_START_TIME && elapsedTime < GRAB_TIME) {
      moveForward();
    }
    
    // Stop and grab the cone
    else if(elapsedTime >= GRAB_TIME && elapsedTime < DRIVE_AGAIN_TIME && !isGripClosed) {
      stopMotor();
      isGripClosed = true;
    }
    
    // Drive forward again with the cone
    else if(elapsedTime >= DRIVE_AGAIN_TIME && elapsedTime < HOLD_TIME) {
      moveForward();
    }
    
    // Stop and hold the cone
    else if(elapsedTime >= HOLD_TIME && elapsedTime < DROP_TIME) {
      stopMotor();
    }

    else if (elapsedTime >= DROP_TIME) {
      isGripClosed = false;
    }
}


void openGrip() {
  generatePulse(OPENGRIP_VALUE);
  delay(300);
}

void closeGrip() {
  generatePulse(CLOSEGRIP_VALUE);
  delay(300);
}

void moveForward() {  
  analogWrite(MOTOR_A1, 0);  
  analogWrite(MOTOR_A2, MID_SPEED);

  analogWrite(MOTOR_B1, MID_SPEED);  
  analogWrite(MOTOR_B2, 0);

}

//function to make robot go backwards
void moveBackwards() {
  brakeLight();
  analogWrite(MOTOR_A1, MAX_SPEED);  
  analogWrite(MOTOR_A2, 0);

  analogWrite(MOTOR_B1, 0);  
  analogWrite(MOTOR_B2, MAX_SPEED);
}

//function to make robot go left
void moveLeft(){
  leftSignal();
  analogWrite(MOTOR_A1, MAX_SPEED);  
  analogWrite(MOTOR_A2, 0);

  analogWrite(MOTOR_B1, MAX_SPEED);  
  analogWrite(MOTOR_B2, 0);
}

//function to make robot go right
void moveRight(){
  rightSignal();
  analogWrite(MOTOR_A1, 0);  
  analogWrite(MOTOR_A2, MAX_SPEED);

  analogWrite(MOTOR_B1, 0);  
  analogWrite(MOTOR_B2, MAX_SPEED);
}

//function to make robot stop
void stopMotor(){
  brakeLight();
  analogWrite(MOTOR_A1, 0);  
  analogWrite(MOTOR_A2, 0);

  analogWrite(MOTOR_B1, 0);  
  analogWrite(MOTOR_B2, 0);
}

void brakeLight() {
  pixels.clear(); // Set all pixel colors to 'off'
  pixels.setPixelColor(0, pixels.Color(0, 150, 0)); //Set left rear color to orange (G,R,B)
  pixels.setPixelColor(1, pixels.Color(0, 150, 0)); //Set right rear color to orange (G,R,B)
  pixels.show();   // Send the updated pixel colors to the hardware.
}

void rightSignal() {
    pixels.clear(); // Set all pixel colors to 'off'
    pixels.setPixelColor(1, pixels.Color(70, 255, 0)); //Set right rear color to orange (G,R,B)
    pixels.setPixelColor(2, pixels.Color(70, 255, 0)); //Set right front color to orange (G,R,B)
    pixels.show();   // Send the updated pixel colors to the hardware.
}

void leftSignal() {
    pixels.clear(); // Set all pixel colors to 'off'
    pixels.setPixelColor(0, pixels.Color(70, 255, 0)); //Set left rear color to orange (G,R,B)
    pixels.setPixelColor(3, pixels.Color(70, 255, 0)); //Set left front color to orange (G,R,B)
    pixels.show();   // Send the updated pixel colors to the hardware.
}

