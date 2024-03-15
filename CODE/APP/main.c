
#include <stdio.h>
#include <intrins.h>

#include "bsp.h"
#include "os.h"


u8 idata task1_stack[30];
u8 idata task2_stack[30];

void setup()
{
    // do something
    delay_ms(100);
}

void task1()
{
    while (1)
    {
        P10 = ~P10;
        OS_TaskWait(EVENT_TIMEOUT, 2, 0);
    }
}

void task2()
{
    while (1)
    {
        P11 = 1;
        delay_ms(2);
        P11 = 0;
        OS_TaskWait(EVENT_INTERVAL, 5, 0);
    }
}

void createTask()
{
    OS_CreateTask(task1, "task1", task1_stack, sizeof(task1_stack));
    OS_CreateTask(task2, "task2", task2_stack, sizeof(task2_stack));
}

void main()
{   
    BSP_Init();
    BSP_UART_Println(UART1, "==== BSP_Init completed! ====");

    setup();
    BSP_UART_Println(UART1, "==== setup complete! ====");

    OS_Init();
    BSP_UART_Println(UART1, "==== OS_Init completed! ====");
    createTask();
    BSP_UART_Println(UART1, "==== createTask completed! ====");
    OS_Start();
}