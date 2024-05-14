#include <Servo.h>

// constants won't change
const int TRIG_PIN  = 2;
const int ECHO_PIN  = 3;
const int SERVO_PIN = 9;
const int BUZZER_PIN = 4;
const int LED_PIN = 10;

const int DISTANCE_THRESHOLD = 20;
int pos = 0;

Servo servo;

float duration_us, distance_cm;

void toggle_led(bool state) {
  asm volatile (
    "cpi %0, 0 \n\t"
    "breq turn_off \n\t"
    "sbi %1, %2 \n\t"
    "rjmp exit \n\t"
    "turn_off: \n\t"
    "cbi %1, %2 \n\t"
    "exit: \n\t"
    :
    : "r" (state), "I" (_SFR_IO_ADDR(PORTB)), "M" (LED_PIN - 8)
  );
}

void setup() {
  Serial.begin(9600);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  servo.attach(SERVO_PIN);
  servo.write(0);
}

void loop() {
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration_us = pulseIn(ECHO_PIN, HIGH);
  distance_cm = 0.017 * duration_us;

  if (distance_cm < DISTANCE_THRESHOLD) {
    servo.write(180);
    digitalWrite(BUZZER_PIN, HIGH);
    tone(BUZZER_PIN, 5000, 500);
    toggle_led(true);
    delay(2500);
    toggle_led(false);
  } else {
    servo.write(90);
    digitalWrite(BUZZER_PIN, LOW);
  }

  Serial.print("distance: ");
  Serial.print(distance_cm);
  Serial.println(" cm");

  delay(500);
}
