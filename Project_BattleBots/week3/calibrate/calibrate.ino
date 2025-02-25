

const int TRANSMIT_PIN = 11;

//define IR Sensor pins
const int SENSOR_PINS[] = {A0, A1, A2, A3, A4, A5, A6, A7};
const int NUM_SENSORS = 8; //amount of sensors

int SENSORS_MIN_VALUE = 200;  //Minimum value for Threshold for obstacle avoidance
int SENSORS_MAX_VALUE = 800; //maximum value for obstacle avoidance

int AVG_VALUE = 0;

int CALIBRATED_MAX_VALUE = 0;  //lowest value of the line sensor
int CALIBRATED_MIN_VALUE = 1023; //highest value of the line sensor

int DEAD_ZONE_W = 0; //temporary value of the deadzone WHITE
int DEAD_ZONE_B = 0; //temporary value of the deadzone BLACK

int LINE_VALUE = 0;
int SURFACE_VALUE = 0;

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

  calibrate();

}

void loop() 
{
  for (int i = 0; i < NUM_SENSORS; i++) 
  {
      int sensorValue = analogRead(SENSOR_PINS[i]);

      if(sensorValue > DEAD_ZONE_B && sensorValue > LINE_VALUE)
      {
          Serial.println("line detected");
      }
      else if (sensorValue > DEAD_ZONE_W && sensorValue > SURFACE_VALUE)
      {
        Serial.println("surface detected");
      }


  }

}

//function to calibrate values
void calibrate()
{
  int sensorValues[NUM_SENSORS];

  Serial.println();
  Serial.println("----------------------------------------------------");
  Serial.print("Sensors: ");
  //read all values from the snesors
  for (int i = 0; i < NUM_SENSORS; i++)
  {
    sensorValues[i] = analogRead(SENSOR_PINS[i]); //read all IRsensor data
    Serial.print(sensorValues[i]); //print IRsensor datas
    Serial.print(" ");

  }
  Serial.println();
  Serial.println("----------------------------------------------------");
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

  LINE_VALUE = CALIBRATED_MIN_VALUE;
  SURFACE_VALUE = CALIBRATED_MAX_VALUE;

  
  Serial.println("Calibrated max value: (surface)");
  Serial.println(CALIBRATED_MAX_VALUE);

  Serial.println("Calibrated min value: (Line)");
  Serial.println(CALIBRATED_MIN_VALUE);

  Serial.println("average value:");
  Serial.println(AVG_VALUE);

  Serial.println("dead zone white:");
  Serial.println(DEAD_ZONE_W);

  Serial.println("dead zone black:");
  Serial.println(DEAD_ZONE_B);

  delay(500);
}


