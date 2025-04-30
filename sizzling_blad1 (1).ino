#include <Servo.h>

// Pin Definitions
const int slideSwitchPin = 2;    // Slide switch input pin (use digital pin)
const int ledPin = 6;            // LED pin
const int servoPin = 9;          // Servo motor pin
const int buzzerPin = 5;         // Buzzer pin

unsigned long slideSwitchStartTime = 0;
bool slideSwitchActivated = false;
bool alarmActivated = false;

Servo driverServo;

void setup() {
  pinMode(slideSwitchPin, INPUT);   // Set the slide switch pin as input
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  driverServo.attach(servoPin);
  driverServo.write(0); // Initialize servo at 0 degrees

  Serial.begin(9600);
  Serial.println("System Initialized: Anti-Sleep Alarm for Driver");
}

void loop() {
  int slideSwitchState = digitalRead(slideSwitchPin);  // Read the state of the slide switch
  Serial.print("Slide Switch State: ");
  Serial.println(slideSwitchState);

  // If the slide switch is activated (ON)
  if (slideSwitchState == HIGH) {
    if (!slideSwitchActivated) {
      slideSwitchActivated = true;
      slideSwitchStartTime = millis(); // Start the timer when the switch is turned on
    } else {
      // Check if the switch has been ON for more than 5 seconds
      if ((millis() - slideSwitchStartTime) >= 5000 && !alarmActivated) {
        activateAlarm();
        alarmActivated = true;
      }
    }
  } else {
    // Reset the alarm if the switch is turned off
    slideSwitchActivated = false;
    alarmActivated = false;
    deactivateAlarm();
  }

  delay(100);
}

// Function to activate LED, buzzer, and move servo motor once
void activateAlarm() {
  // Turn on LED
  digitalWrite(ledPin, HIGH);

  // Activate buzzer
  digitalWrite(buzzerPin, HIGH);
  delay(2000); // Buzzer stays on for 2 seconds
  
  // Move servo motor once
  driverServo.write(90);
  delay(1000); // Hold position for 1 second
  driverServo.write(0);

  Serial.println("Drowsiness Detected! LED ON, Buzzer ON, Servo Activated");
}

// Function to turn off LED and buzzer
void deactivateAlarm() {
  digitalWrite(ledPin, LOW);
  digitalWrite(buzzerPin, LOW);
}