#ifndef _TIMED_TASK_H_
#define _TIMED_TASK_H_

// uncomment to debug using CodeBlocks
//#define _JUST_DEBUG_   // flag to debug outside Arduino


#ifndef _JUST_DEBUG_

#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#endif // _JUST_DEBUG_
#define MAX_TASKS 5



typedef int (*Callback_t)(void *); // puntatore alla funzione di callback

typedef unsigned int Millisecs;

typedef struct st_task {
        Callback_t handler; // task handler callback func
        void* arguments;     // argument given to the callback handler
        unsigned int  expires;
        unsigned long elapsed;
} Task;


 short add_task(Millisecs expires, Callback_t handler, void* args );
 void remove_task(Callback_t handler);
 void tick();

#endif
