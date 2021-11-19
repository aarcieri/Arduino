#ifndef _TIMED_TASK_H_
#define _TIMED_TASK_H_

#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#define MAX_TASKS 10



typedef int (*Callback_t)(void *); // puntatore alla funzione di callback

typedef unsigned long Millisecs;

typedef struct st_task {
        Callback_t handler; // task handler callback func
        void* arguments;     // argument given to the callback handler
        Millisecs start;
        Millisecs expires;
} Task;


 short add_task(Millisecs expires, Callback_t handler, void* args );
 void remove_task(Callback_t handler);
 void tick();

#endif
