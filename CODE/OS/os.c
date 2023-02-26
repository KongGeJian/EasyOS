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
*             + 栈：51是满增栈，即SP总是指向最后一个入栈的字节的地址，每入栈一次SP+1
*             + 栈空间：SP是8位，只能指向idata。并且栈内存没法通过malloc动态分配，这里用户自定义
+             + 寄存器组：任务运行使用bank=0，定时器T0调度用bank=1，编译后的代码，定时调用函数还再用bank=0，编译有bug
*********************************************************************************************************
*/

#include "os.h"



/*
*********************************************************************************************************
*                                               DEFINE
*********************************************************************************************************
*/

#define STACK_DEFVAL 0xA5       //栈默认值

//执行中任务
volatile OS_TASK_TYP xdata * xdata running_task _at_ 0x01F0;
//就绪中任务链表
OS_TASK_LIST_TYP xdata * xdata ready_task_list _at_ 0x01F2;
//阻塞中任务链表
OS_TASK_LIST_TYP xdata * xdata blocked_task_list _at_ 0x01F4;
//隐藏中任务链表：挂起+死亡
OS_TASK_LIST_TYP xdata * xdata hidden_task_list _at_ 0x01F6;

//空闲任务栈
u8 idata idle_task_stack[20];
//空闲任务名称
const char code idle_task_name[] = "OS_IdleTask";



/*
*********************************************************************************************************
*                                         PRIVATE FUNCTION
*********************************************************************************************************
*/

//初始化任务字段
void _Init_Task(OS_TASK_TYP xdata *task, OS_TASK_STACK_TYP xdata *stack) large
{
    stack->base = NULL;
    stack->len = 0;
    stack->sp = NULL;

    task->handle = NULL;    //函数地址，指针类型
    task->name = NULL;
    task->stack = stack;
    task->state = READY;
    task->time_statistics = 0;
    task->time_slice = 0;
    task->blocked_time = 0;
    task->blocked_semaphore = 0;
    task->prev = NULL;
    task->next = NULL;
}

//删除任务
void _Delete_Task(OS_TASK_LIST_TYP xdata *task_list) large
{
    OS_TASK_TYP xdata *t_p, *del_p;

    if (task_list == NULL)
        return;

    t_p = task_list->top;
    while (t_p != NULL)
    {
        if (t_p->state == DEAD)
        {
            del_p = t_p;
            if (t_p == task_list->top)
            {
                task_list->top = t_p->next;
                task_list->size--;
                if (task_list->size == 0)
                    task_list->end = NULL;
            }
            else if (t_p == task_list->end)
            {
                task_list->end = t_p->prev;
                task_list->size--;
            }
            else
            {
                t_p->prev->next = t_p->next;
                t_p->next->prev = t_p->prev;
                task_list->size--;
            }
            t_p = t_p->next;

            BSP_UART_Println(UART1, "Task delete. task_p=%p task_name=%s", del_p, del_p->name);
            free(del_p->stack);
            free(del_p);
        }
        else
        {
            t_p = t_p->next;
        }
    }
}

//任务链表压栈
void _Push_TaskList(OS_TASK_LIST_TYP xdata *task_list, OS_TASK_TYP xdata *task) large
{
    if (task_list == NULL || task == NULL)
        return;
    if (task_list->end == NULL)
    {
        task_list->top = task;
        task_list->end = task;
        task_list->size++;
    }
    else
    {
        task_list->end->next = task;
        task->prev = task_list->end;
        task_list->end = task;
        task_list->size++;
    }
}

//任务链表弹栈
OS_TASK_TYP * _Pop_TaskList(OS_TASK_LIST_TYP xdata *task_list) large
{
    OS_TASK_TYP xdata *t_p;

    if (task_list == NULL || task_list->top == NULL)
        return NULL;

    t_p = task_list->top;

    task_list->top = t_p->next;
    task_list->size--;
    if (task_list->size == 0)
        task_list->end = NULL;
    else
        task_list->top->prev = NULL;

    t_p->next = NULL;
    t_p->prev = NULL;
    return t_p;
}

//是否栈溢出
boolean _Is_StackOverflow(OS_TASK_STACK_TYP xdata *stack) large
{
    u8 idata * idata p;
    if (stack == NULL)
        return false;
    
    p = stack->base + stack->len - 1;
    if (*p != STACK_DEFVAL)
        return true;
    else
        return false;
}

//栈溢出检查
void _StackOverflow_Check(OS_TASK_TYP xdata *task) large
{
    if (task == NULL)
        return;

    if (_Is_StackOverflow(task->stack))
        OS_StackOverflow(task->handle, task->name);
}

