
/* Diversi modi dell'interrupt 
  LOW to trigger the interrupt whenever the pin is low,

  CHANGE to trigger the interrupt whenever the pin changes value
  
  RISING to trigger when the pin goes from low to high,
  
  FALLING for when the pin goes from high to low.
  */

int stato = 0;

void setup() {
  pinMode(10, OUTPUT);
  digitalWrite(10,LOW);
  
  // setta il pullup al pin 7
  pinMode(2, INPUT);
  digitalWrite(2, HIGH);

  // attiva interrupt quando c'e' un passaggio da 1 a 0
  attachInterrupt(digitalPinToInterrupt(2), variaStato, FALLING );
}

void variaStato()
{
   if(stato) stato=0;
   else stato=1;
}

void loop() {
  // put your main code here, to run repeatedly:
  
   // int sensorValue = digitalRead(7);
    
    if(stato)
    {
      digitalWrite(10,HIGH);
      delay(500);
      digitalWrite(10,LOW);  
      delay(500);
    }
}
