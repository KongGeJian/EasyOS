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
*             + 栈空间：SP是8位，只能指向idata。并且栈内存没法通过malloc动态分配，这里用户自定义
*             + 内存分配：init_mempool、malloc分配的内存，每个内存段会额外占用4个字节记录被分配的内存占用的长度信息
+             + 寄存器组：任务运行使用bank=0，定时器T0调度用bank=1，编译后的代码，还使用bank=0，编译有bug
*********************************************************************************************************
*/

#include "os.h"



/*
*********************************************************************************************************
*                                               DEFINE
*********************************************************************************************************
*/

#define ONE_TASK_HEAP_SIZE 29   //一个任务占用的堆空间大小，OS_TASK
#define STACK_DEFVAL 0xA5       //栈默认值

//任务堆区
uint8_t xdata malloc_mempool[(OS_TASK_NUM + 1) * ONE_TASK_HEAP_SIZE + 11 + 4] _at_ 0x0200;
//就绪任务链表
OS_TASK_LIST_TYP xdata * xdata ready_task_list _at_ 0x01F3;
//执行中任务
OS_TASK_TYP xdata * xdata running_task _at_ 0x01F0;

//空闲任务栈
u8 idata idle_task_stack[20];
//空闲任务名称
const char code idle_task_name[] = "OS_IdleTask";



/*
*********************************************************************************************************
*                                         PRIVATE FUNCTION
*********************************************************************************************************
*/

//初始化任务
void _Init_Task(OS_TASK_TYP *task, OS_TASK_STACK_TYP *stack) large
{
    stack->base = NULL;
    stack->len = 0;
    stack->sp = NULL;

    task->handle = NULL;    //函数地址，指针类型
    task->name = NULL;
    task->state = NONE;
    task->timer = 0;        //计数值 TODO 用来做信号量
    task->stack = stack;
    task->prev = NULL;
    task->next = NULL;
}

//删除任务
void _Delete_Task(void) large
{
    OS_TASK_TYP xdata *t_p, *del_p;

    t_p = ready_task_list->top;
    while (t_p != NULL)
    {
        if (t_p->state == DEAD)
        {
            del_p = t_p;
            if (t_p == ready_task_list->top)
            {
                ready_task_list->top = t_p->next;
                ready_task_list->size--;
                if (ready_task_list->size == 0)
                    ready_task_list->end = NULL;
            }
            else if (t_p == ready_task_list->end)
            {
                ready_task_list->end = t_p->prev;
                ready_task_list->size--;
            }
            else
            {
                t_p->prev->next = t_p->next;
                t_p->next->prev = t_p->prev;
            }
            t_p = t_p->next;
            
            BSP_UART_Println(UART1, "Task delete. task_p=%p task_name=%s", del_p, del_p->name);
            free(del_p->stack);
            free(del_p);
        }
        else
            t_p = t_p->next;
    }      
}

//就绪任务压栈
void _Push_Ready_Task(OS_TASK_TYP *task) large
{
    if (task == NULL)
        return;
    if (ready_task_list->end == NULL)
    {
        ready_task_list->top = task;
        ready_task_list->end = task;
        ready_task_list->size++;
    }
    else
    {
        ready_task_list->end->next = task;
        task->prev = ready_task_list->end;
        ready_task_list->end = task;
        ready_task_list->size++;
    }
}

//就绪任务弹栈
OS_TASK_TYP * _Pop_Ready_Task(void) large
{
    OS_TASK_TYP xdata *t_p;

    if (ready_task_list->top == NULL)
        return NULL;

    t_p = ready_task_list->top;

    ready_task_list->top = t_p->next;
    ready_task_list->top->prev = NULL;
    ready_task_list->size--;

    t_p->next = NULL;
    t_p->prev = NULL;
    return t_p;
}

