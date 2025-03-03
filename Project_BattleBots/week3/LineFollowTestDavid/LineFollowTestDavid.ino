// Motor A1 is pin 10
const int MOTOR_A1 = 10;
// Motor A2 is pin 5
const int MOTOR_A2 = 5;
// Motor B1 is pin 6
const int MOTOR_B1 = 6;
// Motor B2 is pin 9
const int MOTOR_B2 = 9;

const int TRANSMIT_PIN = 11;
const int SENSOR_PINS[] = {A7, A6, A5, A4, A3, A2, A1, A0}; //define IR Sensor pins
const int NUM_SENSORS = 8; //amount of sensors
int AVG_VALUE = 0;
int CALIBRATED_MAX_VALUE = 0;  //maximum value of the line sensor
int CALIBRATED_MIN_VALUE = 1023; //minimum value of the line sensor
int DEAD_ZONE_W = 0; //threshold value for white surface
int DEAD_ZONE_B = 0; //threshold value for black line

const int MAX_SPEED = 200;      // Maximum motor speed
const int TURN_SPEED = 160;     // Speed during turns
const int SEARCH_SPEED = 140;   // Speed during search
const int SEARCH_TIMEOUT = 2000; // Time to search before giving up (ms)

// Search state tracking
enum SearchDirection { LEFT, RIGHT };
SearchDirection lastDirection = LEFT;
unsigned long lineLastSeen = 0;
bool searching = false;
int searchPhase = 0;

// Declare lineSensors array globally so debug() can access it
bool lineSensors[8];

void setup() 
{
  Serial.begin(9600);
  //Set all sensor pins as input
  for(int i = 0; i < NUM_SENSORS; i++) 
  {
    pinMode(SENSOR_PINS[i], INPUT);
  }
  
  pinMode(TRANSMIT_PIN, OUTPUT);
  digitalWrite(TRANSMIT_PIN, HIGH); // Turn on IR emitters

  pinMode(MOTOR_A1, OUTPUT);
  pinMode(MOTOR_A2, OUTPUT);
  pinMode(MOTOR_B1, OUTPUT);
  pinMode(MOTOR_B2, OUTPUT);
  
  delay(1000); // Give time for sensors to stabilize
  calibrate();
}

void loop() 
{
  // Read sensor values
  int lineDetected = 0;
  
  // Read all sensors
  for (int i = 0; i < NUM_SENSORS; i++) 
  {
    int sensorValue = analogRead(SENSOR_PINS[i]);
    
    // Detect black line (1) or white surface (0)
    if(sensorValue <= DEAD_ZONE_B) 
    {
      lineSensors[i] = 1;  // Black line
      lineDetected++;
    } 
    else 
    {
      lineSensors[i] = 0;  // White surface
    }
  }

  
  if (lineDetected == 0) 
  {
    // No line detected
    if (!searching) 
    {
      // Just lost the line, start search process
      searching = true;
      searchPhase = 0;
      lineLastSeen = millis();
      Serial.println("Line lost! Starting search pattern");
    }
    
    // Execute search pattern
    searchForLine();
  }
  else 
  {
    // Line found! Reset search variables
    searching = false;
    lineLastSeen = millis();
    
    // Left side sensors detect line (turn left)
    if (lineSensors[0] || lineSensors[1] || lineSensors[2]) 
    {
      lastDirection = LEFT; // Remember which side we last saw the line
      turnLeft();
      Serial.println("Left");
    }
    // Right side sensors detect line (turn right)
    else if (lineSensors[5] || lineSensors[6] || lineSensors[7]) 
    {
      lastDirection = RIGHT; // Remember which side we last saw the line
      turnRight();
      Serial.println("Right");
    }
    // Center sensors detect line (go forward)
    else if (lineSensors[3] || lineSensors[4]) 
    {
      moveForward();
      Serial.println("forward");
    }
  }

  delay(1000);
}

// Search pattern to find the line again
void searchForLine() 
{
  unsigned long searchTime = millis() - lineLastSeen;
  
  // If we've been searching too long, give up and stop
  if (searchTime > SEARCH_TIMEOUT) 
  {
    stopMotor();
    Serial.println("Search timeout reached, stopping");
    return;
  }
  
  switch (searchPhase) 
  {
    case 0:
      // First phase: Short backward movement
      moveBackward();
      if (searchTime > 200) 
      {
        searchPhase = 1;
        lineLastSeen = millis();
        Serial.println("Search phase 1: Backing up completed");
      }
      break;
      
    case 1:
      // Second phase: Turn in the direction the line was last seen
      if (lastDirection == LEFT) 
      {
        searchTurnLeft();
        Serial.println("Search phase 2: Turning left to find line");
      } 
      else 
      {
        searchTurnRight();
        Serial.println("Search phase 2: Turning right to find line");
      }
      
      // After some time, if still not found, switch to phase 2
      if (searchTime > 500) 
      {
        searchPhase = 2;
        lineLastSeen = millis();
      }
      break;
      
    case 2:
      // Third phase: Wider search in opposite direction
      if (lastDirection == LEFT) 
      {
        searchTurnRight();
        Serial.println("Search phase 3: Turning right (wider search)");
      } 
      else 
      {
        searchTurnLeft();
        Serial.println("Search phase 3: Turning left (wider search)");
      }
      
      // After a full turn, if still not found, try forward movement
      if (searchTime > 1000) 
      {
        searchPhase = 3;
        lineLastSeen = millis();
      }
      break;
      
    case 3:
      // Fourth phase: Move forward a bit
      moveForward();
      Serial.println("Search phase 4: Moving forward to find line");
      
      // After some forward movement, go back to first turning direction
      if (searchTime > 300) 
      {
        searchPhase = 1;
        lineLastSeen = millis();
        // Invert the last direction to create a zig-zag pattern
        lastDirection = (lastDirection == LEFT) ? RIGHT : LEFT;
      }
      break;
  }
}

