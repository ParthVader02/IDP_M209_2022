int lineSensorpin_1 = 8;
int lineSensorpin_2 = 9;

void setup() {
  // put your setup code here, to run once:
   Serial.begin(9600);
  pinMode(lineSensorpin_1, INPUT); // 1 = white, 0 = black
  pinMode(lineSensorpin_2, INPUT);
  int left_lineSensor = digitalRead(lineSensorpin_1); 
  int right_lineSensor = digitalRead(lineSensorpin_2);
  start_sequence(left_lineSensor, right_lineSensor);
 
}

void loop() {
  // sets robot to line follow mode
  //Pickup: Junct 4,5,6
  //Dropoff: Junct 9, 1
  bool onReturn 
  int junctCount = 1
  int left_lineSensor = digitalRead(lineSensorpin_1); 
  int right_lineSensor = digitalRead(lineSensorpin_2);
  delay(10); //reads line sensors every 10ms
  if (left_lineSensor == 0 && right_lineSensor == 0))){ //if no white detected on middle 2 sensors
    Serial.println("FORWARD");
  }
  else if(left_lineSensor == 1 && right_lineSensor ==0 ){
    Serial.println("LEFT");
    junctCount++;
  }
  else if((left_lineSensor == 0 && right_lineSensor == 1) && (junctCount != 1)){
    Serial.println("RIGHT");
    junctCount++;
  if (junctCount == 9){
    junctCount = 0;
  }
  }
}
void start_sequence(int left,int right){
  int count = 0;
    while(count < 2){
      Serial.println("FORWARD");}
      if (left == 1 && right == 1){
        count++;}
      Serial.println(count);
    Serial.println("RIGHT");
}
