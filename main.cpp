#include <Stepper.h>

const int stepsPerRevolution = 200;  // change this to fit the number of steps per revolution for your motor
Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11); // Pins connected to the motor driver

const int entrySensorPin = 2; // IR sensor to detect vehicles entering
const int exitSensorPin = 3;  // IR sensor to detect vehicles leaving
const int gateOpenPosition = 90; // Steps needed to open the gate
const int gateClosePosition = -90; // Steps needed to close the gate

int maxSlots = 1; // Limit for the number of vehicles allowed
int currentSlots = 0; // Current number of vehicles inside the parking area
bool gateOpen = false;

void setup() {
  pinMode(entrySensorPin, INPUT);
  pinMode(exitSensorPin, INPUT);
  myStepper.setSpeed(60); // Adjust the speed of the stepper motor
  Serial.begin(9600);
}

void loop() {
  int entrySensorState = digitalRead(entrySensorPin);
  int exitSensorState = digitalRead(exitSensorPin);

  // Vehicle entering
  if (entrySensorState == HIGH && currentSlots < maxSlots && !gateOpen) {
    openGate();
    currentSlots++;
    delay(1000); // Debounce delay
    closeGate();
  }

  // Vehicle leaving
  if (exitSensorState == HIGH && currentSlots > 0 && gateOpen) {
    openGate();
    currentSlots--;
    delay(1000); // Debounce delay
    closeGate();
  }

  // Display the current status
  Serial.print("Available slots: ");
  Serial.println(maxSlots - currentSlots);

  delay(500);
}

void openGate() {
  myStepper.step(gateOpenPosition); 
  gateOpen = true;
  Serial.println("Gate opened");
}

void closeGate() {
  myStepper.step(gateClosePosition);
  gateOpen = false;
  Serial.println("Gate closed");
}
