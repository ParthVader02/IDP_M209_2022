#include <Servo.h>

Servo myservo;
int pos = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  myservo.attach(10);

}

void loop() {
  // put your main code here, to run repeatedly:

}

void pickup(){  
  //moves grabber down 
  for (pos = 90; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(250);                       // waits 15 ms for the servo to reach the position
}}


void dropoff(char colour){
  //moves grabber up when within range
  for (pos = 0; pos <= 90; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(250);   

}}
