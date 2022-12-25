/*
*********************************************************************************************************
*                                                 MODULE
*
* Note(s) : 精确延时用 OS_TaskWait()。这里使用程序不精确延时
* Tip(s)  : 说明：
*             + 延迟函数库（软件延时，不同的芯片参数不同）。 系统频率 12.000M Hz
*             + while 循环 % 等会增加误差，实际软件延迟时间比预期值大很多
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/
#include "lib_delay.h"



/*
*********************************************************************************************************
*                                         PRIVATE FUNCTION
*********************************************************************************************************
*/

/*软件生成延时*/
void Delay1ms() //@12.000MHz
{
    unsigned char i, j;

    i = 16;
    j = 147;
    do
    {
        while (--j);
    } while (--i);
}

/*
*********************************************************************************************************
* Description: 延时初始化函数
*
* Arguments  : none.
*
* Note(s)    : none.
*********************************************************************************************************
*/
void delay_init(void)
{
    delay_us(0);
    delay_ms(0);
}

/*
*********************************************************************************************************
* Description: 延时n微妙
*
* Arguments  : nus  -  n微妙
*
* Note(s)    : 延迟时间，最多跨一个系统计时器溢出。小于50us(实测是53us)没有精度保证
*********************************************************************************************************
*/
void delay_us(u16 nus) compact reentrant
{
    // u32 ms;
    // u16 us;
    // if (nus >= 1000)
    // {
    //     delay_ms(nus / 1000);
    //     nus = nus % 1000;
    // }
    // ms = os_tick;
    // us = OS_TIMER_GetUs();
    // if (999 - us >= nus)
    // {
    //     us += nus;
    // }
    // else
    // {
    //     us = nus - (999 - us) - 1;
    //     ms++;
    // }
    // while (os_tick < ms || (os_tick == ms && OS_TIMER_GetUs() < us));
    while(nus--);
}

/*
*********************************************************************************************************
* Description: 延时n毫秒
*
* Arguments  : nms  -  n毫秒
*
* Note(s)    : 不考虑溢出边界
*********************************************************************************************************
*/
void delay_ms(u16 nms) compact reentrant
{
    while(nms--)
        Delay1ms();
}

/**********************************************RND******************************************************/
