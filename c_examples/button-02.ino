bool started = false;

void setup() {
  pinMode(A0, INPUT);
  pinMode(2, INPUT);
  Serial.begin(9600);
  attachInterrupt(0, changeStatus, CHANGE);
}

void loop() {
  delay(1000);
  if(started){
    int i = analogRead(A0);
    Serial.println(i);
  }
}

void changeStatus(){
  started = !started;
}