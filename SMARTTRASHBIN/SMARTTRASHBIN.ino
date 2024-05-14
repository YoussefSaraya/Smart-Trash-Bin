#include <Servo.h>

// constants won't change
const int TRIG_PIN  = 2;  // Arduino pin connected to Ultrasonic Sensor's TRIG pin
const int ECHO_PIN  = 3;  // Arduino pin connected to Ultrasonic Sensor's ECHO pin
const int SERVO_PIN = 9; // Arduino pin connected to Servo Motor's pin
const int BUZZER_PIN = 4; // Arduino pin connected to Piezo Buzzer's pin
const int LED_PIN = 10; // Arduino pin connected to Piezo Buzzer's pin


const int DISTANCE_THRESHOLD = 20; // centimeters
int pos = 0;    // variable to store the servo position


Servo servo; // create servo object to control a servo

// variables will change:
float duration_us, distance_cm;

void setup() {
  Serial.begin (9600);       // initialize serial port
  pinMode(TRIG_PIN, OUTPUT); // set arduino pin to output mode
  pinMode(ECHO_PIN, INPUT);  // set arduino pin to input mode
    pinMode(BUZZER_PIN, OUTPUT); // set arduino pin to output mode
      pinMode(10, OUTPUT);

  servo.attach(SERVO_PIN);   // attaches the servo on pin 9 to the servo object
  servo.write(0);
}

void loop() {
  // generate 10-microsecond pulse to TRIG pin
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // measure duration of pulse from ECHO pin
  duration_us = pulseIn(ECHO_PIN, HIGH);
  // calculate the distance
  distance_cm = 0.017 * duration_us;
 /*for (pos = 0; pos <= 90; pos += 1) { // goes from 0 degrees to 180 degrees
        // in steps of 1 degree
    servo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(2500);                       // waits 15 ms for the servo to reach the position
  }
    for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    servo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15 ms for the servo to reach the position
  }
  }*/

  if(distance_cm < DISTANCE_THRESHOLD){
    
    servo.write(180); // rotate servo motor to 90 degree
    
    digitalWrite(BUZZER_PIN, HIGH); // turn on Piezo Buzzer
    tone(BUZZER_PIN, 5000,500);
     digitalWrite(10, HIGH);   // turn the LED on 
    delay(2500);
      digitalWrite(10, LOW);    // turn the LED off 

  }
  else
    servo.write(90);  // rotate servo motor to 0 degree 
        digitalWrite(BUZZER_PIN, LOW);  // turn off Piezo Buzzer
  

  // print the value to Serial Monitor
  Serial.print("distance: ");
  Serial.print(distance_cm);
  Serial.println(" cm");

  delay(500);
}