//栈溢出
boolean _Stack_Overflow(OS_TASK_STACK_TYP *stack) large
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
void _Stack_Overflow_Check(void) large
{
    OS_TASK_TYP xdata *t_p;

    t_p = ready_task_list->top;
    while (t_p != NULL)
    {
        if (_Stack_Overflow(t_p->stack))
            OS_StackOverflow(t_p->handle, t_p->name);
        t_p = t_p->next;
    }
    t_p = running_task;
    if (t_p != NULL)
    {
        if (_Stack_Overflow(t_p->stack))
            OS_StackOverflow(t_p->handle, t_p->name);
    }
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
    static const char code task_name[] = "OS_Init";

    memset(malloc_mempool, '\0', sizeof(malloc_mempool));
    init_mempool(&malloc_mempool, sizeof(malloc_mempool));  //初始化堆内存池

    ready_task_list = malloc(sizeof(OS_TASK_LIST_TYP));
    if (ready_task_list == NULL)
        OS_StackOverflow(NULL, task_name);
    ready_task_list->top = NULL;
    ready_task_list->end = NULL;
    ready_task_list->size = 0;
    
    running_task = NULL;
    
    //创建守护任务，放在就绪任务栈顶
    OS_CreateTask(OS_IdleTask, idle_task_name, idle_task_stack, sizeof(idle_task_stack));

    OS_TIMER_Init();
}

/*
*********************************************************************************************************
* Description : 系统启动
*
* Note(s)     : none.
*********************************************************************************************************
*/
void OS_Start(void) large
{
    OS_Task_SW();
    if (running_task != NULL)
    {
        running_task->timer++;
        origin_sp = SP;
        SP = running_task->stack->sp; //函数RET，弹栈，运行
    }

    os_start_enable = 1;
    ET0 = 1;    //使能定时器中断
    TR0 = 1;    //启动定时器
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
* Note(s)     : none.
*********************************************************************************************************
*/
OS_TASK_TYP * OS_CreateTask(TaskHook task, const char code *task_name, u8 idata *stack_base, u8 stack_len) large
{
    OS_TASK_STACK_TYP xdata *ts_p;
    OS_TASK_TYP xdata *t_p;

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
        OS_StackOverflow(task, task_name);
    
    //初始化堆任务参数
    _Init_Task(t_p, ts_p);

    t_p->stack->base = stack_base;
    t_p->stack->len = stack_len;
    *stack_base = (u16) task;       //任务入口地址压栈
    stack_base++;
    *stack_base = (u16) task >> 8;
    if (task != OS_IdleTask)        //其他任务模拟压栈
    {
        stack_base++;
        *stack_base = 0;    //ACC
        stack_base++;
        *stack_base = 0;    //B
        stack_base++;
        *stack_base = 0;    //DPH
        stack_base++;
        *stack_base = 0;    //DPL
        stack_base++;
        *stack_base = 0;    //PSW
        stack_base++;
        *stack_base = 0;    //AR0
        stack_base++;
        *stack_base = 0;    //AR1
        stack_base++;
        *stack_base = 0;    //AR2
        stack_base++;
        *stack_base = 0;    //AR3
        stack_base++;
        *stack_base = 0;    //AR4
        stack_base++;
        *stack_base = 0;    //AR5
        stack_base++;
        *stack_base = 0;    //AR6
        stack_base++;
        *stack_base = 0;    //AR7
    }
    t_p->stack->sp = stack_base;

    t_p->handle = task;
    t_p->name = task_name;
    t_p->state = READY;

    //任务链表挂载
    _Push_Ready_Task(t_p);

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

    _Delete_Task();
    return;
}

/*
*********************************************************************************************************
* Description : 任务切换
*
* Argument(s) : none.
*
* Note(s)     : 在定时器中断中调用此函数.注意不同Bank的影响
*********************************************************************************************************
*/
void OS_Task_SW(void) large
{
    if (running_task != NULL)
    {
        running_task->state = READY;
        _Push_Ready_Task(running_task);
    }
    running_task = _Pop_Ready_Task();
    if (running_task != NULL)
    {
        running_task->state = RUNNING;
    }
    //栈溢出检测
    _Stack_Overflow_Check();
}

/*
*********************************************************************************************************
* Description : 空闲任务（守护任务）
*
* Argument(s) : none.
*
* Note(s)     : OS_Start 是自动创建，用于空闲统计、任务回收 等
*********************************************************************************************************
*/
void OS_IdleTask(void) large
{
 
    while (1)
    {
        //空闲统计 task->timer
        //任务回收
        _Delete_Task();
        
        delay_ms(2);
    }
}


void OS_StackOverflow(TaskHook task, const char code *task_name) large
{
    BSP_UART_Println(UART1, "Not enough memory space. task_name=%s p=%p", task_name, task);
    while(1);
}

/**********************************************END******************************************************/
