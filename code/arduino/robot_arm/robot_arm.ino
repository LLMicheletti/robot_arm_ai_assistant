#include <Servo.h>

//Right Servo
#define RETRACTED 60
#define EXTENDED 0

//Left Servo
#define UP 102
#define DOWN 2

//Claw servo
#define OPEN 115
#define CLOSED 15

//Base servo
#define RIGHT 0
#define STRAIGHT 90
#define LEFT 180

Servo base_servo;
Servo left_servo;
Servo right_servo;
Servo claw_servo;

bool running = false;
String last_command = "";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  base_servo.attach(A0);
  left_servo.attach(A4);
  right_servo.attach(A3);
  claw_servo.attach(A1);
  base_servo.write(STRAIGHT);
  left_servo.write(UP);
  right_servo.write(RETRACTED);
  claw_servo.write(OPEN);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    if (input != last_command) {
      last_command = input;
      running = true;
    }

    if (running) {
      if (input.indexOf("SAY YES") != -1) {
        agree();
      } else if (input.indexOf("SAY NO") != -1) {
        disagree();
      } else if (input.indexOf("SHAKE HANDS") != -1) {
        shake_hands();
      } else if (input.indexOf("GRAB STRAIGHT") != -1) {
        grab_straight();
      }
      running = false;
    }
  }
}

void grab_straight() {
  int pos_left = UP;
  int pos_right = RETRACTED;
  int pos_claw = OPEN;
  int pos_base = STRAIGHT;
  for (int i = 0; i <= 100; i ++) {
    left_servo.write(pos_left - i);
    right_servo.write(pos_right - 0.6*i);
    claw_servo.write(pos_claw - i);
    delay(70);
  }
  for (int i=0; i <= 100; i ++) {
    left_servo.write(DOWN + i);
    right_servo.write(EXTENDED + 0.6*i);
    delay(70);
  }
}

void agree() {
  int pos_left = UP;
  int pos_right = RETRACTED;
  int pos_claw = OPEN;
  int pos_base = STRAIGHT;
  for (int i = 0; i <= 70; i ++) {
    pos_right = RETRACTED - i;
    pos_claw = CLOSED;
    right_servo.write(pos_right);
    claw_servo.write(pos_claw);
    delay(30);
  }
  for (int i = CLOSED; i <= OPEN; i++) {
    claw_servo.write(i);
    delay(5);
  }
  for (int i = OPEN; i >= CLOSED; i--) {
    claw_servo.write(i);
    delay(5);
  }
  for (int i = 0; i <= 70; i ++) {
    right_servo.write(pos_right + i);
    claw_servo.write(CLOSED);
    delay(30);
  }
}

void disagree() {
  int pos_left = UP;
  int pos_right = RETRACTED;
  int pos_claw = OPEN;
  int pos_base = STRAIGHT;
  for (int i = 0; i <= 70; i ++) {
    pos_right = RETRACTED - i;
    pos_claw = CLOSED;
    right_servo.write(pos_right);
    claw_servo.write(pos_claw);
    delay(50);
  }
  for (int i = 0; i <= 40; i++) {
    pos_claw = CLOSED + i*2;
    pos_base = STRAIGHT - i;
    base_servo.write(pos_base);
    claw_servo.write(pos_claw);
    delay(10);
  }
  for (int i = 0; i <= 80; i++) {
    base_servo.write(pos_base + i);
    claw_servo.write(pos_claw - i);
    delay(10);
  }
  for (int i = 0; i <= 70; i ++) {
    base_servo.write(STRAIGHT);
    right_servo.write(pos_right + i);
    claw_servo.write(CLOSED);
    delay(50);
  }
}

void shake_hands() {
  int pos_left = UP;
  int pos_right = RETRACTED;
  int pos_claw = CLOSED;
  int pos_base = STRAIGHT;
  for (int i = 0; i <= 70; i ++) {
    pos_right = RETRACTED - i;
    pos_claw = CLOSED + i;
    right_servo.write(pos_right);
    claw_servo.write(pos_claw);
    delay(50);
  }
  for (int i = pos_claw; i >= CLOSED + 15; i --) {
    claw_servo.write(i);
    delay(5);
  }
  for (int i = 0; i <= 30; i ++) {
    pos_right = RETRACTED - 70 + i;
    pos_left = UP - i/10;
    right_servo.write(pos_right);
    left_servo.write(pos_left);
    delay(30);
  }
  for (int i = 0; i <= 30; i ++) {
    pos_right = RETRACTED - 40 - i;
    pos_left = UP - 3 + i/10;
    right_servo.write(pos_right);
    left_servo.write(pos_left);
    delay(30);
  }
  for (int i = CLOSED; i <= OPEN; i ++) {
    claw_servo.write(i);
    delay(5);
  }
  for (int i = 0; i <= 70; i ++) {
    right_servo.write(pos_right + i);
    delay(30);
  }
}
