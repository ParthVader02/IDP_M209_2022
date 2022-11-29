#include <Adafruit_MotorShield.h>

// Create the motor shield object with the default I2C address
//Adafruit_MotorShield AFMS = Adafruit_MotorShield();
// Or, create it with a different I2C address (say for stacking)
// Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x61);

// Select which 'port' M1, M2, M3 or M4. In this case, M3
//Adafruit_DCMotor *left = AFMS.getMotor(3);
// You can also make another motor on port M4
//Adafruit_DCMotor *right = AFMS.getMotor(4);

int lineSensorpin_1 = 8;
int lineSensorpin_2 = 9;
int lineSensorpin_3 = 11;
int lineSensorpin_4 = 12;

void setup() {
  // put your setup code here, to run once:
   Serial.begin(9600);
  pinMode(lineSensorpin_1, INPUT); // 1 = white, 0 = black
  pinMode(lineSensorpin_2, INPUT);
   pinMode(lineSensorpin_3, INPUT); // 1 = white, 0 = black
  pinMode(lineSensorpin_4, INPUT);
/*
  if (!AFMS.begin()) {         // create with the default frequency 1.6KHz
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }
  Serial.println("Motor Shield found.");*/

//Left
  // Set the speed to start, from 0 (off) to 255 (max speed)
  //left->setSpeed(255);
  // turn on motor
  //left->run(RELEASE);
//RIght
  // Set the speed to start, from 0 (off) to 255 (max speed)
  //right->setSpeed(200);
  // turn on motor
  //right->run(RELEASE);
}

volatile int startFlag = 0;

void loop() {
  // sets robot to line follow mode
  //Pickup: Junct 4,5,6
  //Dropoff: Junct 9, 1
  bool onReturn;
  int junctCount = 0;
  int right_lineSensor = digitalRead(lineSensorpin_1); //8
  int v_right_lineSensor = digitalRead(lineSensorpin_2); //9
  int left_lineSensor = digitalRead(lineSensorpin_3); //11
  int v_left_lineSensor = digitalRead(lineSensorpin_4); //12
  delay(1000); //reads line sensors every 10ms

  Serial.println("right_lineSensor");
  Serial.println(right_lineSensor);
  Serial.println(v_right_lineSensor);
  Serial.println(left_lineSensor);
  Serial.println(v_left_lineSensor);
/*
  if(startFlag == 0){
    int count = 0;
    while(count < 2){
      left->run(FORWARD);
      right->run(BACKWARD);
      if (left == 1 && right == 1){
        count++;}
      Serial.println(count);
    }
    left->run(FORWARD);
    right->run(FORWARD);
    delay(1000);
    Serial.println("Start Sequence completed");
    startFlag = 1;
  }
  if (left_lineSensor == 0 && right_lineSensor == 0){ //if no white detected on middle 2 sensors
  //left->run(FORWARD);
  //right->run(BACKWARD);
  Serial.println("FORWARD");
  Serial.println(left_lineSensor);
  Serial.println(right_lineSensor);
    }
  else if(left_lineSensor == 1 && right_lineSensor ==0 ){
  //  left->run(FORWARD);
  //right->run(FORWARD);
    Serial.println("LEFT");
    Serial.println(left_lineSensor);
  Serial.println(right_lineSensor);
    junctCount++;
  }
  else if((left_lineSensor == 0 && right_lineSensor == 1) && (junctCount != 1)){
  //  left->run(BACKWARD);
  //right->run(BACKWARD);
    Serial.println("RIGHT");
    junctCount++;
    Serial.println(left_lineSensor);
  Serial.println(right_lineSensor);
  }    
  if (junctCount == 10){
    junctCount = 0;
  }*/
}

