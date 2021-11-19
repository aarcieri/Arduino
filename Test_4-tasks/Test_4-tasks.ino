#include <arduino-timedtasks.h>

/*  funzioni di "arduino-timedtasks"
 short add_task(Millisecs expires, Callback_t handler, void* args );
 void remove_task(Callback_t handler);
 void tick();
 */
 
int stato = 0;
int acceso=0;

void setup() {
  pinMode(10, OUTPUT);
  digitalWrite(10,LOW);

  pinMode(LED_BUILTIN, OUTPUT);
  
  // setta il pullup al pin 
  pinMode(2, INPUT);
  digitalWrite(2, HIGH);

  // attiva interrupt quando c'e' un passaggio da 1 a 0
  //attachInterrupt(digitalPinToInterrupt(2), variaStato, FALLING );
  add_task(10, variaStato, NULL);
  //add_task(1000, LampeggioComandato, &acceso);
  add_task(1000, Lampeggio, &acceso);
}

int variaStato(void* args)
{
   int sensorValue = digitalRead(2);
  
  if(sensorValue==LOW)
  {
    if(stato) stato=0;
    else stato=1;
  }
   
   return 1;
}

int LampeggioComandato(void* args) 
{
  int* value = ((int*)(args));
  
  if(stato)
  {
    if(*value)
    {
      *value=0;
      digitalWrite(10,HIGH);
    }
    else
    { 
      *value=1;
      digitalWrite(10,LOW);  
    }
  }

  return 1;
}

int Lampeggio(void* args) 
{
  int* value = ((int*)(args));
  
  if(stato)
  {
    if(*value)
    {
      *value=0;
      digitalWrite(LED_BUILTIN, HIGH); 
    }
    else
    { 
      *value=1;
      digitalWrite(LED_BUILTIN, LOW); 
    }
  }

  return 1;
}

void loop()
{
  tick(); 
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on 
  delay(500);                        // wait for half a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off 
  delay(500);     
}
