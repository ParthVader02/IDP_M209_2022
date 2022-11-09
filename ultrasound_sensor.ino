int ultrasoundSensorPin = 8

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
  pinMode(ultrasoundSensorPin, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("started");
  int ultrasoundSensor; 
  ultrasoundSensor = digitalRead(ultrasoundSensorPin);
  bool coarseTrue;
  if (ultrasoundSensor < 10){
    Serial.println("Coarse");
    coarseTrue = true;
  }
  else{
    Serial.println("Dense");
    coarseTrue = false;
  }
  Serial.println("ended");
}
