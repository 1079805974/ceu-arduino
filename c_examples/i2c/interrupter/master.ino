#define LED 13

bool started = false;

void setup() {
  sei();
  i2c_start();
  pinMode(LED, OUTPUT);
}

void loop() {
  
}

void i2c_start(void)
{
  _SFR_BYTE(sfr) &= ~_BV(bit)
}

ISR(ADC_vect) {
  if(ADC > NIGHT){
    digitalWrite(LED, HIGH);
  }else{
    digitalWrite(LED, LOW);
  }

}
