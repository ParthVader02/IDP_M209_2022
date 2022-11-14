int echoPin = 2;
int trigPin = 3;
int greenLedPin = 4;
int redLedPin = 5;

void setup() { 
 Serial.begin(9600);           // set up Serial library at 9600 bps

  //set ultrasonics sensor and led pins to read and write
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
}

void loop() {
  ultrasonic_read();
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
  Serial.print("Distance: ");
  Serial.println(mean_distance);

  // turn red or green LED on
  if (mean_distance < 15) {
      digitalWrite(greenLedPin, HIGH);
      digitalWrite(redLedPin, LOW);
  } else {
      digitalWrite(greenLedPin, LOW);
      digitalWrite(redLedPin, HIGH);
  }
}
