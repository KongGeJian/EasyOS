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

#include "lib_stdint.h"
#include "bsp.h"
#include "os_timer.h"



/*
*********************************************************************************************************
*                                               DEFINE
*********************************************************************************************************
*/

#define OS_TIME_SLICE   10  //系统滴答数，表示10个os_tick

typedef  void (*TaskHook)(void); //任务函数钩子

//事件（可以多个事件按位或，表示多事件组合）
typedef enum
{
    EVENT_INTERVAL = 0x01,  //间隔
    EVENT_TIMEOUT = 0x02,   //超时
    EVENT_SIGNAL = 0x10,    //信号
} OS_EVENT_E_TYP;

//任务状态
typedef enum
{
    READY = 1,  //1-就绪：已具备执行能力，等待调度器调度
    RUNNING,    //2-运行：任务正在执行，此时它占用CPU
    BLOCKED,    //3-阻塞：当前任务正在等待某个时序或外部中断。包含任务被延时、任务等待信号量、任务等待读写队列或者读写事件等。
    SUSPENDED,  //4-挂起：处于挂起态的任务对调度器而言是不可见的。进入挂起状态的唯一办法就是调用 TaskSuspend() 函数；挂起状态的任务恢复的唯一途径是调用 TaskResume() 函数。
    DEAD,       //5-死亡：调用OS_DeleteTask，任务被标记为死亡态。
} OS_TASK_STATE_E_TYP;

//任务栈（3字节+4）
typedef struct OS_TASK_STACK
{
    uint8_t idata *base;//栈底
    uint8_t len;        //栈大小
    uint8_t idata *sp;  //当前栈顶
} OS_TASK_STACK_TYP;

//任务（21字节+4）
typedef struct OS_TASK
{
    TaskHook handle;                    //要运行的任务函数
    const char code *name;              //任务名称
    OS_TASK_STACK_TYP xdata *stack;     //绑定的栈
    volatile OS_TASK_STATE_E_TYP state; //程序状态
    volatile uint32_t time_statistics;  //时间统计，tick一次+1
    volatile uint8_t time_slice;        //时间片，从 OS_TIME_SLICE 倒计时
    volatile uint8_t blocked_time;      //阻塞时间，初始值 0xFF，tick一次-1
    volatile uint8_t blocked_semaphore; //阻塞信号量，初始值0，8位字节，用户自己使用
    volatile struct OS_TASK *prev;      //前一个任务
    volatile struct OS_TASK *next;      //后一个任务
} OS_TASK_TYP;

//任务链表（7字节+4）
typedef struct OS_TASK_LIST
{
    volatile OS_TASK_TYP *top;  //头
    volatile OS_TASK_TYP *end;  //尾
    volatile uint8_t size;      //任务数
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
extern void OS_InterEn(void) large;
extern void OS_InterDis(void) large;

extern void OS_TaskMark(void) large;
extern void OS_TaskSwitch(void) large;
extern void OS_TaskWait(OS_EVENT_E_TYP event, u8 nms, u8 semaphore) compact reentrant;
extern void OS_TaskSignal(OS_TASK_TYP xdata *task, u8 semaphore) large;

extern OS_TASK_TYP * OS_CreateTask(TaskHook task, const char code *task_name, u8 idata *stack_base, u8 stack_len) large;
extern void OS_DeleteTask(OS_TASK_TYP xdata *task) large;

extern void OS_IdleTask(void) large;
extern void OS_StackOverflow(TaskHook task, const char code *task_name) large;


#endif
/***********************************************END*****************************************************/
