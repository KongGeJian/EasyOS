
#include <stdio.h>
#include <intrins.h>

#include "bsp.h"
#include "os.h"


u8 idata task1_stack[20];
u8 idata task2_stack[20];
const char code task1_name[] = "task1";
const char code task2_name[] = "task2";

void task1()
{
    while (1)
    {
        P10 = ~P10;
        OS_TaskWait(EVENT_TIMEOUT, 2);
    }
}

void task2()
{
    while (1)
    {
        P11 = 1;
        delay_ms(2);
        P11 = 0;
        OS_TaskWait(EVENT_INTERVAL, 5);
    }
}

void createTask()
{
    OS_CreateTask(task1, task1_name, task1_stack, sizeof(task1_stack));
    OS_CreateTask(task2, task2_name, task2_stack, sizeof(task2_stack));
}

void main()
{   
    BSP_Init();
    BSP_UART_SendString(UART1, "STRBUF_LEN too small.\n");
    
    OS_Init();
    createTask();
    OS_Start();
}
