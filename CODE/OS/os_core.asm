$NOMOD51                ; disable predefined 8051 registers

; Standard SFR Symbols 
; ACC     DATA    0E0H
; B       DATA    0F0H
; SP      DATA    81H
; DPL     DATA    82H
; DPH     DATA    83H

_AR0    DATA    00H
_AR1    DATA    01H
_AR2    DATA    02H
_AR3    DATA    03H
_AR4    DATA    04H
_AR5    DATA    05H
_AR6    DATA    06H
_AR7    DATA    07H

    NAME    OS_CORE             ; Module name (optional)

?PR?T0_ISR?OS_CORE      SEGMENT CODE 

    EXTRN   CODE (OS_TaskSwitch)
    EXTRN   CODE (OS_TaskMark)
    EXTRN   XDATA (running_task)
    EXTRN   XDATA (origin_sp)
    EXTRN   DATA (os_tick)


CSEG    AT  0000BH              ; 选择绝对段: /*T0中断服务程序*/
    LJMP    T0_ISR

RSEG    ?PR?T0_ISR?OS_CORE      ; 选择可重定位的段: ?PR?T0_ISR?OS_CORE
T0_ISR:                         ; void T0_ISR() interrupt 1
    PUSH    ACC                 ; ==压栈==
    PUSH    B
    PUSH    DPH
    PUSH    DPL
    PUSH    PSW
    PUSH    _AR0
    PUSH    _AR1
    PUSH    _AR2
    PUSH    _AR3
    PUSH    _AR4
    PUSH    _AR5
    PUSH    _AR6
    PUSH    _AR7
    
    MOV     PSW,#00H            ; USING 0
                                ; //记录当前任务栈顶
    MOV     DPTR,#running_task  ;   if (running_task != NULL)
    MOVX    A,@DPTR
    MOV     R7,A
    INC     DPTR
    MOVX    A,@DPTR
    ORL     A,R7
    JZ      ?L_01?T0_ISR
    
    MOV     R7,SP               ;       running_task->stack->sp = (uint8_t idata *)SP;
    MOV     DPTR,#running_task
    MOVX    A,@DPTR
    MOV     R4,A
    INC     DPTR
    MOVX    A,@DPTR
    MOV     R5,A
    MOV      A,R5
    ADD     A,#05H
    MOV     R5,A
    MOV     A,R4
    ADDC    A,#00H
    MOV     R4,A
    MOV     DPL,R5
    MOV     DPH,R4
    MOVX    A,@DPTR
    MOV     R4,A
    INC     DPTR
    MOVX    A,@DPTR
    MOV     R5,A
    MOV     DPL,R5
    MOV     DPH,R4
    INC     DPTR
    INC     DPTR
    MOV     A,R7
    MOVX    @DPTR,A

?L_01?T0_ISR:
    MOV     DPTR,#origin_sp     ;   SP = origin_sp; //这里使用原始栈，为了不破快任务栈
    MOVX    A,@DPTR
    MOV     SP,A

    MOV     A,os_tick+03H       ;   os_tick++;      //tick自增
    ADD     A,#01H
    MOV     os_tick+03H,A
    MOV     A,os_tick+02H
    ADDC    A,#00H
    MOV     os_tick+02H,A
    MOV     A,os_tick+01H
    ADDC    A,#00H
    MOV     os_tick+01H,A
    MOV     A,os_tick
    ADDC    A,#00H
    MOV     os_tick,A
                                ; //任务调度切换
    LCALL   OS_TaskMark         ;   OS_TaskMark();
    LCALL   OS_TaskSwitch       ;   OS_TaskSwitch();
                                ; //SP还原/切换
    MOV     DPTR,#running_task  ;   if (running_task != NULL)
    MOVX    A,@DPTR
    MOV     R7,A
    INC     DPTR
    MOVX    A,@DPTR
    ORL     A,R7
    JZ      ?L_02?T0_ISR

    MOV     DPTR,#origin_sp     ;       origin_sp = SP;
    MOV     A,SP
    MOVX    @DPTR,A

    MOV     DPTR,#running_task  ;       SP = running_task->stack->sp;
    MOVX    A,@DPTR
    MOV     R6,A
    INC     DPTR
    MOVX    A,@DPTR
    MOV     R7,A
    MOV     A,R7
    ADD     A,#05H
    MOV     R7,A
    MOV     A,R6
    ADDC    A,#00H
    MOV     R6,A
    MOV     DPL,R7
    MOV     DPH,R6
    MOVX    A,@DPTR
    MOV     R6,A
    INC     DPTR
    MOVX    A,@DPTR
    MOV     R7,A
    MOV     DPL,R7
    MOV     DPH,R6
    INC     DPTR
    INC     DPTR
    MOVX    A,@DPTR
    MOV     SP,A

?L_02?T0_ISR:
    POP     _AR7                ; ==弹栈==
    POP     _AR6
    POP     _AR5
    POP     _AR4
    POP     _AR3
    POP     _AR2
    POP     _AR1
    POP     _AR0
    POP     PSW
    POP     DPL
    POP     DPH
    POP     B
    POP     ACC
    RETI                        ; RETI
; END OF T0_ISR

    END             ; End Of File