//堆内存不足
void _Heap_NotEnough(const char code *func_name, const char code *var) large
{
    BSP_UART_Println(UART1, "Not enough memory space. func_name=%s var=%p", func_name, var);
    while (1);
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
void OS_Init(void) large
{
    static const char code func_name[] = "OS_Init";

    running_task = NULL;
    
    ready_task_list = malloc(sizeof(OS_TASK_LIST_TYP));
    if (ready_task_list == NULL)
        _Heap_NotEnough(func_name, "ready_task_list");
    ready_task_list->top = NULL;
    ready_task_list->end = NULL;
    ready_task_list->size = 0;

    blocked_task_list = malloc(sizeof(OS_TASK_LIST_TYP));
    if (blocked_task_list == NULL)
        _Heap_NotEnough(func_name, "blocked_task_list");
    blocked_task_list->top = NULL;
    blocked_task_list->end = NULL;
    blocked_task_list->size = 0;

    hidden_task_list = malloc(sizeof(OS_TASK_LIST_TYP));
    if (hidden_task_list == NULL)
        _Heap_NotEnough(func_name, "hidden_task_list");
    hidden_task_list->top = NULL;
    hidden_task_list->end = NULL;
    hidden_task_list->size = 0;
    
    //创建守护任务，放在就绪任务栈顶
    OS_CreateTask(OS_IdleTask, idle_task_name, idle_task_stack, sizeof(idle_task_stack));

    OS_TIMER_Init();
}

/*
*********************************************************************************************************
* Description : 系统启动
*
* Note(s)     : 启动后第一个任务是 OS_IdleTask，这里只需RET弹出函数入口地址到PC即可。
*********************************************************************************************************
*/
void OS_Start(void) large
{
    OS_TaskSwitch();
    if (running_task != NULL)
    {
        origin_sp = SP;
        SP = running_task->stack->sp; //函数RET，弹栈，运行
    }
    ET0 = 1;    //使能定时器中断
    TR0 = 1;    //启动定时器
}

/*
*********************************************************************************************************
* Description : 任务标记
*
* Argument(s) : none.
*
* Note(s)     : 标记只处理：运行中任务、阻塞中任务
*********************************************************************************************************
*/
void OS_TaskMark(void) large
{
    OS_TASK_TYP xdata *t_p;
    register u8 i;
    //运行中任务
    if (running_task != NULL)
    {
        running_task->time_statistics++;
        if (running_task->time_slice != 0)
            running_task->time_slice--;
        if (running_task->blocked_time != 0)
            running_task->blocked_time--;
    }
    //阻塞中任务
    for (i = blocked_task_list->size; i > 0; i--)
    {
        t_p = _Pop_TaskList(blocked_task_list);
        if (t_p == NULL)
            continue;
        if (t_p->blocked_time != 0)
            t_p->blocked_time--;
        if (t_p->blocked_time == 0 && t_p->blocked_semaphore == 0)
        {
            t_p->state = READY;
            _Push_TaskList(ready_task_list, t_p);
        }
        else
        {
            _Push_TaskList(blocked_task_list, t_p);
        }
    }
}

/*
*********************************************************************************************************
* Description : 任务切换
*
* Argument(s) : none.
*
* Note(s)     : 在定时器中断中调用此函数。注意不同Bank的影响
*********************************************************************************************************
*/
void OS_TaskSwitch(void) large
{
    if (running_task != NULL && running_task->time_slice != 0)
        return;
    //栈溢出检测，只需检测当前运行中的任务即可
    _StackOverflow_Check(running_task);
    //运行中任务，状态切换。运行中 -> 就绪、阻塞、死亡
    if (running_task != NULL)
    {
        switch (running_task->state)
        {
        case RUNNING:
            running_task->state = READY;
            _Push_TaskList(ready_task_list, running_task);
            break;
        case BLOCKED:
            if (running_task->blocked_time == 0 && running_task->blocked_semaphore == 0)
            {
                running_task->state = READY;
                _Push_TaskList(ready_task_list, running_task);
            }
            else
            {
                _Push_TaskList(blocked_task_list, running_task);
            }
            break;
        case DEAD:
            _Push_TaskList(hidden_task_list, running_task);
            break;
        }
    }
    //从就绪队列弹出一个可运行任务
    running_task = _Pop_TaskList(ready_task_list);
    while (running_task != NULL)
    {
        if (running_task->state == READY)
        {
            running_task->state = RUNNING;
            running_task->time_slice = OS_TIME_SLICE;
            running_task->blocked_time = 0xFF;
            break;
        }
        else if (running_task->state == DEAD)
        {
            _Push_TaskList(hidden_task_list, running_task);
            running_task = _Pop_TaskList(ready_task_list);
        }
    }
}

/*
*********************************************************************************************************
* Description : 任务等待
*
* Argument(s) : event - 事件类型。参考OS_EVENT_E_TYP。EVENT_INTERVAL 和 EVENT_TIMEOUT 不可同时使用。EVENT_TIMEOUT | EVENT_SIGNAL 表示倒计时结束，同时信号到达
*               nms - 延迟时间。nms=0，表示让出当前cpu
*               semaphore - 需等待信号量。8位长度，用户可自定义使用，比如按位定义8个信号。
*
* Note(s)     : 只处理当前运行中任务。当前任务进入阻塞状态，不会立即进入调度切换，等待定时器下次tick时切换。
*********************************************************************************************************
*/
void OS_TaskWait(OS_EVENT_E_TYP event, u8 nms, u8 semaphore) compact reentrant
{
    u8 elapsed;
    if (running_task == NULL)
        return;
    running_task->state = BLOCKED;
    // EVENT_INTERVAL 和 EVENT_TIMEOUT，优先取EVENT_INTERVAL
    if ((event & EVENT_INTERVAL) == EVENT_INTERVAL)
    {
        elapsed = 0xFF - running_task->blocked_time;
        running_task->blocked_time = (elapsed >= nms) ? 0 : (nms - elapsed);
    }
    else if ((event & EVENT_TIMEOUT) == EVENT_TIMEOUT)
    {
        running_task->blocked_time = nms;
    }
    if ((event & EVENT_SIGNAL) == EVENT_SIGNAL)
    {
        running_task->blocked_semaphore = semaphore;
    }

    running_task->time_slice = 0;   //时间片归0，让出CPU
    while (running_task->state != RUNNING); //等下下次进入运行态，继续开始运行
}

/*
*********************************************************************************************************
* Description : 任务信号到达
*
* Argument(s) : task - 任务句柄。调用 OS_CreateTask 的返回值。
*               semaphore - 到达信号。无论用户定怎么定义，这里只需按位清楚即可，比如传入 0x02, 将任务中第2位(从第到高)清0。
*
* Note(s)     : 理论上，task应该只会处于阻塞队列中；同时不能对当前任务设置。
*********************************************************************************************************
*/
void OS_TaskSignal(OS_TASK_TYP xdata *task, u8 semaphore) large
{
    if (task == NULL || task == running_task)
        return;
    task->blocked_semaphore &= ~semaphore;
}

/*
*********************************************************************************************************
* Description : 创建定时任务。
*
* Argument(s) : task - 要运行的任务函数
*               task_name - 任务名
*               stack_base - 任务栈首地址
*               stack_len - 任务栈长度，最小16
*
* Return(s)   : 创建好的任务地址
*
* Note(s)     : none.
*********************************************************************************************************
*/
OS_TASK_TYP * OS_CreateTask(TaskHook task, const char code *task_name, u8 idata *stack_base, u8 stack_len) large
{
    OS_TASK_STACK_TYP xdata *ts_p;
    OS_TASK_TYP xdata *t_p;
    register u8 i;

    if (stack_len < 16)
    {
        BSP_UART_Println(UART1, "Task stack minimum 16. task_name=%s stack_len=%bu", task_name, stack_len);
        while(1);
    }

    //栈全部初始化成 STACK_DEFVAL，用于栈溢出检测
    memset(stack_base, STACK_DEFVAL, stack_len);

    //分配任务堆内存
    ts_p = malloc(sizeof(OS_TASK_STACK_TYP));
    t_p = malloc(sizeof(OS_TASK_TYP));
    if (ts_p == NULL || t_p == NULL)
        _Heap_NotEnough("OS_CreateTask", task_name);
    
    //初始化堆任务字段
    _Init_Task(t_p, ts_p);

    t_p->stack->base = stack_base;
    t_p->stack->len = stack_len;
    *stack_base = (u16) task;       //任务入口地址压栈
    stack_base++;
    *stack_base = (u16) task >> 8;
    if (task != OS_IdleTask)        //其他任务模拟压栈
    {
        for (i = 0; i < 13; i++)
        {
            stack_base++;
            *stack_base = 0;        //ACC B DPH DPL PSW AR0-AR7
        }
    }
    t_p->stack->sp = stack_base;

    t_p->handle = task;
    t_p->name = task_name;
    t_p->state = READY;

    //任务链表挂载
    _Push_TaskList(ready_task_list, t_p);

    return t_p;
}

/*
*********************************************************************************************************
* Description : 删除（取消）定时任务。
*
* Argument(s) : task - 要取消的任务函数
*
* Note(s)     : 任务可能正在运行，这里只标记，真正销毁是在任务切换的时候
*********************************************************************************************************
*/
void OS_DeleteTask(OS_TASK_TYP xdata *task) large
{
    if (task == NULL)
        return;
    task->state = DEAD;
}

/*
*********************************************************************************************************
* Description : 空闲任务（守护任务）
*
* Argument(s) : none.
*
* Note(s)     : 用户禁止创建，OS_Start 是自动创建，用于空闲统计、任务回收 等
*********************************************************************************************************
*/
void OS_IdleTask(void) large
{
    while (1)
    {
        //空闲统计 task->timer
        //任务回收
        _Delete_Task(hidden_task_list);
        //让出CPU
        OS_TaskWait(EVENT_TIMEOUT, 0, 0);
    }
}

/*
*********************************************************************************************************
* Description : 栈溢出
*
* Argument(s) : none.
*
* Note(s)     : 栈溢出，打印任务信息，程序进入死循环。
*********************************************************************************************************
*/
void OS_StackOverflow(TaskHook task, const char code *task_name) large
{
    ET0 = 0;    //关闭T0中断
    TR0 = 0;    //关闭T0
    BSP_UART_Println(UART1, "Stack overflow. task_name=%s task_p=%p", task_name, task);
    while(1);
}

/**********************************************END******************************************************/
