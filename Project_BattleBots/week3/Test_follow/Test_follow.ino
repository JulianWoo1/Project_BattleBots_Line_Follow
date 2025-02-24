// Motor A1 is pin 10
const int MOTOR_A1 = 10;
// Motor A2 is pin 5
const int MOTOR_A2 = 5;
// Motor B1 is pin 6
const int MOTOR_B1 = 6;
// Motor B2 is pin 9
const int MOTOR_B2 = 9;

const int TRANSMIT_PIN = 11;

//define IR Sensor pins
const int SENSOR_PINS[] = {A0, A1, A2, A3, A4, A5, A6, A7};
const int NUM_SENSORS = 8; //amount of sensors

int threshold = 300;  //Threshold for obstacle avoidance

/*
The 8 IR sensors return analog values (0-1023).
Dark lines (black) give low values (~0-300).
Light surfaces (white) give high values (~700-1023).
*/

void setup() {
  Serial.begin(115200);

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

  //read all values from the snesors
  for (int i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print("Sensors: ");
    sensorValues[i] = analogRead(SENSOR_PINS[i]); //read all IRsensor data
    Serial.print(sensorValues[i]); //print IRsensor datas
    Serial.print(" ");

    //checks if sensors detect any obstacles
    if(sensorValues[i] > threshold)
    {
      obstacleDetected = true;
    }
  }

  Serial.println();
  Serial.println("-------------------");
  delay(200);


  int LEFT_SIDE = sensorValues[0]; //reads the left most sensor 
  int RIGHT_SIDE = sensorValues[7]; //reads the right most senor
   
   // a obstacle is detected the if statemnet starts
  if (obstacleDetected)
  {
    stopMotor(); //stop robot motor
    delay(500);

    if(LEFT_SIDE > RIGHT_SIDE)
    {
      dodgeRight(); 
      delay(500);
    }
    else 
    {
      dodgeLeft();
      delay(500);
    }

    moveForward();
    delay(500);
  }

}


//function to make robot go forward
void moveForward() {  
  analogWrite(MOTOR_A1, 0);  
  analogWrite(MOTOR_A2, 255);

  analogWrite(MOTOR_B1, 255);  
  analogWrite(MOTOR_B2, 0);
}

//function to make robot go backwards
void moveBackwards() {
  analogWrite(MOTOR_A1, 255);  
  analogWrite(MOTOR_A2, 0);

  analogWrite(MOTOR_B1, 0);  
  analogWrite(MOTOR_B2, 255);
}

//function to make robot go left
void moveLeft(){
  analogWrite(MOTOR_A1, 255);  
  analogWrite(MOTOR_A2, 0);

  analogWrite(MOTOR_B1, 255);  
  analogWrite(MOTOR_B2, 0);

}

void dodgeLeft(){
  analogWrite(MOTOR_A1, 191);  
  analogWrite(MOTOR_A2, 0);

  analogWrite(MOTOR_B1, 191);  
  analogWrite(MOTOR_B2, 0);

}

//function to make robot go right
void moveRight(){
  analogWrite(MOTOR_A1, 0);  
  analogWrite(MOTOR_A2, 255);

  analogWrite(MOTOR_B1, 0);  
  analogWrite(MOTOR_B2, 255);
}

void dodgeRight(){
  analogWrite(MOTOR_A1, 0);  
  analogWrite(MOTOR_A2, 191);

  analogWrite(MOTOR_B1, 0);  
  analogWrite(MOTOR_B2, 191);

}

//function to make robot stop
void stopMotor(){
  analogWrite(MOTOR_A1, 0);  
  analogWrite(MOTOR_A2, 0);

  analogWrite(MOTOR_B1, 0);  
  analogWrite(MOTOR_B2, 0);
}





