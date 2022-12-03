/*
*********************************************************************************************************
*                                      OPERATING SYSTEM PACKAGE
*
* Name    : 系统计时器
* Version : V1.0
* Author  : 空格键
* ----------------------------
* Note(s) : 时间单位，有os_timer.c决定。这里是ms
* Tip(s)  : 说明：
*             + 堆栈：51是满增栈，即SP总是指向最后一个入栈的字节的地址，每入栈一次SP+1
*********************************************************************************************************
*/

#include "os.h"



/*
*********************************************************************************************************
*                                               DEFINE
*********************************************************************************************************
*/

//任务栈
typedef struct OS_TASK_STACK
{
    uint8_t base;   //栈低
    uint8_t len;    //栈大小
    uint8_t sp;     //当前栈顶
    OS_TASK_TYP *task;  //绑定任务
} OS_TASK_STACK_TYP;

//系统任务
typedef struct OS_TASK
{
    char *name;             //任务名称
    TaskHook handle;        //要运行的任务函数
    volatile OS_TASK_STATE_E_TYP state;   //程序状态
    uint16_t timer;         //计时器计数值，看定时器调度时间
    // uint16_t reload;        //为0时不能重复调用
    OS_TASK_STACK_TYP *stack;   //绑定的栈
} OS_TASK_TYP;

//任务池
OS_TASK_TYP xdata task_list[OS_TASKS_NUM];
//任务栈
OS_TASK_STACK_TYP idata task_stack_list[OS_TASKS_NUM];



/*
*********************************************************************************************************
*                                         PRIVATE FUNCTION
*********************************************************************************************************
*/

//初始化话任务
void Init_Task(u8 id)
{
    if (id >= OS_TASKS_NUM)
        return;
    task_list[id].state = NONE;
    task_list[id].name = NULL;
    task_list[id].handle = NULL;    //函数地址，指针类型
    task_list[id].timer = 0;        //计数值 TODO 用来做信号量
    task_list[id].reload = 0;       //重装值
}

//初始化任务结构体
void Init_TaskList(void)
{
    u8 idata id;
    for (id = 0; id < OS_TASKS_NUM; id++)
        Init_Task(id);
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
* Argument(s) : task - 要运行的任务函数
*               task_name - 任务名
*               timer - 时间计数值，0表示立即执行，非0表示延迟执行
*               reload - 任务执行周期
*
* Note(s)     : none.
*********************************************************************************************************
*/
void OS_CreateTask(TaskHook task, const char *task_name)
{
    u8 idata id;
    for (id = 0; id < OS_TASKS_NUM; id++)
    {
        if (task_list[id].handle == NULL)
            break;
    }
    if (id == OS_TASKS_NUM) //任务已满
        return;
    
    task_list[id].timer = 0;
    task_list[id].reload = 0;
    task_list[id].handle = task;
    task_list[id].name = task_name;
    task_list[id].state = READY;
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

    for (id = 0; id < OS_TASKS_NUM; id++)
    {
        if (task_list[id].handle == task)
            break;
    }
    if (id == OS_TASKS_NUM) //没有匹配的任务
        return;
    Init_Taks(id);

    while ((id + 1) < OS_TASKS_NUM)
    {
        task_list[id].state = task_list[id + 1].state;
        task_list[id].name = task_list[id + 1].name;
        task_list[id].handle = task_list[id + 1].handle;
        task_list[id].timer = task_list[id + 1].timer;
        task_list[id].reload = task_list[id + 1].reload;
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
    for (id = 0; id < OS_TASKS_NUM; id++) //逐个任务时间处理
    {
        if (task_list[id].timer) //时间不为0
        {
            task_list[id].timer--;        //减去一个节拍
            if (task_list[id].timer == 0) //时间减完了
            {
                task_list[id].timer = task_list[id].reload; //恢复计时器值，从新下一次。值大于0才能产生下次触发
                task_list[id].state = READY;                      //任务可以运行
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
    if (task_list[task_index].run == true)
    {
        task_list[task_index].task_handle(); //运行任务
        task_list[task_index].run = false;   //清标志

        if (task_list[task_index].reload == 0) //任务只要求运行一次，删除任务
        {
            OS_DeleteTask(task_list[task_index].task_handle);
            task_index--;   //删除任务后面任务会前移。后面要+1，这里先-1下。
        }
    }
    if (++task_index >= OS_TASKS_NUM)
        task_index = 0;
}

/**********************************************END******************************************************/
