#include <Servo.h>

// constants won't change
const int TRIG_PIN  = 2;  // Arduino pin connected to Ultrasonic Sensor's TRIG pin
const int ECHO_PIN  = 3;  // Arduino pin connected to Ultrasonic Sensor's ECHO pin
const int SERVO_PIN = 9; // Arduino pin connected to Servo Motor's pin
const int BUZZER_PIN = 4; // Arduino pin connected to Piezo Buzzer's pin

const int DISTANCE_THRESHOLD = 20; // centimeters
int pos = 0;    // variable to store the servo position

Servo servo; // create servo object to control a servo

// variables will change:
float duration_us, distance_cm;

void inline_assembly_buzzer(uint8_t pin, uint16_t frequency, uint16_t duration) {
  uint16_t delay_count = (1000000 / frequency) / 2;
  uint32_t loop_count = ((uint32_t)duration * 1000) / (delay_count * 2);

  asm volatile(
    "sbi %0, %1 \n\t" // Set buzzer pin to output
    "1: \n\t"
    "cbi %0, %1 \n\t" // Clear buzzer pin
    "rcall delay \n\t"
    "sbi %0, %1 \n\t" // Set buzzer pin
    "rcall delay \n\t"
    "subi %A2, lo8(-1) \n\t"
    "sbci %B2, hi8(-1) \n\t"
    "sbci %C2, hh8(-1) \n\t"
    "sbci %D2, hhh8(-1) \n\t"
    "brne 1b \n\t"
    "rjmp exit \n\t"
    "delay: \n\t"
    "ldi r24, %3 \n\t"
    "ldi r25, %4 \n\t"
    "2: \n\t"
    "sbiw r24, 1 \n\t"
    "brne 2b \n\t"
    "ret \n\t"
    "exit: \n\t"
    : // No output operands
    : "I" (_SFR_IO_ADDR(DDRB)), // DDRB register for pin direction
      "M" (pin - 8), // Buzzer pin number (assuming it's a digital pin on PORTB)
      "r" (loop_count),
      "M" (delay_count & 0xFF),
      "M" ((delay_count >> 8) & 0xFF)
    : "r24", "r25"
  );
}

void setup() {
  Serial.begin (9600);       // initialize serial port
  pinMode(TRIG_PIN, OUTPUT); // set arduino pin to output mode
  pinMode(ECHO_PIN, INPUT);  // set arduino pin to input mode
  pinMode(BUZZER_PIN, OUTPUT); // set arduino pin to output mode

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

  if(distance_cm < DISTANCE_THRESHOLD){
    servo.write(180); // rotate servo motor to 90 degree
    inline_assembly_buzzer(BUZZER_PIN, 5000, 500);
    delay(2500);
  }
  else {
    servo.write(90);  // rotate servo motor to 0 degree 
    digitalWrite(BUZZER_PIN, LOW);  // turn off Piezo Buzzer
  }

  // print the value to Serial Monitor
  Serial.print("distance: ");
  Serial.print(distance_cm);
  Serial.println(" cm");

  delay(500);
}
