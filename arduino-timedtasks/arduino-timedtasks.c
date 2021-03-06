

#include "arduino-timedtasks.h"

#include <stdlib.h>
#include <limits.h>

#ifdef _JUST_DEBUG_

    #include <sys/time.h>

    #define MAX_VALUE_TIMER 20

    //return millisec elapsed from first call
    unsigned int millis_now()
    {
        static struct timeval start;
        static char init=0;
        struct timeval now;

        if(!init){ gettimeofday(&start, NULL); init=1; }

        gettimeofday(&now, NULL);

        unsigned int millisecs = (unsigned int) ((now.tv_sec-start.tv_sec) * 1000 + (now.tv_usec-start.tv_usec)/1000);


        // simulate the case of time exceded the maximum
        if((now.tv_sec-start.tv_sec)>=MAX_VALUE_TIMER)
        {
            gettimeofday(&start, NULL);
        }


        return millisecs;
    }

#else
    #define millis_now() millis()
#endif // _JUST_DEBUG_



Task* getTask(int index)
{
    static Task tasks[MAX_TASKS];
    static char init=0;

    if(!init)
    {
        init=1;
        for(int i=0; i<MAX_TASKS; i++)
        {
            tasks[i].handler=NULL;
        }
    }

    return &(tasks[index]);
}

void deleteTask(int index)
{
    Task* t = getTask(index);

    t->handler=NULL;
}

int getNextFreeTask()
{
    for(int i=0; i<MAX_TASKS; i++)
    {
        if(getTask(i)->handler==NULL) return i;
    }

    return -1;
}

short add_task(Millisecs expires, Callback_t handler, void* args )
{
    int index = getNextFreeTask();

    if(index!=-1)
    {
        Task* t = getTask(index);
        t->arguments = args;
        t->handler   = handler;
        t->expires   = expires;
        t->elapsed   = 0;

        return 1;
    }

    return 0;
}

void remove_task(Callback_t handler)
{
    Task* t;

    for(int i=0; i<MAX_TASKS; i++)
    {
        t = getTask(i);
        if(t->handler!=NULL && t->handler==handler)
        {
            t->handler = NULL;
            return;
        }
    }
}


unsigned long delta(unsigned int timeNow, unsigned long lastStart)
{
    unsigned long delta;

    if(timeNow < lastStart)
    {
        delta = ((unsigned long)UINT_MAX-lastStart) + timeNow;
        #ifdef _JUST_DEBUG_
        delta =((unsigned long)MAX_VALUE_TIMER*1000-lastStart) + timeNow;
        #endif // _JUST_DEBUG_
    }else
    {
        delta = timeNow - lastStart;
    }

    return delta;
}

void tick()
{
    static unsigned long lastStart=0;
    Task* task;
    unsigned int timeNow;
    unsigned long d; // tempo trascorso tra 2 chiamate successive a tick

    timeNow = millis_now();

    if(lastStart==0) lastStart=timeNow;

    d = delta(timeNow, lastStart);

    for(int i=0; i<MAX_TASKS; i++)
    {

        task = getTask(i);
        task->elapsed += d;

        if(task->handler!=NULL && task->elapsed >= task->expires)
        {
            short result = task->handler(task->arguments);

            if(!result) deleteTask(i);
            else
            {
                task->elapsed = 0;
            }
        }
    }

    lastStart=timeNow;
}




