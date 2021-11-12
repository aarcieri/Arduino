int stato = 1;

void setup() {
  // put your setup code here, to run once:
  //Serial.begin(9600);
  pinMode(10, OUTPUT);
  // setta il pullup al pin 7
  pinMode(7, INPUT);
  digitalWrite(7, HIGH);
  
  digitalWrite(10,LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  int sensorValue = digitalRead(7);
  
  if(sensorValue==LOW){
    if(stato) stato=0;
    else stato=1;
    
    delay(500);
  }else
  {
  
    if(stato)
      digitalWrite(10,HIGH);
    else
      digitalWrite(10,LOW);
       
  }
  
}
