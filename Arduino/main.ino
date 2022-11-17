#include <Adafruit_MotorShield.h>
#include <Servo.h>

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield();

// Select motor ports M1,2,3,4
Adafruit_DCMotor *left = AFMS.getMotor(3);
Adafruit_DCMotor *right = AFMS.getMotor(4);

//initialise line_sensor pins
int lineSensorpin_1 = 8;
int lineSensorpin_2 = 9;
int lineSensorpin_3 = 10;
int lineSensorpin_4 = 11;

int echoPin = 2;
int trigPin = 3;
int greenLedPin = 4;
int redLedPin = 5;

Servo myservo;
int pos = 0;


void setup() {

 //run startup sequence, flash some LEDs to tell us its alive

  Serial.begin(9600);           // set up Serial library at 9600 bps
  //set digital line sensor pins to read
  pinMode(lineSensorpin_1, INPUT); 
  pinMode(lineSensorpin_2, INPUT);
  pinMode(lineSensorpin_3, INPUT); 
  pinMode(lineSensorpin_4, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);

  myservo.attach(9);

//check if motorshield is connected
  if (!AFMS.begin()) {         // create with the default frequency 1.6KHz
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }
  Serial.println("Motor Shield found.");

//Left
  // Set the speed to start, from 0 (off) to 255 (max speed)
  left->setSpeed(220);
  left->run(RELEASE);
//RIght
  // Set the speed to start, from 0 (off) to 255 (max speed)
  right->setSpeed(150);
  right->run(RELEASE);
}

int ultrasonic_read() {
  long duration;
  int distance[100];
  int block_type;
  for (int i = 0; i < 100; i++) {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    //calculate duration of pulse and distance to object
    duration = pulseIn(echoPin, HIGH);

    //check if input is valid, if not then loop again
    int dis = duration * 0.034/2;
    if (dis < 60) {
      distance[i] = dis;
    } 
    else {
      i--;
    }
  }

  //calculate mean distance
  int mean_distance = 0;
  for (int i = 0; i < 100; i++) {
    mean_distance += distance[i];
  }
  mean_distance = mean_distance / 100;

  // turn red or green LED on
  if (mean_distance < 15) {
      block_type = 0; //low density
      digitalWrite(greenLedPin, HIGH);
      digitalWrite(redLedPin, LOW);
  } 
  else {
      block_type =1; //high density
      digitalWrite(greenLedPin, LOW);
      digitalWrite(redLedPin, HIGH);
  }
  return block_type;
}

volatile int startFlag = 1;
volatile int stop_follow = false;
void loop() {
  //initialise static counters, for start sequence and junction count
  static int junction_count = 0; 
  static int start_count = 0;
  int line_detect = 0;
  int block_type;
  bool hasBlock;
  int desiredJunction;
  //initialise sensors for readable code and read pins
  int right_lineSensor = digitalRead(lineSensorpin_1); //8
  int v_right_lineSensor = digitalRead(lineSensorpin_2); //9
  int left_lineSensor = digitalRead(lineSensorpin_3); //10
  int v_left_lineSensor = digitalRead(lineSensorpin_4); //11
  delay(10);
  Serial.println(v_left_lineSensor);
  //Serial.println(right_lineSensor);
  //Serial.println(v_left_lineSensor);
  //Serial.println(v_right_lineSensor);
  //get to main line and rotate 90 deg clockwise
  if (startFlag == 0){
    if (left_lineSensor ==1 && right_lineSensor ==1 && v_left_lineSensor ==1 && v_right_lineSensor ==1){ // 1111
    delay(100);
      start_count++;
      Serial.println(start_count);
    }
    start_to_line(start_count, left_lineSensor);
  }
  
  if(startFlag ==1){

  //Serial.println("start_up done");

  // Query PC if blocks are seen from camera, and which junction they are at

  //line follow algorithm, while checking for junctions#

  if(right_lineSensor ==1  && v_right_lineSensor ==1){ //right junction
    junction_count++;
  }

  else if(left_lineSensor ==1  && v_left_lineSensor ==1){ //left junction
    junction_count++;
  }

  else if(left_lineSensor ==1 && right_lineSensor ==1 && v_left_lineSensor ==1 && v_right_lineSensor ==1){ //middle
    junction_count++;    
  }

  junction(junction_count, hasBlock, block_type, desiredJunction);
  

  while (stop_follow != true){
    line_follow(left_lineSensor, right_lineSensor, v_left_lineSensor, v_right_lineSensor);
  }
  
  // if near block
  //run ultrasound sensor, LED to show type, save type
  block_type = ultrasonic_read();
  //run servo motor to capture block

  //go back to main line and follow line again 

  //when no line detected, keep going straight till line detected (tunnel)

  //from block type, go to correct bin and deposit

  //reverse to main line, check if more blocks (CV), if blocks -> collect, if not -> go back home
  }
}

void line_follow(int left_lineSensor, int right_lineSensor, int v_left_lineSensor, int v_right_lineSensor){
  int kp = 50;
  int error = getError(left_lineSensor, right_lineSensor, v_left_lineSensor, v_right_lineSensor);
  Serial.println(error);
  static int left_speed;
  static int right_speed;

  left_speed = 200 - kp*error;
  right_speed = 150 + kp*error;

  if(left_speed >255){
    left_speed = 255;
  }
  if(right_speed >255){
    right_speed = 255;
  }

  left->setSpeed(left_speed);
  right->setSpeed(right_speed);
  
  left->run(FORWARD);
  right->run(BACKWARD);
}

void start_to_line(int count, int left_lineSensor){
  //go forward until 1111
  if(count!=2){
Serial.println("GO FORWARD");
  left->run(FORWARD);
  right->run(BACKWARD);
  }
  else if(count ==2){ //once main line reached
    if(left_lineSensor !=1){
       left->run(FORWARD); //rotate until line detected
      right ->run(BACKWARD);
    }
    else{
    left ->fullOff();
    right ->fullOff();
     startFlag = 1;
    }
  }
}

int getError(int a, int b, int c, int d){
  int error;
  if(a==0 && b==0 && c==0 && d==0){
    error = 0;    
  }
  else if(a==1 && b==0 && c==0 && d==0){
    error = 2;
  }
  else if(a==0 && b==0 && c==1 && d==0){
    error = 4;
  }
  else if(a==0 && b==1 && c==0 && d==0){
    error = -2;
  }
  else if(a==0 && b==0 && c==0 && d==1){
    error = -4;
  }
  else{
    error = 0;
  }
  return error;
}

void junction(int count, bool hasBlock, int blockType, int desiredJunction){
  if (hasBlock == false){
    if (count == 2 && desiredJunction == 2){
      //turn left, pickup function
      // find coordinates of blue block 1 in x,y
      //if no blue in 2's xy co-ords continue forward
    }
    else if (count == 3 && desiredJunction == 3){
      //turn right, pickup function
      // find coordinates of blue block 2 in x,y
      //if no blue in 2's xy co-ords continue forward
    }
    else if (count == 4 && desiredJunction == 4)
    { //turn left, pickup function
      // find coordinates of blue block 3 in x,y
      //if no blue in 2's xy co-ords continue forward}



  }
  else if(hasBlock == true){
    //deposit block
    if (count == 1){
      if (blockType == 0){
      //low density green
      //continue forward
      }}
      else if(blockType == 1){
      //high density red
      //turn right
    }
    
    else if (count == 5){}
      if (blockType == 0){
      //low density green
      //turn right
      }else if(blockType == 1){
      //high density red
      //continue forward
    }
  //else turn 

  }
  
}}

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

