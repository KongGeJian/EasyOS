
#include <stdio.h>
#include <intrins.h>

#include "bsp.h"


u8 idata task1_stack[20];
u8 idata task2_stack[20];
const char code task1_name[] = "task1";
const char code task2_name[] = "task2";

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
    OS_CreateTask(task1, task1_name, task1_stack, sizeof(task1_stack));
    OS_CreateTask(task2, task2_name, task2_stack, sizeof(task2_stack));
}

void main()
{   
    OS_Init();
    delay_init();
    
    createTask();
    
    OS_Start();
}
