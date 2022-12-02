
#line 1 "CODE\os.c" /0










 
 
  
#line 1 "CODE\os.h" /0








 
 
 
 
 




 
 
  
#line 1 "C:\DevelopTool\Keil_v5\C51\Inc\stdlib.h" /0







 
 
 
 
 
 
 
 typedef unsigned int size_t;
 
 
 
 
 
 
 
 
 typedef char wchar_t;
 
 
 
#line 28 "C:\DevelopTool\Keil_v5\C51\Inc\stdlib.h" /1
 
 
 
 
 
 
 
 
 
  
 
 
#line 40 "C:\DevelopTool\Keil_v5\C51\Inc\stdlib.h" /0
 
 
 
 
 
 
#line 46 "C:\DevelopTool\Keil_v5\C51\Inc\stdlib.h" /1
 
 
 
 
 
 
#line 52 "C:\DevelopTool\Keil_v5\C51\Inc\stdlib.h" /0
 
 #pragma SAVE
 #pragma REGPARMS
 
 extern char   cabs (char  val);
 
 extern int    abs  (int   val);
 
 extern long   labs (long  val);
 
 extern float  atof (const char *s1);
 extern long   atol (const char *s1);
 extern int    atoi (const char *s1);
 extern int    rand (void);
 extern void   srand (unsigned int seed);
 
 
#line 69 "C:\DevelopTool\Keil_v5\C51\Inc\stdlib.h" /1
 
  
 
 
#line 73 "C:\DevelopTool\Keil_v5\C51\Inc\stdlib.h" /0
 
 extern float         strtod  (const char *nptr, char **endptr);
 extern long          strtol  (const char *nptr, char **endptr, unsigned char base);
 extern unsigned long strtoul (const char *nptr, char **endptr, unsigned char base);
 
 
 
 extern void init_mempool          (void xdata *p, size_t size);
 extern void xdata *malloc  (size_t size);
 extern void free                  (void xdata *p);
 extern void xdata *realloc (void xdata *p, size_t size);
 extern void xdata *calloc  (size_t nmemb, size_t size);
 
 
#line 87 "C:\DevelopTool\Keil_v5\C51\Inc\stdlib.h" /1
 
 
#line 89 "C:\DevelopTool\Keil_v5\C51\Inc\stdlib.h" /0
 
 #pragma RESTORE
 
 
#line 20 "CODE\os.h" /0
 
 
  
#line 1 "CODE\cfg.h" /0










 
 
 
 
 
 
 
  
