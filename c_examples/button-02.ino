
void setup() {
  pinMode(A0, INPUT);
  pinMode(2, INPUT);
  Serial.begin(9600);
  attachInterrupt(0, changeStatus, readADC);
}

void readADC(){
  int i = analogRead(A0);
  Serial.println(i);
}

void loop(){}