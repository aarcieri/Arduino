#include <stdio.h>
#include <stdlib.h>
#include "arduino-timedtasks.h"

int z;

int task_generic(void* args)
{
    char* taskName = (char*) args;
    //printf("\n %s z=%d", taskName,z);
    z++;
    return 1;
}


int task1(void* args)
{
    //printf("\n Task1");
    return 0;
}

int task2(void* args)
{
    //printf("\n Task2");
    add_task(1000, task1, NULL);
    return 1;
}



int main()
{
    z=0;
    add_task(1000, task1, NULL);
    add_task(3000, task2, NULL);
    add_task(2000, task_generic, "Gino");
    add_task(80, task_generic,"Pluto");

    printf("\n START ");
    while(1) tick();

    return 0;
}