#line 1 "C:\DevelopTool\Keil_v5\C51\Inc\STC\STC8G.H" /0
 
 
 
 
 
 
 
 sfr         P0          =           0x80;
 sbit        P00         =           P0^0;
 sbit        P01         =           P0^1;
 sbit        P02         =           P0^2;
 sbit        P03         =           P0^3;
 sbit        P04         =           P0^4;
 sbit        P05         =           P0^5;
 sbit        P06         =           P0^6;
 sbit        P07         =           P0^7;
 sfr         SP          =           0x81;
 sfr         DPL         =           0x82;
 sfr         DPH         =           0x83;
 sfr         S4CON       =           0x84;
 sfr         S4BUF       =           0x85;
 sfr         PCON        =           0x87;
 sfr         TCON        =           0x88;
 sbit        TF1         =           TCON^7;
 sbit        TR1         =           TCON^6;
 sbit        TF0         =           TCON^5;
 sbit        TR0         =           TCON^4;
 sbit        IE1         =           TCON^3;
 sbit        IT1         =           TCON^2;
 sbit        IE0         =           TCON^1;
 sbit        IT0         =           TCON^0;
 sfr         TMOD        =           0x89;
 sfr         TL0         =           0x8A;
 sfr         TL1         =           0x8B;
 sfr         TH0         =           0x8C;
 sfr         TH1         =           0x8D;
 sfr         AUXR        =           0x8E;
 sfr         INTCLKO     =           0x8F;
 sfr         P1          =           0x90;
 sbit        P10         =           P1^0;
 sbit        P11         =           P1^1;
 sbit        P12         =           P1^2;
 sbit        P13         =           P1^3;
 sbit        P14         =           P1^4;
 sbit        P15         =           P1^5;
 sbit        P16         =           P1^6;
 sbit        P17         =           P1^7;
 sfr         P1M1        =           0x91;
 sfr         P1M0        =           0x92;
 sfr         P0M1        =           0x93;
 sfr         P0M0        =           0x94;
 sfr         P2M1        =           0x95;
 sfr         P2M0        =           0x96;
 sfr         SCON        =           0x98;
 sbit        SM0         =           SCON^7;
 sbit        SM1         =           SCON^6;
 sbit        SM2         =           SCON^5;
 sbit        REN         =           SCON^4;
 sbit        TB8         =           SCON^3;
 sbit        RB8         =           SCON^2;
 sbit        TI          =           SCON^1;
 sbit        RI          =           SCON^0;
 sfr         SBUF        =           0x99;
 sfr         S2CON       =           0x9A;
 sfr         S2BUF       =           0x9B;
 sfr         IRCBAND     =           0x9D;
 sfr         LIRTRIM     =           0x9E;
 sfr         IRTRIM      =           0x9F;
 sfr         P2          =           0xA0;
 sbit        P20         =           P2^0;
 sbit        P21         =           P2^1;
 sbit        P22         =           P2^2;
 sbit        P23         =           P2^3;
 sbit        P24         =           P2^4;
 sbit        P25         =           P2^5;
 sbit        P26         =           P2^6;
 sbit        P27         =           P2^7;
 sfr         P_SW1       =           0xA2;
 sfr         IE          =           0xA8;
 sbit        EA          =           IE^7;
 sbit        ELVD        =           IE^6;
 sbit        EADC        =           IE^5;
 sbit        ES          =           IE^4;
 sbit        ET1         =           IE^3;
 sbit        EX1         =           IE^2;
 sbit        ET0         =           IE^1;
 sbit        EX0         =           IE^0;
 sfr         SADDR       =           0xA9;
 sfr         WKTCL       =           0xAA;
 sfr         WKTCH       =           0xAB;
 sfr         S3CON       =           0xAC;
 sfr         S3BUF       =           0xAD;
 sfr         TA          =           0xAE;
 sfr         IE2         =           0xAF;
 sfr         P3          =           0xB0;
 sbit        P30         =           P3^0;
 sbit        P31         =           P3^1;
 sbit        P32         =           P3^2;
 sbit        P33         =           P3^3;
 sbit        P34         =           P3^4;
 sbit        P35         =           P3^5;
 sbit        P36         =           P3^6;
 sbit        P37         =           P3^7;
 sfr         P3M1        =           0xB1;
 sfr         P3M0        =           0xB2;
 sfr         P4M1        =           0xB3;
 sfr         P4M0        =           0xB4;
 sfr         IP2         =           0xB5;
 sfr         IP2H        =           0xB6;
 sfr         IPH         =           0xB7;
 sfr         IP          =           0xB8;
 sbit        PPCA        =           IP^7;
 sbit        PLVD        =           IP^6;
 sbit        PADC        =           IP^5;
 sbit        PS          =           IP^4;
 sbit        PT1         =           IP^3;
 sbit        PX1         =           IP^2;
 sbit        PT0         =           IP^1;
 sbit        PX0         =           IP^0;
 sfr         SADEN       =           0xB9;
 sfr         P_SW2       =           0xBA;
 sfr         ADC_CONTR   =           0xBC;
 sfr         ADC_RES     =           0xBD;
 sfr         ADC_RESL    =           0xBE;
 sfr         P4          =           0xC0;
 sbit        P40         =           P4^0;
 sbit        P41         =           P4^1;
 sbit        P42         =           P4^2;
 sbit        P43         =           P4^3;
 sbit        P44         =           P4^4;
 sbit        P45         =           P4^5;
 sbit        P46         =           P4^6;
 sbit        P47         =           P4^7;
 sfr         WDT_CONTR   =           0xC1;
 sfr         IAP_DATA    =           0xC2;
 sfr         IAP_ADDRH   =           0xC3;
 sfr         IAP_ADDRL   =           0xC4;
 sfr         IAP_CMD     =           0xC5;
 sfr         IAP_TRIG    =           0xC6;
 sfr         IAP_CONTR   =           0xC7;
 sfr         P5          =           0xC8;
 sbit        P50         =           P5^0;
 sbit        P51         =           P5^1;
 sbit        P52         =           P5^2;
 sbit        P53         =           P5^3;
 sbit        P54         =           P5^4;
 sbit        P55         =           P5^5;
 sbit        P56         =           P5^6;
 sbit        P57         =           P5^7;
 sfr         P5M1        =           0xC9;
 sfr         P5M0        =           0xCA;
 sfr         P6M1        =           0xcb;
 sfr         P6M0        =           0xcc;
 sfr         SPSTAT      =           0xCD;
 sfr         SPCTL       =           0xCE;
 sfr         SPDAT       =           0xCF;
 sfr         PSW         =           0xD0;
 sbit        CY          =           PSW^7;
 sbit        AC          =           PSW^6;
 sbit        F0          =           PSW^5;
 sbit        RS1         =           PSW^4;
 sbit        RS0         =           PSW^3;
 sbit        OV          =           PSW^2;
 sbit        F1          =           PSW^1;
 sbit        P           =           PSW^0;
 sfr         T4T3M       =           0xD1;
 sfr         T4H         =           0xD2;
 sfr         T4L         =           0xD3;
 sfr         T3H         =           0xD4;
 sfr         T3L         =           0xD5;
 sfr         T2H         =           0xD6;
 sfr         T2L         =           0xD7;
 sfr         CCON        =           0xD8;
 sbit        CF          =           CCON^7;
 sbit        CR          =           CCON^6;
 sbit        CCF2        =           CCON^2;
 sbit        CCF1        =           CCON^1;
 sbit        CCF0        =           CCON^0;
 sfr         CMOD        =           0xD9;
 sfr         CCAPM0      =           0xDA;
 sfr         CCAPM1      =           0xDB;
 sfr         CCAPM2      =           0xDC;
 sfr         ADCCFG      =           0xDE;
 sfr         IP3         =           0xDF;
 sfr         ACC         =           0xE0;
 sfr         P7M1        =           0xe1;
 sfr         P7M0        =           0xe2;
 sfr         DPS         =           0xE3;
 sfr         DPL1        =           0xE4;
 sfr         DPH1        =           0xE5;
 sfr         CMPCR1      =           0xE6;
 sfr         CMPCR2      =           0xE7;
 sfr         P6          =           0xe8;
 sbit        P60         =           P6^0;
 sbit        P61         =           P6^1;
 sbit        P62         =           P6^2;
 sbit        P63         =           P6^3;
 sbit        P64         =           P6^4;
 sbit        P65         =           P6^5;
 sbit        P66         =           P6^6;
 sbit        P67         =           P6^7;
 sfr         CL          =           0xE9;
 sfr         CCAP0L      =           0xEA;
 sfr         CCAP1L      =           0xEB;
 sfr         CCAP2L      =           0xEC;
 sfr         IP3H        =           0xEE;
 sfr         AUXINTIF    =           0xEF;
 sfr         B           =           0xF0;
 sfr         PWMSET      =           0xF1;
 sfr         PCA_PWM0    =           0xF2;
 sfr         PCA_PWM1    =           0xF3;
 sfr         PCA_PWM2    =           0xF4;
 sfr         IAP_TPS     =           0xF5;
 sfr         PWMCFG01    =           0xF6;
 sfr         PWMCFG23    =           0xF7;
 sfr         P7          =           0xf8;
 sbit        P70         =           P7^0;
 sbit        P71         =           P7^1;
 sbit        P72         =           P7^2;
 sbit        P73         =           P7^3;
 sbit        P74         =           P7^4;
 sbit        P75         =           P7^5;
 sbit        P76         =           P7^6;
 sbit        P77         =           P7^7;
 sfr         CH          =           0xF9;
 sfr         CCAP0H      =           0xFA;
 sfr         CCAP1H      =           0xFB;
 sfr         CCAP2H      =           0xFC;
 sfr         PWMCFG45    =           0xFE;
 sfr         RSTCFG      =           0xFF;
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
#line 18 "CODE\cfg.h" /0
 
 
 




 
 
 
 
 
 
 
 
 
 
