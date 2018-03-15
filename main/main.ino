/*
Example of Zumo with IR-reflectance array, LED,servo and ultrasonic sensor mounted in front.
Zumo goes forward in constant speed  until the sonar detects something within maxRange.
If within range: the LED is turned on, Zumo turns slighly away (left) from object detected
while turning servo forth and back (0-180) in steps.
If border (black tape) is found, it turns either side.
Pushbuttom must be pressed to start Zumo.    
    Sonar:  echoPin = 0
            triggerPin = 1
    LED:    ledPin = 2 
    Servo:  servoPin = 6
    
*/
#include <NewPing.h>
#include <NewServo.h>

#include <ZumoMotors.h>
#include <Pushbutton.h>
#include <QTRSensors.h>
#include <PLab_ZumoMotors.h>
#include <ZumoReflectanceSensorArray.h>


// this might need to be tuned for different lighting conditions, surfaces, etc.
#define QTR_THRESHOLD  1900 // 
  
// these might need to be tuned for different motor types
#define REVERSE_SPEED     200 // 0 is stopped, 400 is full speed
#define TURN_SPEED        200
#define FORWARD_SPEED     240
#define REVERSE_DURATION  200 // ms
#define TURN_DURATION     300 // ms
#define LEFT 1  // turn direction
#define RIGHT 2

PLab_ZumoMotors PLab_motors;

const int echoPin = 0; 
const int triggerPin = 1; 
// Max distance (in cm) of interest
const int maxDistance = 40;  
const int ledPin = 2;
const int servoPin = 6;
const int mid = 73;

#define NUM_SENSORS 6
unsigned int sensor_values[NUM_SENSORS];
// Initialize "sonar" with trigger pin, echo pin and max distance,
// and then servo, motor, button and IR-sensor array
NewPing sonar(triggerPin, echoPin, maxDistance);
NewServo myServo;      
ZumoMotors motors;
Pushbutton button(ZUMO_BUTTON); 
// Tell Zumo that we have no emitter-pin
ZumoReflectanceSensorArray sensors(QTR_NO_EMITTER_PIN); 
 
 
void setup() {
   pinMode(ledPin,OUTPUT);
   myServo.attach(servoPin); 
   button.waitForButton();
   //motors.setSpeeds(FORWARD_SPEED,FORWARD_SPEED);
}

void loop() {
  //motors.setSpeeds(FORWARD_SPEED,FORWARD_SPEED);
  //servoSweepAndKeepLines()
  servoSweepTest();
}

void keepLines(){
       // Read IR-sensors and check if border detected
   sensors.read(sensor_values);
  if (sensor_values[0] < QTR_THRESHOLD)
  {
    // if leftmost sensor detects line, reverse and turn to the right
    turn(RIGHT);
  }
  if (sensor_values[5] < QTR_THRESHOLD)
  {
     // if rightmost sensor detects line, reverse and turn to the left
    turn(LEFT);
  }
}

void servoSweepAndKeepLines(){
  //Sweep left
  for(int i=0; i<163; i+=82){
  keepLines();
  myServo.write(i);
  unsigned int time = sonar.ping();
  float distance = sonar.convert_cm(time);
  if (!distance > 0) {
   // No object in front
   digitalWrite(ledPin,LOW); 
   motors.setSpeeds(FORWARD_SPEED,FORWARD_SPEED);
 } 
 else {
   // Object detected
   digitalWrite(ledPin,HIGH);
   delay(500);
   digitalWrite(ledPin,LOW);
 }
 }
 //Sweep right
  for(int i=162; i>-1; i-=82){
  keepLines();
  myServo.write(i);
  unsigned int time = sonar.ping();
  float distance = sonar.convert_cm(time);
  if (!distance > 0) {
   digitalWrite(ledPin,LOW); 
   motors.setSpeeds(FORWARD_SPEED,FORWARD_SPEED);
 } 
 else {
   digitalWrite(ledPin,HIGH);
   delay(500);
   digitalWrite(ledPin,LOW);
 }
 }
}
  

void servoSweepTest(){
  //Sweep left
  for(int i=0; i<163; i+=41){
  myServo.write(i);
  unsigned int time = sonar.ping();
  float distance = sonar.convert_cm(time);
  if (!distance > 0) {
   // No object in front
   digitalWrite(ledPin,LOW); 
 } 
 else {
   // Object detected
   digitalWrite(ledPin,HIGH);
   delay(500);
   digitalWrite(ledPin,LOW);
 }
 }
 //Sweep right
  for(int i=162; i>-1; i-=41){
  myServo.write(i);
  unsigned int time = sonar.ping();
  float distance = sonar.convert_cm(time);
  if (!distance > 0) {
   // No object in front
   digitalWrite(ledPin,LOW); 
 } 
 else {
   // Object detected
   digitalWrite(ledPin,HIGH);
   delay(500);
   digitalWrite(ledPin,LOW);
 }
  }
}

void turn(int direction){ 
//
// Move Zumo backwards, then turn in given direction,
// and continue forward
//
   // Check wich direction to turn
   if (direction == RIGHT)
   {
    motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
    delay(REVERSE_DURATION);
    motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
    delay(TURN_DURATION);
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
  }
  else // turn left
  {
    motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
    delay(REVERSE_DURATION);
    motors.setSpeeds(-TURN_SPEED, TURN_SPEED);
    delay(TURN_DURATION);
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
  }  
}

