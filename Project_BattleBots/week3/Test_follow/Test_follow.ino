// Motor A1 is pin 10
const int MOTOR_A1 = 10;
// Motor B1 is pin 5
const int MOTOR_A2 = 5;
// Motor B1 is pin 6
const int MOTOR_B1 = 6;
// Motor B2 is pin 9
const int MOTOR_B2 = 9;

const int TRANSMIT_PIN = 11;

const int SENSOR_PINS[] = {A0, A1, A2, A3, A4, A5, A6, A7};
const int NUM_SENSORS = 8;

int threshold = 500;  

void setup() {
  Serial.begin(115200);

  for(int i = 0; i < NUM_SENSORS; i++)
  {
    pinMode(SENSOR_PINS[i], INPUT);
  }

  pinMode(TRANSMIT_PIN, OUTPUT);
}

void loop() {

  for (int i = 0; i < NUM_SENSORS; i++)
  {
    int sensorValue = analogRead(SENSOR_PINS[i]);
    Serial.print("Sensor ");
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.println(sensorValue);


    if(sensorValue > threshold)
    {
      stopMotor();
      delay(500);
      dodgeLeft();
      break;

    }
  }

  delay(1000);

  if (Serial.available())
  {
    char command = Serial.read();
    if (command == '1')
    {
      maxForward();
    }
  }
}


//function to make robot go forward
void maxForward() {  
  analogWrite(MOTOR_A1, 0);  
  analogWrite(MOTOR_A2, 255);

  analogWrite(MOTOR_B1, 255);  
  analogWrite(MOTOR_B2, 0);
}

//function to make robot go backwards
void maxBackwards() {
  analogWrite(MOTOR_A1, 255);  
  analogWrite(MOTOR_A2, 0);

  analogWrite(MOTOR_B1, 0);  
  analogWrite(MOTOR_B2, 255);
}

//function to make robot go left
void maxLeft(){
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
void maxRight(){
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