#line 22 "CODE\os.h" /0
 
 
  
#line 1 "CODE\os_timer.h" /0








 
 
 
 
 




 
 
  
#line 1 "CODE\os.h" /0








 
 
 
#line 12 "CODE\os.h" /1
  
 




 
 
  
 
  
 
  
 
  
 
 




 
 
  
 
 
 
 




 
 
 
 
 
 
 
 
 
 
 
 
 
 
#line 58 "CODE\os.h" /0
 
#line 20 "CODE\os_timer.h" /0
 
 
  
#line 1 "CODE\lib_stdint.h" /0

























 
 
 
 
 
 typedef enum {false = 0, true = 1} boolean;
 
 typedef unsigned char byte;
 typedef unsigned int word;
 
 
 typedef   signed    char int8_t;
 typedef   signed     int int16_t;
 typedef   signed    long int32_t;
 
 typedef unsigned    char uint8_t;
 typedef unsigned     int uint16_t;
 typedef unsigned    long uint32_t;
 
 typedef int32_t  s32;
 typedef int16_t s16;
 typedef int8_t  s8;
 
 typedef const int32_t sc32;   
 typedef const int16_t sc16;   
 typedef const int8_t sc8;    
 
 typedef uint32_t  u32;
 typedef uint16_t u16;
 typedef uint8_t  u8;
 
 typedef const uint32_t uc32;   
 typedef const uint16_t uc16;   
 typedef const uint8_t uc8;    
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
#line 22 "CODE\os_timer.h" /0
 
 
 
 




 
 
 extern volatile u32 data os_tick;    
 extern volatile bit data os_start_enable;    
 
 




 
 
 extern void OS_TIMER_Init(void);
 
 extern u16  OS_TIMER_GetUs(void);
 
 
 
 
