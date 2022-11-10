#define ECHO 2
#define TRIG 3
#define redLED 5
#define greenLED 4

long duration;
int distance;

void setup() {
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);

  Serial.begin(9600);
}

bool denseBlock;

void loop() {
  digitalWrite(TRIG, LOW);

  delayMicroseconds(2);

  digitalWrite(TRIG, HIGH);

  delayMicroseconds(10);

  digitalWrite(TRIG, LOW);

  duration = pulseIn(ECHO, HIGH);

  distance = duration * 0.034/2;

  if (distance < 60){
    Serial.print("Distance: ");
    Serial.println(distance);

    if (distance < 15) {
      digitalWrite(greenLED, HIGH);
      digitalWrite(redLED, LOW);
      denseBlock = false
    } else {
      digitalWrite(greenLED, LOW);
      digitalWrite(redLED, HIGH);
      denseBlock = false
   //if(denseBlock == true){}
   //else{}
      
    }
    delay(1000);
  }
}
