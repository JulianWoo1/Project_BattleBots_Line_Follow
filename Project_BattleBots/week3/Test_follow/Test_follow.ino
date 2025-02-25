// Motor A1 is pin 10
const int MOTOR_A1 = 10;
// Motor A2 is pin 5
const int MOTOR_A2 = 5;
// Motor B1 is pin 6
const int MOTOR_B1 = 6;
// Motor B2 is pin 9
const int MOTOR_B2 = 9;

const int TRANSMIT_PIN = 11;

const int MAX_SPEED = 240;

const int DODGE_SPEED = 191;


//define IR Sensor pins
const int SENSOR_PINS[] = {A0, A1, A2, A3, A4, A5, A6, A7};
const int NUM_SENSORS = 8; //amount of sensors

int THRESHOLD_MIN = 200;  //Minimum value for Threshold for obstacle avoidance
int THRESHOLD_MAX = 800; //maximum value for obstacle avoidance

/*
The 8 IR sensors return analog values (0-1023).
Dark lines (black) give low values (~0-300).
Light surfaces (white) give high values (~700-1023).
*/

void setup() {
  Serial.begin(9600);


  //Set all sensor pins as input
  for(int i = 0; i < NUM_SENSORS; i++)
  {
    pinMode(SENSOR_PINS[i], INPUT);
  }

  pinMode(TRANSMIT_PIN, OUTPUT);

  pinMode(MOTOR_A1, OUTPUT);
  pinMode(MOTOR_A2, OUTPUT);
  pinMode(MOTOR_B1, OUTPUT);
  pinMode(MOTOR_B2, OUTPUT);
}

void loop() {

  bool obstacleDetected = false;  
  int sensorValues[NUM_SENSORS];


  Serial.print("Sensors: ");
  //read all values from the snesors
  for (int i = 0; i < NUM_SENSORS; i++)
  {
    sensorValues[i] = analogRead(SENSOR_PINS[i]); //read all IRsensor data
    Serial.print(sensorValues[i]); //print IRsensor datas
    Serial.print(" ");

    //checks if sensors detect any obstacles
    if(sensorValues[i] < THRESHOLD_MAX && sensorValues[i] > THRESHOLD_MIN)
    {
      obstacleDetected = true;
    }
  }

  Serial.println();
  Serial.println("---------------------------------");
  delay(200);


  int LEFT_SIDE = sensorValues[0] + sensorValues[1]; //reads the left most sensor 
  int RIGHT_SIDE = sensorValues[7] + sensorValues[6]; //reads the right most senor



   // a obstacle is detected the if statemnet starts



  moveForward();

  if (obstacleDetected)
  {
    stopMotor(); //stop robot motor    
    delay(100);

    if(LEFT_SIDE > RIGHT_SIDE)
    {
      turnRight(); 
    }
    else
    {
      turnLeft();
    }

    moveForward();
  }


}

void turnLeft()
{

  analogWrite(MOTOR_A1, 150);  
  analogWrite(MOTOR_A2, 0);

  analogWrite(MOTOR_B1, 150);  
  analogWrite(MOTOR_B2, 0);
}

void turnRight()
{
  analogWrite(MOTOR_A1, 0);  
  analogWrite(MOTOR_A2, 150);

  analogWrite(MOTOR_B1, 0);  
  analogWrite(MOTOR_B2, 150);
}


//function to make robot go forward
void moveForward() {  
  analogWrite(MOTOR_A1, 0);  
  analogWrite(MOTOR_A2, MAX_SPEED);

  analogWrite(MOTOR_B1, MAX_SPEED);  
  analogWrite(MOTOR_B2, 0);
}

//function to make robot go backwards
void moveBackwards() {
  analogWrite(MOTOR_A1, MAX_SPEED);  
  analogWrite(MOTOR_A2, 0);

  analogWrite(MOTOR_B1, 0);  
  analogWrite(MOTOR_B2, MAX_SPEED);
}

//function to make robot go left
void moveLeft(){
  analogWrite(MOTOR_A1, MAX_SPEED);  
  analogWrite(MOTOR_A2, 0);

  analogWrite(MOTOR_B1, MAX_SPEED);  
  analogWrite(MOTOR_B2, 0);

}

void dodgeLeft(){
  analogWrite(MOTOR_A1, DODGE_SPEED);  
  analogWrite(MOTOR_A2, 0);

  analogWrite(MOTOR_B1, DODGE_SPEED);  
  analogWrite(MOTOR_B2, 0);

}

//function to make robot go right
void moveRight(){
  analogWrite(MOTOR_A1, 0);  
  analogWrite(MOTOR_A2, MAX_SPEED);

  analogWrite(MOTOR_B1, 0);  
  analogWrite(MOTOR_B2, MAX_SPEED);
}

void dodgeRight(){
  analogWrite(MOTOR_A1, 0);  
  analogWrite(MOTOR_A2, DODGE_SPEED);

  analogWrite(MOTOR_B1, 0);  
  analogWrite(MOTOR_B2, DODGE_SPEED);

}

//function to make robot stop
void stopMotor(){
  analogWrite(MOTOR_A1, 0);  
  analogWrite(MOTOR_A2, 0);

  analogWrite(MOTOR_B1, 0);  
  analogWrite(MOTOR_B2, 0);
}

void calibrate()
{
    int sensorValues[NUM_SENSORS];

  Serial.print("Sensors: ");
  //read all values from the snesors
  for (int i = 0; i < NUM_SENSORS; i++)
  {
    sensorValues[i] = analogRead(SENSOR_PINS[i]); //read all IRsensor data
    Serial.print(sensorValues[i]); //print IRsensor datas
    Serial.print(" ");

  }
  Serial.println();
  Serial.println("---------------------------------");
  delay(200);

  CALIBRATED_MAX_VALUE = 0;
  CALIBRATED_MIN_VALUE = 1023;


  for (int i = 0; i < NUM_SENSORS; i++)
  {
    if (sensorValues[i] > CALIBRATED_MAX_VALUE)
    {
      CALIBRATED_MAX_VALUE = sensorValues[i];


    }

    if(sensorValues[i] < CALIBRATED_MIN_VALUE)
    {
      CALIBRATED_MIN_VALUE = sensorValues[i];
    }

  }

  AVG_VALUE = (CALIBRATED_MAX_VALUE + CALIBRATED_MIN_VALUE) /2;
  DEAD_ZONE_W = (AVG_VALUE - 50);
  DEAD_ZONE_B = (AVG_VALUE + 50);

  
  Serial.println("Calibrated max value:");
  Serial.println(CALIBRATED_MAX_VALUE);

  Serial.println("Calibrated min value:");
  Serial.println(CALIBRATED_MIN_VALUE);

  Serial.println("average value:");
  Serial.println(AVG_VALUE);

  Serial.println("dead zone white:");
  Serial.println(DEAD_ZONE_W);

  Serial.println("dead zone black:");
  Serial.println(DEAD_ZONE_B);

  delay(500);
}


