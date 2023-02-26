/*
*********************************************************************************************************
*                                        BOARD SUPPORT PACKAGE
*
* Name    : UART
* Version : V1.0
* Author  : 空格键
*********************************************************************************************************
*/

#ifndef __BSP_UART_H__
#define __BSP_UART_H__

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include <stdarg.h>
#include <stdio.h>

#include "bsp.h"


/*
*********************************************************************************************************
*                                               DEFINE
*********************************************************************************************************
*/

//串口枚举
typedef enum UART_E
{
    UART1,  //串口1
    UART2,  //串口2
} UART_E_TYP;

extern volatile byte pdata u1_r_data;
extern volatile byte pdata u1_r_parity;
extern volatile byte pdata u2_r_data;
extern volatile byte pdata u2_r_parity;

#define STRBUF_LEN 160 //字符串buf长度



/*
*********************************************************************************************************
*                                             FUNCTION
*********************************************************************************************************
*/

extern void  BSP_UART_Init(void) large;

extern void  BSP_UART_SendByte(UART_E_TYP uart, byte dat) compact reentrant;
extern void  BSP_UART_SendString(UART_E_TYP uart, const char *s) compact reentrant;
extern void  BSP_UART_Println(UART_E_TYP uart, const char *format, ...) compact reentrant;


#endif
/***********************************************END*****************************************************/
