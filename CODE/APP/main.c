
#include <stdio.h>
#include <intrins.h>

#include "cfg.h"


#include "lib_delay.h"
#include "lib_stdint.h"


void task1()
{
    while (1)
    {
        P10 = ~P10;
        delay_ms(2);
    }
}

void task2()
{
    while (1)
    {
        P11 = ~P11;
        delay_ms(5);
    }
}

void createTask()
{
    OS_CreateTask(0, 20, task1);
    OS_CreateTask(10, 10, task2);
}

void main()
{   
    OS_Init();
    delay_init();
    
    createTask();
    
    OS_Start();
    while (1)
    {
        OS_TaskProcess();
    }
}
