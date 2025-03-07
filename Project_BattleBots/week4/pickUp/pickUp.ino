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

//range how the gripper open and closes. 360 degrees
int OPENGRIP_VALUE = 120;
int CLOSEGRIP_VALUE = 50;

unsigned long previousMillis = 0;

const long interval = 2000; //time between actions. every 1000 is a second

// establish variables for duration of the ping, and the distance result
// in inches and centimeters:
long duration, inches, cm;
int readCount = 0; //counts the ammount of messages

void generatePulse(int angle){
  int pulseWidth = map(angle, 0, 180, 544, 2400);
  digitalWrite(SERVO_PIN, HIGH);
  delayMicroseconds(pulseWidth);
  digitalWrite(SERVO_PIN, LOW);
}

void setup() 
{
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)

  // put your setup code here, to run once:
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

void pickUp()
{
      // Store the current time in milliseconds since the program started
      unsigned long currentMillis = millis();

      static unsigned long previousMillis1 = 0;
      static unsigned long previousMillis2 = 0;
      static unsigned long previousMillis3 = 0;
      static unsigned long previousMillis4 = 0;
      static unsigned long previousMillis5 = 0;
      static bool open = false;

      int interval2 = 2000;
      int interval3 = 3000;
      int interval4 = 4000;
      int interval5 = 5000;

      // Check if the difference between the current time and the previous time is greater than or equal to the set interval
      //interval = 1000
      if(currentMillis - previousMillis1 >= interval)
      {
        if(!open)
        {
          openGrip();
        }
        else
        {
          closeGrip();
        }

        // If true, update the previous time to the current time
        //when the if statement finishes it loops again.
        open = !open;
        previousMillis1 = currentMillis;
      
      }

      if(currentMillis - previousMillis2 >= interval2)
      {
        moveForward();
        previousMillis2 = currentMillis;
      }

      if(currentMillis - previousMillis3 >= interval3)
      {
        stopMotor();
        previousMillis3 = currentMillis;
      }

      if(currentMillis - previousMillis4 >= interval4)
      {
        if(open)
        {
          closeGrip();
        }
        previousMillis4 = currentMillis;
      }

      if(currentMillis - previousMillis5 >= interval5)
      {
        moveForward();
        previousMillis5 = currentMillis;
      }
    
}


void openGrip(){
  generatePulse(OPENGRIP_VALUE);
  delay(300);
}

void closeGrip()
{
  generatePulse(CLOSEGRIP_VALUE);
  delay(300);
}

void moveForward() {  
  analogWrite(MOTOR_A1, 0);  
  analogWrite(MOTOR_A2, MAX_SPEED);

  analogWrite(MOTOR_B1, MAX_SPEED);  
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


  long microsecondsToInches(long microseconds) {
    // According to Parallax's datasheet for the PING))), there are 73.746
    // microseconds per inch (i.e. sound travels at 1130 feet per second).
    // This gives the distance travelled by the ping, outbound and return,
    // so we divide by 2 to get the distance of the obstacle.
    // See: https://www.parallax.com/package/ping-ultrasonic-distance-sensor-downloads/
    return microseconds / 74 / 2;
  }

  long microsecondsToCentimeters(long microseconds) {
    // The speed of sound is 340 m/s or 29 microseconds per centimeter.
    // The ping travels out and back, so to find the distance of the object we
    // take half of the distance travelled.
    return microseconds / 29 / 2;

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

    delay(500); // Wait for the specified time

    // Turn the pixels off
    pixels.clear(); // Set all pixel colors to 'off'
    pixels.show();   // Send the updated pixel colors to the hardware.

    delay(500); // Wait for the specified time
}

void leftSignal() {
    pixels.clear(); // Set all pixel colors to 'off'
    pixels.setPixelColor(0, pixels.Color(70, 255, 0)); //Set left rear color to orange (G,R,B)
    pixels.setPixelColor(3, pixels.Color(70, 255, 0)); //Set left front color to orange (G,R,B)
    pixels.show();   // Send the updated pixel colors to the hardware.

    delay(500); // Wait for the specified time

    // Turn the pixels off
    pixels.clear(); // Set all pixel colors to 'off'
    pixels.show();   // Send the updated pixel colors to the hardware.

    delay(500); // Wait for the specified time
}

