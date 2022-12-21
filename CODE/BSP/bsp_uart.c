/*
*********************************************************************************************************
*                                        BOARD SUPPORT PACKAGE
*
* Name    : UART
* Version : V1.0
* Author  : 空格键
* ----------------------------
* Note(s) : 波特率发生器T2自动重载值。T2只有16位自动重装载模式，且默认12T模式。
*             串口定义
*             ----------------------------------------------------------------
*             串口      引脚                    切换
*             Uart1     RxD/P30 TxD/P31         p_SW1.S1_S[1:0] [初始值]
*             Uart2     RxD2/P10 TxD2/P11       -
*             ----------------------------------------------------------------
*             串口有四种工作模式：
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include "bsp_uart.h"



/*
*********************************************************************************************************
*                                               DEFINE
*********************************************************************************************************
*/

#define BAUD 115200         //UART波特率
#define BRT (65536 - SYSclk/4/BAUD) //波特率发生器T2自动重载值：1T模式公式 reload=65536 - SYSclk / (4 * Baud)

/*定义UART奇偶校验模式*/
#define NONE_PARITY     0   //None parity
#define ODD_PARITY      1   //Odd parity
#define EVEN_PARITY     2   //Even parity
#define MARK_PARITY     3   //Mark parity
#define SPACE_PARITY    4   //Space parity

#define PARITYBIT NONE_PARITY   //Testing even parity

/*串口2控制寄存器S2CON关键位*/
#define S2RI  0x01          //S2CON.0
#define S2TI  0x02          //S2CON.1
#define S2RB8 0x04          //S2CON.2
#define S2TB8 0x08          //S2CON.3

bit busy;


//串口1读取的缓存数据，外部使用声明即可: extern byte xdata u1_r_data;
volatile byte pdata u1_r_data;
//串口1读取的奇偶校验位，外部使用声明即可: extern bit  xdata u1_r_parity;
volatile byte pdata u1_r_parity;
//串口2读取的缓存数据，外部使用声明即可: extern byte xdata u2_r_data;
volatile byte pdata u2_r_data;
//串口1读取的奇偶校验位，外部使用声明即可: extern bit  xdata u2_r_parity;
volatile byte pdata u2_r_parity;


//字符串buf
static const char xdata strbuf[STRBUF_LEN];


/*
*********************************************************************************************************
*                                         PRIVATE FUNCTION
*********************************************************************************************************
*/

/*串口1发送字节*/
static void UART1_SendByte(byte dat) compact reentrant
{
#if (DEBUG_USE_SIMULATOR == 0)
    while (busy);           //等待上一个数据发送完成
#endif
    ACC = dat;              //计算偶数奇偶校验位P（PSW.0）
    if (P)                  //根据P设置奇偶校验位
    {
#if (PARITYBIT == ODD_PARITY)
        TB8 = 0;            //将奇偶校验位设置为0
#elif (PARITYBIT == EVEN_PARITY)
        TB8 = 1;            //将奇偶校验位设置为1
#endif
    }
    else
    {
#if (PARITYBIT == ODD_PARITY)
        TB8 = 1;            //将奇偶校验位设置为1
#elif (PARITYBIT == EVEN_PARITY)
        TB8 = 0;            //将奇偶校验位设置为0
#endif
    }
    busy = 1;               //标志位置位，由中断清除
    SBUF = ACC;             //将数据发送到UART缓冲区
}

/*串口2发送字节*/
static void UART2_SendByte(byte dat) compact reentrant
{
#if (DEBUG_USE_SIMULATOR == 0)
    while (busy);           //等待上一个数据发送完成
#endif
    ACC = dat;              //计算偶数奇偶校验位P（PSW.0）
    if (P)                  //根据P设置奇偶校验位
    {
#if (PARITYBIT == ODD_PARITY)
        S2CON &= ~S2TB8;    //将奇偶校验位设置为0
#elif (PARITYBIT == EVEN_PARITY)
        S2CON |= S2TB8;     //将奇偶校验位设置为1
#endif
    }
    else
    {
#if (PARITYBIT == ODD_PARITY)
        S2CON |= S2TB8;     //将奇偶校验位设置为1
#elif (PARITYBIT == EVEN_PARITY)
        S2CON &= ~S2TB8;    //将奇偶校验位设置为0
#endif
    }
    busy = 1;               //标志位置位，由中断清除
    S2BUF = ACC;            //将数据发送到UART2缓冲区
}


