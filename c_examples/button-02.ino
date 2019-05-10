#define LED 3
#define NIGHT 80

bool started = false;

void setup() {
  sei();
  adc_init();
  pinMode(LED,OUTPUT);
}

void loop() {
  
}

void adc_init(void)
{
  ADMUX |= 1<<REFS0 | 1<<MUX0 ;
  ADCSRA |= (1<<ADEN)  |(1<<ADSC)| (1<<ADATE) |(1<<ADIE);
}

ISR(ADC_vect) {
  if(ADC > NIGHT){
    digitalWrite(LED, HIGH);
  }else{
    digitalWrite(LED, LOW);
  }

}
