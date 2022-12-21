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
#include <string.h>

#include "bsp.h"

#include "os_timer.h"

#include "lib_stdint.h"


/*
*********************************************************************************************************
*                                               DEFINE
*********************************************************************************************************
*/

#define OS_TIME 10  //系统滴答数，表示10个os_tick

#define OS_TASK_NUM            2    //最大可运行任务数

typedef  void (*TaskHook)(void); //任务函数钩子

//任务状态  TODO https://zhuanlan.zhihu.com/p/503838768
typedef enum
{
    NONE,
    READY = 1,  //1-就绪：已具备执行能力，等待调度器调度
    RUNNING,    //2-运行：任务正在执行，此时它占用CPU
    BLOCKED,    //3-阻塞：当前任务正在等待某个时序或外部中断，该任务不在就绪列表中。包含任务被挂起、任务被延时、任务正在等待信号量、读写队列或者等待读写事件等。
    SUSPENDED,  //4-挂起：处于挂起态的任务对调度器而言是不可见的，任务进入挂起状态的唯一办法就是调用 vTaskSuspend()函数；挂起状态的任务恢复的唯一途径是调用 vTaskResume() 或vTaskResumeFromISR()函数
    DEAD,       //5-死亡：延迟销毁
} OS_TASK_STATE_E_TYP;

//任务栈（3字节+4）
typedef struct OS_TASK_STACK
{
    uint8_t idata *base;   //栈底
    uint8_t len;    //栈大小
    uint8_t idata *sp;     //当前栈顶
} OS_TASK_STACK_TYP;

//任务（18字节+4）
typedef struct OS_TASK
{
    TaskHook handle;            //要运行的任务函数
    const char code *name;      //任务名称
    volatile OS_TASK_STATE_E_TYP state; //程序状态
    uint32_t timer;             //计时器，tick一次+1
    OS_TASK_STACK_TYP xdata *stack;     //绑定的栈
    struct OS_TASK *prev;       //前一个任务
    struct OS_TASK *next;       //后一个任务
} OS_TASK_TYP;

//任务链表（7字节+4）
typedef struct OS_TASK_LIST
{
    OS_TASK_TYP *top;       //头
    OS_TASK_TYP *end;       //尾
    volatile uint8_t size;  //任务数
} OS_TASK_LIST_TYP;


//执行中任务
extern OS_TASK_TYP xdata * xdata running_task;


/*
*********************************************************************************************************
*                                             FUNCTION
*********************************************************************************************************
*/

extern void OS_Init(void) large;

extern void OS_Start(void) large;

extern OS_TASK_TYP * OS_CreateTask(TaskHook task, const char code *task_name, u8 idata *stack_base, u8 stack_len) large;
extern void OS_DeleteTask(OS_TASK_TYP xdata *task) large;

extern void OS_Task_SW(void) large;
extern void OS_IdleTask(void) large;

extern void OS_StackOverflow(TaskHook task, const char code *task_name) large;


#endif
/***********************************************END*****************************************************/
