

#include "arduino-timedtasks.h"

#include <stdlib.h>
#include <limits.h>


Task getTask(int index)
{
    static Task tasks[MAX_TASKS];
    static char init=0;

    if(!init)
    {
        for(int i=0; i<MAX_TASKS; i++)
        {
            tasks[i].start = 0;
            tasks[i].handler=NULL;
        }
    }

    return tasks[index];
}

void deleteTask(int index)
{
    Task t = getTask(index);
    t.handler=NULL;
}

int getNextFreeTask()
{
    for(int i=0; i<MAX_TASKS; i++)
    {
        if(getTask(i).handler==NULL) return i;
    }

    return -1;
}

short add_task(Millisecs expires, Callback_t handler, void* args )
{
    int index = getNextFreeTask();

    if(index!=-1)
    {
        Task t = getTask(index);
        t.arguments = args;
        t.handler   = handler;
        t.start     = millis();
        t.expires   = expires;

        return 1;
    }

    return 0;
}

void remove_task(Callback_t handler)
{
    Task t;

    for(int i=0; i<MAX_TASKS; i++)
    {
        t = getTask(i);
        if(t.handler!=NULL && t.handler==handler)
        {
            t.handler = NULL;
            return;
        }
    }
}

void tick()
{
    Task task;
    unsigned long timeNow;
    unsigned long duration;

    for(int i=0; i<MAX_TASKS; i++)
    {
        task = getTask(i);

        if(task.handler!=NULL)
        {
            timeNow = millis();
            duration = timeNow - task.start;

            if(duration<0) // si verifica quando si va oltre il tempo massimo (circa 50 giorni)
            {
                duration = (ULONG_MAX-task.start) + timeNow;
            }

            if (duration >= task.expires)
            {
                short result = task.handler(task.arguments);

                if(!result) deleteTask(i);
                else task.start = timeNow;
            }
        }
    }
}

