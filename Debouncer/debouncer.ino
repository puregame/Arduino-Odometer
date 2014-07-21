
int pushButton = 0;
int outPin = 2;

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  // make the pushButton's pin an input:
  pinMode(outPin, OUTPUT);
  digitalWrite(outPin, LOW);
  pinMode(pushButton, INPUT);
  digitalWrite(pushButton, HIGH);
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input pin:
  if(digitalRead(pushButton) == LOW){
    digitalWrite(outPin, HIGH);
    delay(10);
    digitalWrite(outPin, LOW);
    delay(40);
    Serial.print("0");

  }
}