/*
*********************************************************************************************************
*                                                 MODULE
*
* Note(s) : 扩展的数类型和宏
*
*   在标准C语言中基本的数据类型为 char,int,short,long,float,double，而在c51编译器中int,short 相同，float,double 相同
*
* ----------------------------------------------------------
*   数据类型        长度        值域
* ----------------------------------------------------------
*   unsigned char   单字节      0～255
*   signed char     单字节      -128～+127
*   unsigned int    双字节      0～65535
*   signed int      双字节      -32768～+32767
*   unsigned long   四字节      0～4294967295
*   signed long     四字节      -2147483648～+2147483647
*   float           四字节      ±1.175494E-38～±3.402823E+38 符合IEEE-754标准
*	*               1~3字节     对象的地址
*   bit	            位          0 或 1
*   sbit            位          0 或 1
*   sfr             单字节      0～255
*   sfr16           双字节      0～65535
* ----------------------------------------------------------
*********************************************************************************************************
*/

#ifndef __LIB_stdint_H__
#define __LIB_stdint_H__

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

typedef const int32_t sc32;  /*!< Read Only */
typedef const int16_t sc16;  /*!< Read Only */
typedef const int8_t sc8;   /*!< Read Only */

typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;

typedef const uint32_t uc32;  /*!< Read Only */
typedef const uint16_t uc16;  /*!< Read Only */
typedef const uint8_t uc8;   /*!< Read Only */
 

//    /* 7.18.1.4 integer types capable of holding object pointers */
//#if __sizeof_ptr == 8
//typedef   signed       __INT64 intptr_t;
//typedef unsigned       __INT64 uintptr_t;
//#else
//typedef   signed           int intptr_t;
//typedef unsigned           int uintptr_t;
//#endif


    /* minimum values of exact-width signed integer types */
#define INT8_MIN                   -128
#define INT16_MIN                -32768
#define INT32_MIN          (~0x7fffffff)   /* -2147483648 is unsigned */

    /* maximum values of exact-width signed integer types */
#define INT8_MAX                    127
#define INT16_MAX                 32767
#define INT32_MAX            2147483647

    /* maximum values of exact-width unsigned integer types */
#define UINT8_MAX                   255u
#define UINT16_MAX                65535u
#define UINT32_MAX           4294967295ul


#endif /* __LIB_stdint_H__ */