// Turn left
void turnLeft() {
  analogWrite(MOTOR_A1, 0);
  analogWrite(MOTOR_A2, TURN_SPEED);
  analogWrite(MOTOR_B1, 0);
  analogWrite(MOTOR_B2, 0);
}

// Turn right
void turnRight() {
  analogWrite(MOTOR_A1, 0);
  analogWrite(MOTOR_A2, 0);
  analogWrite(MOTOR_B1, TURN_SPEED);
  analogWrite(MOTOR_B2, 0);
}

// Search turn left (slower speed)
void searchTurnLeft() {
  analogWrite(MOTOR_A1, 0);
  analogWrite(MOTOR_A2, SEARCH_SPEED);
  analogWrite(MOTOR_B1, 0);
  analogWrite(MOTOR_B2, SEARCH_SPEED/2);
}

// Search turn right (slower speed)
void searchTurnRight() {
  analogWrite(MOTOR_A1, 0);
  analogWrite(MOTOR_A2, SEARCH_SPEED/2);
  analogWrite(MOTOR_B1, SEARCH_SPEED);
  analogWrite(MOTOR_B2, 0);
}

// Move forward
void moveForward() {
  analogWrite(MOTOR_A1, 0);
  analogWrite(MOTOR_A2, MAX_SPEED);
  analogWrite(MOTOR_B1, MAX_SPEED);
  analogWrite(MOTOR_B2, 0);
}

// Move backward
void moveBackward() {
  analogWrite(MOTOR_A1, MAX_SPEED/2);
  analogWrite(MOTOR_A2, 0);
  analogWrite(MOTOR_B1, 0);
  analogWrite(MOTOR_B2, MAX_SPEED/2);
}

// Stop motors
void stopMotor() {
  analogWrite(MOTOR_A1, 0);
  analogWrite(MOTOR_A2, 0);
  analogWrite(MOTOR_B1, 0);
  analogWrite(MOTOR_B2, 0);
}

void debug() 
{
  // Print sensor values every 500ms
  static unsigned long lastPrintTime = 0;
  if (millis() - lastPrintTime > 500) 
  {
    for (int i = 0; i < NUM_SENSORS; i++) 
    {
      Serial.print(lineSensors[i]);
      Serial.print(" ");
    }
    Serial.print(" | Search: ");
    Serial.print(searching ? "YES" : "NO");
    Serial.print(" | Phase: ");
    Serial.print(searchPhase);
    Serial.print(" | Last direction: ");
    Serial.println(lastDirection == LEFT ? "LEFT" : "RIGHT");
    
    lastPrintTime = millis();
  }
}

// Calibration function

void calibrate() 
{
  Serial.println("Starting automatic calibration...");
  Serial.println("Move the robot over both the line and surface for 5 seconds");
  
  // Initialize values
  CALIBRATED_MAX_VALUE = 0;
  CALIBRATED_MIN_VALUE = 1023;
  
  // Set calibration duration (5 seconds)
  unsigned long startTime = millis();
  unsigned long calibrationDuration = 4000; // 4 seconds
  
  // Sample values continuously during calibration period
  while (millis() - startTime < calibrationDuration) 
  {
    for (int i = 0; i < NUM_SENSORS; i++) 
    {
      int sensorValue = analogRead(SENSOR_PINS[i]);
      
      // Update max and min values
      if (sensorValue > CALIBRATED_MAX_VALUE) 
      {
        CALIBRATED_MAX_VALUE = sensorValue;
      }
      if (sensorValue < CALIBRATED_MIN_VALUE) 
      {
        CALIBRATED_MIN_VALUE = sensorValue;
      }
    }
    
    // Short delay between readings
    delay(10);
    
    // Display progress every second
    if ((millis() - startTime) % 1000 < 20) 
    {
      Serial.print("Calibrating... ");
      Serial.print((millis() - startTime) / 1000);
      Serial.println(" seconds elapsed");
    }
  }
  
  // Calculate thresholds
  AVG_VALUE = (CALIBRATED_MAX_VALUE + CALIBRATED_MIN_VALUE) / 2;
  DEAD_ZONE_W = AVG_VALUE + (CALIBRATED_MAX_VALUE - AVG_VALUE) * 0.2;
  DEAD_ZONE_B = AVG_VALUE - (AVG_VALUE - CALIBRATED_MIN_VALUE) * 0.2;
  
  Serial.println("----------------------------------------------------");
  Serial.println("Calibration Results:");
  Serial.println("----------------------------------------------------");
  Serial.print("Maximum sensor value (white surface): ");
  Serial.println(CALIBRATED_MAX_VALUE);
  Serial.print("Minimum sensor value (black line): ");
  Serial.println(CALIBRATED_MIN_VALUE);
  Serial.print("Average value: ");
  Serial.println(AVG_VALUE);
  Serial.print("White threshold: ");
  Serial.println(DEAD_ZONE_W);
  Serial.print("Black threshold: ");
  Serial.println(DEAD_ZONE_B);
  Serial.println("----------------------------------------------------");
  Serial.println("Calibration complete. Ready to follow line.");
  Serial.println("----------------------------------------------------");
  delay(1000);
}
