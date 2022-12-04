/*
*********************************************************************************************************
*                                      OPERATING SYSTEM PACKAGE
*
* Name    : 系统计时器
* Version : V1.0
* Author  : 空格键
* ----------------------------
* Note(s) : 时间单位，有os_timer.c决定。这里是ms
*********************************************************************************************************
*/

#include "os.h"



/*
*********************************************************************************************************
*                                               DEFINE
*********************************************************************************************************
*/

//系统任务
typedef struct OS_TASK
{
    volatile boolean run;   //程序运行标记：false-不运行，true-运行
    uint16_t timer;         //计时器计数值，看定时器调度时间
    uint16_t reload;        //为0时不能重复调用
    TaskHook task_handle;   //要运行的任务函数
} OS_TASK_TYP;

//任务池容器
OS_TASK_TYP xdata os_task_pool[OS_TASKS_LEN];



/*
*********************************************************************************************************
*                                         PRIVATE FUNCTION
*********************************************************************************************************
*/

//初始化话任务
void Init_Taks(u8 id)
{
    if (id >= OS_TASKS_LEN)
        return;
    os_task_pool[id].run = false;        //默认停止
    os_task_pool[id].timer = 0;          //计数值
    os_task_pool[id].reload = 0;         //重装值
    os_task_pool[id].task_handle = NULL; //函数名，指针类型
}

//初始化任务结构体
void Init_TaskList(void)
{
    u8 idata id;
    for (id = 0; id < OS_TASKS_LEN; id++)
        Init_Taks(id);
}

/*
*********************************************************************************************************
* Description : 系统初始化
*
* Caller(s)   : -
*
* Note(s)     : none.
*********************************************************************************************************
*/
void OS_Init(void)
{
    Init_TaskList();
    OS_TIMER_Init();
}

/*
*********************************************************************************************************
* Description : 系统启动
*
* Note(s)     : none.
*********************************************************************************************************
*/
void OS_Start(void)
{
    os_start_enable = 1;
}

/*
*********************************************************************************************************
* Description : 创建定时任务。
*
* Argument(s) : timer - 时间计数值，0表示立即执行，非0表示延迟执行
*               reload - 任务执行周期
*               task - 要运行的任务函数
*
* Note(s)     : none.
*********************************************************************************************************
*/
void OS_CreateTask(u16 timer, u16 reload, TaskHook task)
{
    u8 idata id;
    for (id = 0; id < OS_TASKS_LEN; id++)
    {
        if (os_task_pool[id].task_handle == NULL)
            break;
    }
    if (id == OS_TASKS_LEN) //任务已满
        return;
    
    os_task_pool[id].run = (timer == 0) ? true : false;
    os_task_pool[id].timer = (timer != 0) ? timer: reload;
    os_task_pool[id].reload = reload;
    os_task_pool[id].task_handle = task;
}

/*
*********************************************************************************************************
* Description : 删除（取消）定时任务。
*
* Argument(s) : task - 要取消的任务函数
*
* Note(s)     : 后面的任务向前移动
*********************************************************************************************************
*/
void OS_DeleteTask(TaskHook task)
{
    u8 idata id;

    if (task == NULL)
        return;

    for (id = 0; id < OS_TASKS_LEN; id++)
    {
        if (os_task_pool[id].task_handle == task)
            break;
    }
    if (id == OS_TASKS_LEN) //没有匹配的任务
        return;
    Init_Taks(id);

    while ((id + 1) < OS_TASKS_LEN)
    {
        os_task_pool[id].run = os_task_pool[id + 1].run;
        os_task_pool[id].timer = os_task_pool[id + 1].timer;
        os_task_pool[id].reload = os_task_pool[id + 1].reload;
        os_task_pool[id].task_handle = os_task_pool[id + 1].task_handle;
        id++;
    }
}

/*
*********************************************************************************************************
* Description : 任务标记
*
* Argument(s) : none.
*
* Note(s)     : 在定时器中断中调用此函数.注意不同Bank的影响
*********************************************************************************************************
*/
void OS_TaskMark(void)
{
    u8 idata id;
    for (id = 0; id < OS_TASKS_LEN; id++) //逐个任务时间处理
    {
        if (os_task_pool[id].timer) //时间不为0
        {
            os_task_pool[id].timer--;        //减去一个节拍
            if (os_task_pool[id].timer == 0) //时间减完了
            {
                os_task_pool[id].timer = os_task_pool[id].reload; //恢复计时器值，从新下一次。值大于0才能产生下次触发
                os_task_pool[id].run = true;                      //任务可以运行
            }
        }
    }
}

/*
*********************************************************************************************************
* Description : 任务处理
*
* Argument(s) : none.
*
* Note(s)     : 在定时器中断中调用此函数.注意不同Bank的影响
*********************************************************************************************************
*/
uint8_t idata task_index = 0;
void OS_TaskProcess(void)
{
    if (os_task_pool[task_index].run == true)
    {
        os_task_pool[task_index].task_handle(); //运行任务
        os_task_pool[task_index].run = false;   //清标志

        if (os_task_pool[task_index].reload == 0) //任务只要求运行一次，删除任务
        {
            OS_DeleteTask(os_task_pool[task_index].task_handle);
            task_index--;   //删除任务后面任务会前移。后面要+1，这里先-1下。
        }
    }
    if (++task_index >= OS_TASKS_LEN)
        task_index = 0;
}

/**********************************************END******************************************************/
