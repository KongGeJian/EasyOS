/*
*********************************************************************************************************
*                                      OPERATING SYSTEM PACKAGE
*
* Name    : 系统
* Version : V1.0
* Author  : 空格键
*********************************************************************************************************
*/

#ifndef __OS_H__
#define __OS_H__

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include <stdlib.h>

#include "cfg.h"

#include "os_timer.h"

#include "lib_stdint.h"


/*
*********************************************************************************************************
*                                               DEFINE
*********************************************************************************************************
*/

#define OS_TASKS_LEN    6  //最大可运行任务数

typedef  void (*TaskHook)(void); //任务函数钩子


/*
*********************************************************************************************************
*                                             FUNCTION
*********************************************************************************************************
*/

extern void OS_Init(void);

extern void OS_Start(void);

extern void OS_CreateTask(u16 timer, u16 reload, TaskHook task);
extern void OS_DeleteTask(TaskHook task);

extern void OS_TaskMark(void);
extern void OS_TaskProcess(void);


#endif
/***********************************************END*****************************************************/
