#include <AccelStepper.h>

#define pulsex 6
#define dirx 7
#define pulsey 5
#define diry 4
#define MotorInterfaceType 1

// --------------------------------------- Define Variables -----------------------------------------

// Define stepper motors and their connections
AccelStepper stepperx(MotorInterfaceType, pulsex, dirx);
AccelStepper steppery(MotorInterfaceType, pulsey, diry);

// Define microswitches
const int xMicroswitchPin = 2;
const int yMicroswitchPin = 0;
const int homingSpeed = 1000;

// Defining Grid Parameters
// Note the steps are only for the current driver switch settings on the arduino
// const long xtot_steps = -11000;  // 1 high, 2 low, 3 low  11500
// const long ytot_steps = -81000; // 1-3 low

// The values below is for a single line scan (steps change depending on how many stops)
const long xtot_steps = -3000;  
const long ytot_steps = -12000; 
const int numRows = 1;
const int numCols = 30;

const long xSpacing = (xtot_steps / numCols);
const long ySpacing = (ytot_steps / numRows);

// Define the pause duration (24 hours = 86400000 milliseconds)
// const unsigned long pauseDuration = 86400000;
// const unsigned long pauseDuration = 3600000;
const unsigned long pauseDuration = 12000000; // 3.3 hours per point
// const unsigned long pauseDuration = 5000;


// Pulse Separator Information
// Incrementer for number of pulses based on location of scanner
int pulseCount = 1;
const unsigned long pulseLength = 5000;

// Setup for long term holds during measurment
unsigned long measurementStartTime;
bool measurementInProgress = false;
bool moveToNextPoint = false;


// --------------------------------------- Homing -------------------------------------------------
//Must home y-axis first to void vertical mounts on table
void homeYAxis()
{
  Serial.println("Homing the Y axis...");
  steppery.setSpeed(homingSpeed);
  while(digitalRead(yMicroswitchPin) != LOW)
  {
    steppery.runSpeed();
  }

  //Stops motor at microswitch and sets y position to 0
  steppery.stop();
  steppery.setCurrentPosition(0);
  Serial.println("Y axis home position reached.");
}

void homeXAxis()
{
  Serial.println("Homing the X axis...");
  stepperx.setSpeed(homingSpeed);
  while(digitalRead(xMicroswitchPin) != LOW)
  {
    stepperx.runSpeed();
  }

  //Stops motor at microswitch and sets x position to 0
  stepperx.stop();
  stepperx.setCurrentPosition(0);
  Serial.println("X axis home position reached.");
}

// ---------------------------------------Digital Pulse Trigger -----------------------------------------
// Function to generate a digital pulse
void generatePulse(int pulsePin, unsigned long pulseDuration) {
  // Set the pulse pin as an output
  pinMode(pulsePin, OUTPUT);

  // Generate a single pulse
  digitalWrite(pulsePin, HIGH);
  delayMicroseconds(pulseDuration);
  digitalWrite(pulsePin, LOW);
}

// --------------------------------------- Main Code ---------------------------------------------------
void setup() 
{
  // Max speed and accel for each stepper motor
  // Speed = steps/sec
  stepperx.setMaxSpeed(800);
  stepperx.setAcceleration(500);
  // stepperx.moveTo(12800);

  steppery.setMaxSpeed(800);
  steppery.setAcceleration(500);
  // steppery.moveTo(6400);

  // Initialize the microswitch pin as an input
  pinMode(xMicroswitchPin, INPUT_PULLUP);
  pinMode(yMicroswitchPin, INPUT_PULLUP);
  
  //For debugging
  Serial.begin(9600);

  // Homing each axis
  Serial.println("");

  Serial.println(xSpacing);
  Serial.println(ySpacing);


  homeYAxis();
  homeXAxis();
}

void loop() 
{
  static int currentRow = 3;
  static int currentCol = 0;

  
  if(!measurementInProgress)
  {
    long targetX;
    //This generates a snake pattern style of scanning
    if(currentRow % 2 == 0){
      targetX = currentCol * xSpacing;  //Left to Right for even rows
    }
    else{
      targetX = (numCols -1 - currentCol) * xSpacing; //Right to Left of odd rows
    }
    long targetY = currentRow * ySpacing;
    Serial.println(targetY);


    stepperx.moveTo(targetX);
    steppery.moveTo(targetY);

    //waits to start measurement until steppers reach position
    while(stepperx.distanceToGo() != 0 || steppery.distanceToGo() !=0)
    {
      stepperx.run();
      steppery.run();
    }

    //Time to start the measurement
    measurementStartTime = millis();
    measurementInProgress = true;
    for (int i = 0; i < 1; i++) 
    {
      generatePulse(9, pulseLength);
      delay(500); // Short delay between pulses
    }
    Serial.println("Measurement in Progress. Holding for time.");
    Serial.print("Pulse Count: ");
    Serial.println(pulseCount);
  }

  //Checks to see if the pause duration has passed
  if(measurementInProgress && (millis() - measurementStartTime >= pauseDuration))
  {
    measurementInProgress = false;
    moveToNextPoint = true;
  }
  
  //If moveToNextPoint = true... kicks to this function to make a measurement at the next point
  //resets once all point finished
  if(moveToNextPoint)
  {
    currentCol++;
    pulseCount++;
    Serial.print("Current Column:" );
    Serial.println(currentCol);
    if(currentCol >= numCols)
    {
      currentCol = 0;
      currentRow++;
      Serial.print("Current Row: ");
      Serial.println(currentRow);

      if(currentRow >= numRows)
      {
        currentRow = 0;
        pulseCount = 0;
      Serial.println("Finished. Returning to Home.");

      }
    }
    moveToNextPoint = false;
  }
}
