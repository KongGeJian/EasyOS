# 1. 项目

`EasyOS` 简单OS/RTOS。

---
---

# 2. 清单

```
MCU：STC8G1K08 (1T)
FOSC：内部时钟 12.000M Hz 
```

# 3. 目录结构

代码在 ./CODE 目录下
```
+---APP - 程序入口
|       main.c - 主程序
|       cfg.h - 配置头文件
|       STARTUP.A51
|
+---BSP - 板载支持包。bsp_xxx 为BSP各个子模块
|       bsp.c - bsp入口文件
|       bsp.h - bsp入口头文件
|       bsp_uart.c
|       bsp_uart.h
|
+---EXT-LIB - 扩展的公共包
|       lib_delay.c - 延迟
|       lib_delay.h
|       lib_stdint.h - C99引入的C标准库，C51没有，这里单独引入
|
\---OS - 简单实现的RTOS
        os.c - RTOS主实现
        os.h
        os_timmer.c - 系统计时器
        os_timmer.h
        os_core.asm - T0任务切换
```

---
---

# 4. RTOS介绍

T0作为系统时钟定时器，设定一个系统时钟 `os_tick=1ms`。

这里有两种：
+ 简单调度: 是定义多个task，定时器标记每个任务的时间计时。main loop 中分别处理延时时间到达的任务。整体上所有任务还是串行，任务顺序固定，但是代码空间开销小。
+ 固定时间片调度: 是自个任务独享内存堆栈。任务顺序通过链表组织，任务挂起立即释放CPU。CPU资源利用高，但是代码空间开销大。


## 简单调度

> `git checkout feature/simple-scheduling`

特点：
+ 创建任务自定义周期时间
+ 定时器中断只负责系统 os_tick 递增；标记注册任务自己计时器
+ 任务执行完成等待下个周期，运行期间中断不能打断任务
+ 任务逻辑简单，可以简单理解成协作式调度

注意：
+ 中断不能打断任务，如果任务执行时间超过定义周期时间，任务不会主动让出
+ 任务的时间片就是任务自身执行时间，不固定
+ 任务不能有死循环，否则其他任务无法执行

## 固定时间片调度

> `git checkout feature/fixed-time-slice-scheduling`

特点：
+ 定时器中断只负责系统 `os_tick` 递增；`OS_TIME_SLICE` 表示多少个tick，执行一次任务切换
+ 每个任务有独立的堆栈，任务中要有 while(1) 循环；同一函数可创建多个任务（注意要可重入）
+ 支持任务删除：任务删除延迟执行，在空闲任务中操作回收
+ 支持任务延迟：OS_TaskWait(EVENT_TIMEOUT, 10, 0) 用于任务延迟10个tick，并释放CPU
+ 支持信号量：OS_TaskWait(EVENT_SIGNAL, 0, 0x02) 指定等待0x02信号，并释放CPU；OS_TaskSignal(task, 0x02) 通知指定任务信号达到。信号可用户自定义

注意：
+ 任务栈需要用户自定义，每个栈至少要16个字节。
+ 内存堆需要用户根据硬件内存，合理设置 `MEM_POOL_SIZE`
+ 只是单纯实现任务基本功能，就需要 Program Size: data=143.3 xdata=585 const=354 code=7981

# 5. 优化

**1. 指针**

Cx51指针分两种：`通用指针（3个字节）`和 `指定内存指针（1个或2个字节）`。使用指定内存指针，可以显著加速8051 C 程序。<br>
参考：https://www.keil.com/support/man/docs/c51/c51_le_ptrs.htm

**2. 任务状态**

任务状态，参考 https://zhuanlan.zhihu.com/p/503838768 <br>
这里增加 `DEAD-死亡态`，延迟删除任务。
