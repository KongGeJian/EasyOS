# EasyOS

简单OS/RTOS

---
---

# 清单

```
MCU：STC8G1K08 (1T)
FOSC：内部时钟 12.000M Hz 
```

# 目录结构

代码在 ./CODE 目录下
```
+---CODE
|       main.c - 主程序
|       cfg.h - 配置头文件
|       STARTUP.A51
|
|       lib_delay.c - 延迟
|       lib_delay.h
|       lib_stdint.h - C99引入的C标准库，C51没有，这里单独引入
|
|       os.c - RTOS主实现
|       os.h
|       os_timmer.c - 系统计时器
|       os_timmer.h
```

---
---

# RTOS介绍

T0作为系统时钟定时器，设定一个系统时钟os_tick=1ms；

这里有两种

## 简单调度

> git 检出 tag xxx

特点：
+ 创建任务自定义周期时间
+ 定时器中断只负责系统 os_tick 递增；标记注册任务自己计时器
+ 任务执行完成等待下个周期，运行期间中断不能打断任务
+ 任务逻辑简单，可以简单理解成协作式调度

注意：
+ 中断不能打断任务，如果任务执行时间超过定义周期时间，任务不会主动让出
+ 任务的时间片就是任务自身执行时间，不固定

## 固定时间片调度

> git 检出 tag xxx

特点：
+ xxx

注意：
+ xxx
