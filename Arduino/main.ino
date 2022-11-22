#include <Adafruit_MotorShield.h>

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield();

// Select motor ports M1,2,3,4
Adafruit_DCMotor *left = AFMS.getMotor(4);
Adafruit_DCMotor *right = AFMS.getMotor(3);

//initialise line_sensor pins
int lineSensorpin_1 = 8;
int lineSensorpin_2 = 9;
int lineSensorpin_3 = 11;
int lineSensorpin_4 = 12;

int buttonState = 0;
int buttonPin = 6;
int echoPin = 2;
int trigPin = 3;
int greenLedPin = 4;
int redLedPin = 5;
int amberLedPin = 7;


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
  pinMode(buttonPin, INPUT);
  pinMode(amberLedPin, OUTPUT);


  while (buttonState == 0) {
    buttonState = digitalRead(buttonPin);
    //Serial.println(buttonState);
  }


//check if motorshield is connected
  if (!AFMS.begin()) {         // create with the default frequency 1.6KHz
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }
  Serial.println("Motor Shield found.");

//Left
  // Set the speed to start, from 0 (off) to 255 (max speed)
  left->setSpeed(225);
  left->run(RELEASE);
//RIght
  // Set the speed to start, from 0 (off) to 255 (max speed)
  right->setSpeed(225);
  right->run(RELEASE);
}

 int startFlag = 1;
//volatile int stop_follow = false;
void loop() {
  //initialise static counters, for start sequence and junction count
  static int junction_count = 0; 
  static int start_count = 0;
  int line_detect = 0;
  int block_type;
  //initialise sensors for readable code and read pins
  int right_lineSensor = digitalRead(lineSensorpin_1); //8
  int v_right_lineSensor = digitalRead(lineSensorpin_2); //9
  int left_lineSensor = digitalRead(lineSensorpin_3); //11
  int v_left_lineSensor = digitalRead(lineSensorpin_4); //12
  delay(10);

  //get to main line and rotate 90 deg clockwise
  if (startFlag == 0){
    if (left_lineSensor ==1 && right_lineSensor ==1 && v_left_lineSensor ==1 && v_right_lineSensor ==1){ // 1111
    delay(200);
      start_count++;
      Serial.println(start_count);
    }
    start_to_line(start_count, left_lineSensor);
  }
  
  if(startFlag ==1){

  //Serial.println("start_up done");

  // Query PC if blocks are seen from camera, and which junction they are at

  //line follow algorithm, while checking for junctions#

  //junction(junction_count);

  if((v_right_lineSensor ==1 && right_lineSensor ==1)||(v_left_lineSensor ==1 && left_lineSensor ==1)){ //right junction
  delay(250);
  junction_count++;
  }
  line_follow(left_lineSensor, right_lineSensor, v_left_lineSensor, v_right_lineSensor);
    Serial.println(junction_count);
  
  // if near block
  //run ultrasound sensor, LED to show type, save type
  //block_type = ultrasonic_read();
  //run servo motor to capture block

  //go back to main line and follow line again 

  //when no line detected, keep going straight till line detected (tunnel)

  //from block type, go to correct bin and deposit

  //reverse to main line, check if more blocks (CV), if blocks -> collect, if not -> go back home
  }
}

void line_follow(int left_lineSensor, int right_lineSensor, int v_left_lineSensor, int v_right_lineSensor){
  int kp = 105;
  digitalWrite(amberLedPin, HIGH);
  digitalWrite(amberLedPin, LOW);
  int error = getError(left_lineSensor, right_lineSensor, v_left_lineSensor, v_right_lineSensor);
  //Serial.println(error);
  static int left_speed;
  static int right_speed;

  left_speed = 215 - kp*error;
  right_speed = 225 + kp*error;

  if(left_speed >255){
    left_speed = 245;
  }
  if(right_speed >255){
    right_speed = 255;
  }

  left->setSpeed(left_speed);
  right->setSpeed(right_speed);
  
  left->run(BACKWARD);
  right->run(BACKWARD);
}

void start_to_line(int count, int left_lineSensor){
  //go forward until 1111
  if(count!=2){
    digitalWrite(amberLedPin, HIGH);
      digitalWrite(amberLedPin, LOW);
  left->run(BACKWARD);
  right->run(BACKWARD);
  }
  else if(count ==2){ //once main line reached
  left->run(BACKWARD);
  right->run(BACKWARD);
  delay(200);
      left->run(BACKWARD); //rotate until line detected
      right ->run(FORWARD);
      delay(1000);
      Serial.println("line follow now");
     startFlag = 1;
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
  else if(a==0 && b==1 && c==0 && d==0){
    error = -2;
  }
  else if(a==0 && b==0 && c==1 && d==0){
    error = 3;
  }
  else if(a==0 && b==1 && c==0 && d==1){
    error = -3;
  }
  else{
    error = 0;
  }
  return error;
}

void junction(int count){
  if(count==1){

  }
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