  
  
  //#include <stdio.h>
  //#include <stdlib.h>
  //#include <unistd.h>
  
  #include <arduino-timedtasks.h>
  
  int ledInternal;
  int led10;
  
  int task_generic(void* args)
  {
      char* taskName = (char*) args;
      //printf("\n %s z=%d", taskName,z);
      //z++;
      return 1;
  }
  
  
  int blinkInternalLed(void* args)
  {
    if(ledInternal==LOW) ledInternal=HIGH;
    else ledInternal=LOW;

    digitalWrite(LED_BUILTIN, ledInternal); 
    
    return 1;
  }
  
  int blinkPin10(void* args)
  {
    if(led10==LOW) led10=HIGH;
    else led10=LOW;

    digitalWrite(10, led10); 
    
    return 1;
  }
  
  void setup() {
    pinMode(10, OUTPUT);
    pinMode(LED_BUILTIN, OUTPUT);
    
    ledInternal = 0;
    led10=0;

    add_task(2000, blinkInternalLed, NULL);
    add_task(1000, blinkPin10, NULL);

    /*
    char* tasN1 = "#1";
    char* tasN2 = "#2";
    add_task(10000, task_generic,&tasN1);
    add_task(3000, task_generic, &tasN2);
    */
  }
  
  void loop(){
      tick();
  }


  