#line 24 "CODE\os.h" /0
 
 
  
#line 1 "CODE\lib_stdint.h" /0

























 
 
 
#line 29 "CODE\lib_stdint.h" /1
  
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
  
  
  
 
 
  
  
  
 
 
  
  
  
 
 
 
#line 89 "CODE\lib_stdint.h" /0
 
#line 26 "CODE\os.h" /0
 
 
 




 
 
 
 
 typedef  void (*TaskHook)(void);  
 
 




 
 
 extern void OS_Init(void);
 
 extern void OS_Start(void);
 
 extern void OS_CreateTask(u16 timer, u16 reload, TaskHook task);
 extern void OS_DeleteTask(TaskHook task);
 
 extern void OS_TaskMark(void);
 extern void OS_TaskProcess(void);
 
 
 
 
#line 13 "CODE\os.c" /0
 
 
 
 




 
 
 
 typedef struct OS_TASK
 {
 volatile boolean run;    
 uint16_t timer;          
 uint16_t reload;         
 TaskHook task_handle;    
 } OS_TASK_TYP;
 
 
 OS_TASK_TYP xdata os_task_pool[6];
 
 
 




 
 
 
 void Init_Taks(u8 id)
 {
 if (id >= 6)
 return;
 os_task_pool[id].run = false;         
 os_task_pool[id].timer = 0;           
 os_task_pool[id].reload = 0;          
 os_task_pool[id].task_handle = ((void *)0);  
 }
 
 
 void Init_TaskList(void)
 {
 u8 idata id;
 for (id = 0; id < 6; id++)
 Init_Taks(id);
 }
 








 
 void OS_Init(void)
 {
 Init_TaskList();
 OS_TIMER_Init();
 }
 






 
 void OS_Start(void)
 {
 os_start_enable = 1;
 }
 










 
 void OS_CreateTask(u16 timer, u16 reload, TaskHook task)
 {
 u8 idata id;
 for (id = 0; id < 6; id++)
 {
 if (os_task_pool[id].task_handle == ((void *)0))
 break;
 }
 if (id == 6)  
 return;
 
 os_task_pool[id].run = (timer == 0) ? true : false;
 os_task_pool[id].timer = (timer != 0) ? timer: reload;
 os_task_pool[id].reload = reload;
 os_task_pool[id].task_handle = task;
 }
 








 
 void OS_DeleteTask(TaskHook task)
 {
 u8 idata id;
 
 if (task == ((void *)0))
 return;
 
 for (id = 0; id < 6; id++)
 {
 if (os_task_pool[id].task_handle == task)
 break;
 }
 if (id == 6)  
 return;
 Init_Taks(id);
 
 while ((id + 1) < 6)
 {
 os_task_pool[id].run = os_task_pool[id + 1].run;
 os_task_pool[id].timer = os_task_pool[id + 1].timer;
 os_task_pool[id].reload = os_task_pool[id + 1].reload;
 os_task_pool[id].task_handle = os_task_pool[id + 1].task_handle;
 id++;
 }
 }
 








 
 void OS_TaskMark(void)
 {
 u8 idata id;
 for (id = 0; id < 6; id++)  
 {
 if (os_task_pool[id].timer)  
 {
 os_task_pool[id].timer--;         
 if (os_task_pool[id].timer == 0)  
 {
 os_task_pool[id].timer = os_task_pool[id].reload;  
 os_task_pool[id].run = true;                       
 }
 }
 }
 }
 








 
 uint8_t idata task_index = 0;
 void OS_TaskProcess(void)
 {
 if (os_task_pool[task_index].run == true)
 {
 os_task_pool[task_index].task_handle();  
 os_task_pool[task_index].run = false;    
 
 if (os_task_pool[task_index].reload == 0)  
 {
 OS_DeleteTask(os_task_pool[task_index].task_handle);
 task_index--;    
 }
 }
 if (++task_index >= 6)
 task_index = 0;
 }
 
 
