#ifndef _TIMED_TASK_H_
#define _TIMED_TASK_H_

#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include <stdlib.h>

#define MAX_TASKS 10

typedef int (*Callback_t)(void *); // puntatore alla funzione di callback

typedef unsigned long Millisecs;

typedef struct st_task {
        Callback_t callback; /* task handler callback func */
        void* arguments; /* argument given to the callback handler */
        Millisecs start;
} Task;

Task getFreeTask(int index)
{
    static Task tasks[MAX_TASKS];
    static char init=0;

    if(!initi)
    {
        for(int i=0; i<MAX_TASKS; i++) tasks[i].handler=NULL;

        return task[0];
    }

    for(int i=0; i<MAX_TASKS; i++)
    {
        if(tasks[i].handler==NULL) return tasks[i];
    }

    return NULL;
}

Task add_task(Millisecs start_time, Callback_t handler, void* args )
{
    Task t = getFreeTask();

    if(t!=NULL)
    {
        t.arguments = args;
        t.callback = handler;
        t.start=start_time;
    }

    return t;
}

void remove_task(Callback_t handler)
{
    for(int i=0; i<MAX_TASKS; i++)
    {
        if(tasks[i].handler!=NULL && tasks[i].handler==handler)
        {
            tasks[i].handler = NULL;
            return;
        }
    }
}


template <
    size_t max_tasks = TIMER_MAX_TASKS, /* max allocated tasks */
    unsigned long (*time_func)() = millis, /* time function for timer */
    typename T = void * /* handler argument type */
>
class Timer {
  public:

    typedef uintptr_t Task; /* public task handle */
    typedef bool (*handler_t)(T opaque); /* task handler func signature */

    /* Calls handler with opaque as argument in delay units of time */
    Task
    in(unsigned long delay, handler_t h, T opaque = T())
    {
        return task_id(add_task(time_func(), delay, h, opaque));
    }

    /* Calls handler with opaque as argument at time */
    Task
    at(unsigned long time, handler_t h, T opaque = T())
    {
        const unsigned long now = time_func();
        return task_id(add_task(now, time - now, h, opaque));
    }

    /* Calls handler with opaque as argument every interval units of time */
    Task
    every(unsigned long interval, handler_t h, T opaque = T())
    {
        return task_id(add_task(time_func(), interval, h, opaque, interval));
    }

    /* Cancel the timer task */
    void
    cancel(Task &task)
    {
        if (!task) return;

        timer_foreach_task(t) {
            if (t->handler && (t->id ^ task) == (uintptr_t)t) {
                remove(t);
                break;
            }
        }

        task = (Task)NULL;
    }

    /* Cancel all timer tasks */
    void
    cancel()
    {
        timer_foreach_task(t) {
            remove(t);
        }
    }

    /* Ticks the timer forward - call this function in loop() */
    unsigned long
    tick()
    {
        tick<void>();
        return ticks();
    }

    template <typename R> void
    tick()
    {
        timer_foreach_task(task) {
            if (task->handler) {
                const unsigned long t = time_func();
                const unsigned long duration = t - task->start;

                if (duration >= task->expires) {
                    task->repeat = task->handler(task->opaque) && task->repeat;

                    if (task->repeat) task->start = t;
                    else remove(task);
                }
            }
        }
    }

    /* Ticks until the next event */
    unsigned long
    ticks() const
    {
        unsigned long ticks = (unsigned long)-1, elapsed;
        const unsigned long start = time_func();

        timer_foreach_const_task(task) {
            if (task->handler) {
                const unsigned long t = time_func();
                const unsigned long duration = t - task->start;

                if (duration >= task->expires) {
                    ticks = 0;
                    break;
                } else {
                    const unsigned long remaining = task->expires - duration;
                    ticks = remaining < ticks ? remaining : ticks;
                }
            }
        }

        elapsed = time_func() - start;

        if (elapsed >= ticks || ticks == (unsigned long)-1) ticks = 0;
        else ticks -= elapsed;

        return ticks;
    }

    /* Number of active tasks in the timer */
    size_t
    size() const
    {
        size_t s = 0;

        timer_foreach_const_task(task) {
            if (task->handler) ++s;
        }

        return s;
    }

    /* True if there are no active tasks */
    bool
    empty() const
    {
        timer_foreach_const_task(task) {
            if (task->handler) return false;
        }

        return true;
    }

    Timer() : ctr(0), tasks{} {}

  private:

    size_t ctr;

    struct task {
        handler_t handler; /* task handler callback func */
        T opaque; /* argument given to the callback handler */
        unsigned long start,
                      expires; /* when the task expires */
        size_t repeat, /* repeat task */
               id;
    } tasks[max_tasks];

    inline
    void
    remove(struct task *task)
    {
        task->handler = NULL;
        task->opaque = T();
        task->start = 0;
        task->expires = 0;
        task->repeat = 0;
        task->id = 0;
    }

    inline
    Task
    task_id(const struct task * const t)
    {
        const Task id = (Task)t;

        return id ? id ^ t->id : id;
    }

    inline
    struct task *
    next_task_slot()
    {
        timer_foreach_task(slot) {
            if (slot->handler == NULL) return slot;
        }

        return NULL;
    }

    inline
    struct task *
    add_task(unsigned long start, unsigned long expires,
             handler_t h, T opaque, bool repeat = 0)
    {
        struct task * const slot = next_task_slot();

        if (!slot) return NULL;

        if (++ctr == 0) ++ctr; // overflow

        slot->id = ctr;
        slot->handler = h;
        slot->opaque = opaque;
        slot->start = start;
        slot->expires = expires;
        slot->repeat = repeat;

        return slot;
    }
};


#endif
