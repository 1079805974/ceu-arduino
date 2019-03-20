##include "Wire.h"

void initGY30(){
  Wire.begin();
  Wire.beginTransmission(0b0100011);
  Wire.write(0b00000001);
  Wire.endTransmission();    
}

int readGY30(){
  int val = 0;

  Wire.beginTransmission(0b0100011);
  Wire.write(0b00000111);
  Wire.endTransmission();
 
  Wire.beginTransmission(0b0100011);
  Wire.write(0b00100000);
  Wire.endTransmission();
  delay(120);

  Wire.requestFrom(0b0100011, 2); // 2byte every time
  for (val = 0; Wire.available() >= 1; ) {
    char c = Wire.read();
    val = (val << 8) + (c & 0xFF);
  }
  val = val / 1.2;
  return val;
}