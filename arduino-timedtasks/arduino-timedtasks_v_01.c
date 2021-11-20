

#include "arduino-timedtasks.h"

#include <stdlib.h>
#include <limits.h>

#ifdef _JUST_DEBUG_

    #include <sys/time.h>

    #define MAX_VALUE_TIMER 4

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
            tasks[i].start = 0;
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
        t->start     = millis_now();
        t->expires   = expires;

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


void tick()
{
    Task* task;
    unsigned int timeNow;
    unsigned long duration;

    for(int i=0; i<MAX_TASKS; i++)
    {
        task = getTask(i);

        if(task->handler!=NULL)
        {
            timeNow = millis_now();

            duration = timeNow - task->start;

            if(timeNow < task->start) // si verifica quando si va oltre il tempo massimo (circa 50 giorni su Arduino)
            {
                duration = ((unsigned long)UINT_MAX-task->start) + timeNow;
                #ifdef _JUST_DEBUG_
                duration =((unsigned long)MAX_VALUE_TIMER*1000-task->start) + timeNow;
                //printf("\n Task=%ld -- timeNow=%ld-- start=%ld duration=%ld expires=%ld", i, timeNow, task->start, duration, task->expires);
                #endif // _JUST_DEBUG_
            }

            if (duration >= task->expires)
            {
                short result = task->handler(task->arguments);

                if(!result) deleteTask(i);
                else task->start = timeNow;
            }
        }
    }
}




