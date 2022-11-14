#include <Adafruit_MotorShield.h>

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield();

// Select motor ports M1,2,3,4
Adafruit_DCMotor *left = AFMS.getMotor(3);
Adafruit_DCMotor *right = AFMS.getMotor(4);

//initialise ultrasonic sensor and LED pins
int echoPin = 2;
int trigPin = 3;
int greenLedPin = 4;
int redLedPin = 5;

//initialise line_sensor pins
int lineSensorpin_1 = 8;
int lineSensorpin_2 = 9;
int lineSensorpin_3 = 10;
int lineSensorpin_4 = 11;


void setup() {

 //run startup sequence, flash some LEDs to tell us its alive

  Serial.begin(9600);           // set up Serial library at 9600 bps

  //set ultrasonics sensor and led pins to read and write
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  
  //set digital line sensor pins to read
  pinMode(lineSensorpin_1, INPUT); 
  pinMode(lineSensorpin_2, INPUT);
  pinMode(lineSensorpin_3, INPUT); 
  pinMode(lineSensorpin_4, INPUT);

//check if motorshield is connected
  if (!AFMS.begin()) {         // create with the default frequency 1.6KHz
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }
  Serial.println("Motor Shield found.");

//Left
  // Set the speed to start, from 0 (off) to 255 (max speed)
  left->setSpeedFine(2000);
  left->run(RELEASE);
//RIght
  // Set the speed to start, from 0 (off) to 255 (max speed)
  right->setSpeedFine(2000);
  right->run(RELEASE);
}

volatile int startFlag = 0;

void loop() {
  //initialise static counters, for start sequence and junction count
  static int junction_count = 0;
  static int start_count = 0;
  int line_detect = 0;

  //initialise sensors for readable code and read pins
  int left_lineSensor = digitalRead(lineSensorpin_1); //8
  int right_lineSensor = digitalRead(lineSensorpin_2); //9
  int v_left_lineSensor = digitalRead(lineSensorpin_3); //10
  int v_right_lineSensor = digitalRead(lineSensorpin_4); //11

  //get to main line and rotate 90 deg clockwise
  if (startFlag == 0){
    if (left_lineSensor ==1 && right_lineSensor ==1 && v_left_lineSensor ==1 && v_right_lineSensor ==1){ // 1111
      start_count++;
    }
    if (left_lineSensor ==0 && right_lineSensor ==0 && v_left_lineSensor ==0 && v_right_lineSensor ==0){ //0000
      line_detect = 1; //main line detected
    }
    start_to_line(start_count, line_detect);
  }
  
  if(startFlag ==1){

  //line follow algorithm, while checking for junctions

  //if junctions seen, do things

  //if block there, run ultrasound sensor, LED to show type, save type to memory
  ultrasonic_read();
  // to check if block exists -> CV

  //run servo motor to capture block

  //go back to main line and follow line again 

  //when no line detected, keep going straight till line detected (tunnel)

  //from block type, go to correct bin and deposit

  //reverse to main line, check if more blocks (CV), if blocks -> collect, if not -> go back home

  }
}

void line_follow(){  

}

void start_to_line(int count, int line_detect){
  //go forward until 1111
  left->run(BACKWARD);
  right->run(FORWARD);

  if(count == 1){ //line follow between start and main line
    line_follow();
  }

  else if(count ==2){ //once main line reached
    left->run(RELEASE); //stop
    right->run(RELEASE);

    while(line_detect !=1){
      left->run(BACKWARD); //rotate until line detected
      right->run(FORWARD);
    }
    startFlag = 1;
  }
}

void ultrasonic_read() {
  long duration;
  int distance[100];

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
    } else {
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
      digitalWrite(greenLedPin, HIGH);
      digitalWrite(redLedPin, LOW);
  } else {
      digitalWrite(greenLedPin, LOW);
      digitalWrite(redLedPin, HIGH);
  }
}