/*
*********************************************************************************************************
*                                             INTERRUPT
*********************************************************************************************************
*/

/*UART1中断服务程序*/
void UART1_ISR() interrupt 4 using 3
{
    if (RI) //接收
    {
        RI = 0;             //清除接收中断标志
        u1_r_data = SBUF;
        u1_r_parity = RB8;
    }
    if (TI) //发送
    {
        TI = 0;             //Clear transmit interrupt flag
        busy = 0;           //Clear transmit busy flag
    }
}

/*UART2中断服务程序*/
void UART2_ISR() interrupt 8 using 3
{
    if (S2CON & S2RI) //接收
    {
        S2CON &= ~S2RI;                //清除接收中断标志
        u2_r_data = S2BUF;             //缓存数据
        u2_r_parity = (S2CON & S2RB8); //显示奇偶校验位
    }
    if (S2CON & S2TI) //发送
    {
        S2CON &= ~S2TI; //清除发送中断标志
        busy = 0;       //清除发送忙标志
    }
}

/*
*********************************************************************************************************
* Description : UART初始化。
*
* Caller(s)   : BSP_Init().
*
* Note(s)     : none.
*********************************************************************************************************
*/
void BSP_UART_Init(void) large
{
    //串口1、2控制寄存器
#if (PARITYBIT == NONE_PARITY)
    SCON = 0x50;            //串口1，方式1：8-bit 串口，波特率可变
    S2CON = 0x10;           //串口2，方式0：8-bit 串口，波特率可变
#elif (PARITYBIT == ODD_PARITY) || (PARITYBIT == EVEN_PARITY) || (PARITYBIT == MARK_PARITY)
    SCON = 0xda;            //串口1，方式3：9-bit 串口，波特率可变，奇偶校验位初始值为1
    S2CON = 0x9a;           //串口2，方式1：9-bit 串口，波特率可变，奇偶校验位初始值为1
#elif (PARITYBIT == SPACE_PARITY)
    SCON = 0xd2;            //串口1，方式3：9-bit 串口，波特率可变，奇偶校验位初始值为0
    S2CON = 0x92;           //串口2，方式3：9-bit 串口，波特率可变，奇偶校验位初始值为0
#endif

    T2L = BRT;
    T2H = BRT >> 8;

    AUXR |= 0x15;           //串口1波特率不加倍
                            //定时器T2 1T模式
                            //串口1使用T2作为波特率发生器
                            //T2开始计数
                            //串口2只能使用T2作为波特率发生器，这里无需设置

    ES = 1;                 //使能UART1中断
    IE2 |= 0x01;            //使能UART2中断
    EA = 1;                 //断开主中断开关，其他地方已经打开

    busy = 0;
}

/*
*********************************************************************************************************
* Description : UART发送字节数据
*
* Argument(s) : UART - 串口枚举 UART1/UART2
*               dat - 要发送的数据
*
* Return(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************
*/
void BSP_UART_SendByte(UART_E_TYP UART, byte dat) large reentrant
{
    if (UART == UART1)
        UART1_SendByte(dat);
    else
        UART2_SendByte(dat);
}

/*
*********************************************************************************************************
* Description : 将字符串发送到UART
*
* Argument(s) : UART - 串口枚举 UART1/UART2
*               *s - 字符串地址
*
* Return(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************
*/
void BSP_UART_SendString(UART_E_TYP UART, char *s) large reentrant
{
    if (UART == UART1)
        while (*s)                //检查字符串的结尾
            UART1_SendByte(*s++); //发送当前字符，后自增字符串地址指针
    else
        while (*s)                //检查字符串的结尾
            UART2_SendByte(*s++); //发送当前字符，后自增字符串地址指针
}

/*
*********************************************************************************************************
* Description : 串口打印行
*
* Argument(s) : UART - 串口枚举 UART1/UART2
*               *s - 字符串地址
*
* Return(s)   : none.
*********************************************************************************************************
*/
void BSP_UART_Println(UART_E_TYP UART, const char *format, ...) large reentrant
{
    va_list aptr;

    va_start(aptr, format);
    vsprintf(strbuf, format, aptr);
    va_end(aptr);

    BSP_UART_SendString(UART, strbuf);
    BSP_UART_SendByte(UART, '\n');
}

/**********************************************RND******************************************************/
