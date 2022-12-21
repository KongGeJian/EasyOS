/*
*********************************************************************************************************
*                                                 MODULE
*
* Note(s) : 延迟函数库
*********************************************************************************************************
*/

#ifndef __LIB_DELAY_H__
#define __LIB_DELAY_H__

/*
*********************************************************************************************************
*                                              INCLUDE FILES
*********************************************************************************************************
*/
#include <intrins.h>

#include "lib_stdint.h"

#include "os_timer.h"



/*
*********************************************************************************************************
*                                             FUNCTION
*********************************************************************************************************
*/

void    delay_init(void);
void    delay_us(u16 nus) compact reentrant;
void    delay_ms(u16 nms) compact reentrant;

#endif