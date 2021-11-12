int stato = 1;

void setup() {
  // put your setup code here, to run once:
  //Serial.begin(9600);
  pinMode(10, OUTPUT);
  pinMode(7, INPUT_PULLUP);
  //digitalWrite(7, HIGH);
  digitalWrite(10,LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  int sensorValue = digitalRead(7);
  
  if(sensorValue==HIGH){
    /*if(stato) stato=0;
    else stato=1;*/
    digitalWrite(10,HIGH);
  }
  else
  {
    digitalWrite(10,LOW);
  }
  
 /* if(stato){
    digitalWrite(10,HIGH);
    delay(500);
    digitalWrite(10,LOW);
    delay(500);
  }*/
  
  delay(1000);
}
