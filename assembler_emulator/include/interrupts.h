#ifndef _interrupts_h_
#define _interrupts_h_

#include "emulatorheader.h"

#define IO_SIZE 0xFFFF

#define KEY_CHAR 0x1000
#define KEY_STATUS 0x1010
#define KEY_PRESSED 0x200
#define KEY_OUT 0x2000

#define PSW_INTERRUPT 1
#define INSTR_INTERRUPT 2
#define KEY_INTERRUPT 3
#define TIMER_INTERRUPT 15 

#define PSW_INT_MASK 0x1
#define INSTR_INT_MASK 0x2
#define KEY_INT_MASK 0x4
#define TIMER_INT_MASK 0x8

#define PSW_TIMER_FLAG 0x20000000

void* input();
void* output();
void* psw_timer();
void* timer();

int nextInterrupt();

Bit16u io[IO_SIZE];
Bit8u hardware_int;
Bit8u lock_int;

#endif
