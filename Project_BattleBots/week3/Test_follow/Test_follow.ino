// Motor A1 is pin 10
const int MOTOR_A1 = 10;
// Motor A2 is pin 5
const int MOTOR_A2 = 5;
// Motor B1 is pin 6
const int MOTOR_B1 = 6;
// Motor B2 is pin 9
const int MOTOR_B2 = 9;

const int TRANSMIT_PIN = 11;

const int SENSOR_PINS[] = {A0, A1, A2, A3, A4, A5, A6, A7};
const int NUM_SENSORS = 8;

int threshold = 300;  //Threshold for obstacle avoidance

/*
The 8 IR sensors return analog values (0-1023).
Dark lines (black) give low values (~0-300).
Light surfaces (white) give high values (~700-1023).
*/

void setup() {
  Serial.begin(115200);

  for(int i = 0; i < NUM_SENSORS; i++)
  {
    pinMode(SENSOR_PINS[i], INPUT);
  }

  pinMode(TRANSMIT_PIN, OUTPUT);
}

void loop() {

  bool obstacleDetected = false;
  int sensorValues[NUM_SENSORS];

  for (int i = 0; i < NUM_SENSORS; i++)
  {
    sensorValues[i] = analogRead(SENSOR_PINS[i]);
    Serial.print("Sensor ");
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.println(sensorValues[i]);


    if(sensorValues > threshold)
    {
      obstacleDetected = true;
    }
  }

  Serial.println("-------------------");
  delay(200);

  if (obstacleDetected)
  {
    stopMotor();
    delay(500);
    dodgeLeft();
    delay(500);
    moveForward();
  }

/*
  int leftValue = analogRead(SENSOR_PINS[0]); 
  int rightValue = analogRead(SENSOR_PINS[7]); 

  if (sensorValues[0] > threshold || sensorValues[1] > threshold)  
  {
    dodgeRight();  
  } 
  else if (sensorValues[6] > threshold || sensorValues[7] > threshold)  
  {
    dodgeLeft();  
  } 
  else 
  {
    moveForward();  
  }

*/

  delay(1000);

  if (Serial.available())
  {
    char command = Serial.read();
    if (command == '1')
    {
      moveForward();
    }
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





