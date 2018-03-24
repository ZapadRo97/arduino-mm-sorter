#include <AccelStepper.h>
#include <Servo.h>
#define HALFSTEP 8

// Motor pin definitions
#define motorPin1  8     // IN1 on the ULN2003 driver 1
#define motorPin2  9     // IN2 on the ULN2003 driver 1
#define motorPin3  10     // IN3 on the ULN2003 driver 1
#define motorPin4  11     // IN4 on the ULN2003 driver 1

// Initialize with pin sequence IN1-IN3-IN2-IN4 for using the AccelStepper with 28BYJ-48
AccelStepper stepper1(HALFSTEP, motorPin1, motorPin3, motorPin2, motorPin4);
Servo myservo;

void setup() {
  stepper1.setMaxSpeed(3000.0);
  stepper1.setAcceleration(100.0);
  stepper1.setSpeed(3000);
  stepper1.moveTo(-10);
  pinMode(12, OUTPUT);
  digitalWrite(12, HIGH);
  
  myservo.attach(6);
  
  myservo.write(90);
}//--(end setup )---

void loop() {

  //Change direction when the stepper reaches the target position
  //if (stepper1.distanceToGo() == 0) {
  // stepper1.moveTo(-stepper1.currentPosition());
  //}
  
  stepper1.run();
}
