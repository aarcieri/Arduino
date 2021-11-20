#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#include "arduino-timedtasks.h"

int z;

int task_generic(void* args)
{
    char* taskName = (char*) args;
    printf("\n %s z=%d", taskName,z);
    z++;
    return 1;
}


int task0(void* args)
{
    printf("\n Task_0");
    //sleep(4);
    return 1;
}

int task1(void* args)
{
   printf("\n \t\tTask_1");
    //add_task(1000, task0, NULL);
    return 1;
}


int main()
{
    z=0;
    add_task(2000, task0, NULL);
    add_task(1000, task1, NULL);
    add_task(10000, task_generic,"\n #1");
    add_task(3000, task_generic, "\n \t#2");
    /*add_task(8000, task_generic, "\n \t\t#3");
    add_task(50, task_generic, "\n \t\t\t#4");*/


    printf("\n START ");

    while(1)
    {
        tick();
    }

    return 0;
}
