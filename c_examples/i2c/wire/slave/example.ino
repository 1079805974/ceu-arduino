#include <Wire.h>

int LED = 13;

void setup()

{
  Wire.begin(4);                    
  Wire.onReceive(receiveEvent);     
  Wire.onRequest(requestEvent);
  pinMode(LED, OUTPUT);           
  Serial.begin(9600);
}

void loop()
{
  delay(100);
}

void receiveEvent(int howMany)   
{
  int c = Wire.read();      
  Serial.println(c);
  if (c == 1)
  {
    digitalWrite(LED, HIGH); 
  }
  else if (c == 0)
  {
    digitalWrite(LED, LOW); 
  }
}

void requestEvent() {
  Wire.write("hello"); 
}