/*
*********************************************************************************************************
*                                              uC/OS-II
*                                        The Real-Time Kernel
*
*                    Copyright 1992-2020 Silicon Laboratories Inc. www.silabs.com
*
*                                 SPDX-License-Identifier: APACHE-2.0
*
*               This software is subject to an open source license and is distributed by
*                Silicon Laboratories Inc. pursuant to the terms of the Apache License,
*                    Version 2.0 available at www.apache.org/licenses/LICENSE-2.0.
*
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*
*                                                  H8S
*                                            (Advanced Mode)
*
* Filename : os_cpu.h
* Version  : V2.93.00
*********************************************************************************************************
*/
#ifndef _OS_CPU_H
#define _OS_CPU_H

#ifdef  OS_CPU_GLOBALS
#define OS_CPU_EXT
#else
#define OS_CPU_EXT  extern
#endif

#include  <intrinsics.h>                     /* IAR intrinsic function prototypes                            */

/*
*********************************************************************************************************
*                                              DATA TYPES
*********************************************************************************************************
*/

typedef  unsigned  char    BOOLEAN;
typedef  unsigned  char    INT8U;                    /* Unsigned  8 bit quantity                       */
typedef  signed    char    INT8S;                    /* Signed    8 bit quantity                       */
typedef  unsigned  short   INT16U;                   /* Unsigned 16 bit quantity                       */
typedef  signed    short   INT16S;                   /* Signed   16 bit quantity                       */
typedef  unsigned  long    INT32U;                   /* Unsigned 32 bit quantity                       */
typedef  signed    long    INT32S;                   /* Signed   32 bit quantity                       */
typedef  float             FP32;                     /* Single precision floating point                */
typedef  double            FP64;                     /* Double precision floating point                */

typedef  unsigned  long   OS_STK;                    /* Each stack entry is 24-bit wide (32 bits)      */
typedef  unsigned  char    OS_CPU_SR;                /* CPU Status Register                            */

/*
*********************************************************************************************************
*                                            Hitachi H8S
*
* Method #1:  Disable/Enable interrupts using simple instructions.  After critical section, interrupts
*             will be enabled even if they were disabled before entering the critical section.  You MUST
*             change the constant in OS_CPU_A.ASM, function OSIntCtxSw() from 10 to 8.
*
* Method #2:  Disable/Enable interrupts by preserving the state of interrupts.  In other words, if
*             interrupts were disabled before entering the critical section, they will be disabled when
*             leaving the critical section.  You MUST change the constant in OS_CPU_A.ASM, function
*             OSIntCtxSw() from 8 to 10.
*
* Method #3:  Disable/Enable interrupts by preserving the state of interrupts.  Generally speaking you
*             would store the state of the interrupt disable flag in the local variable 'cpu_sr' and then
*             disable interrupts.  'cpu_sr' is allocated in all of uC/OS-II's functions that need to
*             disable interrupts.  You would restore the interrupt disable state by copying back 'cpu_sr'
*             into the CPU's status register.
*********************************************************************************************************
*/
#define  OS_CRITICAL_METHOD    3


#if OS_CRITICAL_METHOD == 1

#define OS_ENTER_CRITICAL()   __set_imask_ccr(1)

#define OS_EXIT_CRITICAL()    __set_imask_ccr(0)
#endif


#if OS_CRITICAL_METHOD == 2

#define OS_ENTER_CRITICAL()   cpu_sr = __read_ccr(); __or_ccr(0x80)

#define OS_EXIT_CRITICAL()    __write_ccr(cpu_sr);
#endif


#if OS_CRITICAL_METHOD == 3

#define OS_ENTER_CRITICAL()   cpu_sr = __read_ccr(); or_ccr(0x80)

#define OS_EXIT_CRITICAL()    __write_ccr(cpu_sr);
#endif


#define  OS_TASK_SW()         __TRAPA(0)

#define  OS_STK_GROWTH        1                  /* Define stack growth: 1 = Down, 0 = Up              */


/*
*********************************************************************************************************
*                                            GLOBAL VARIABLES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            PROTOTYPES
*********************************************************************************************************
*/
void OSStartHighRdy( void );
void OSIntCtxSw( void );
void OSCtxSw( void );

#endif
