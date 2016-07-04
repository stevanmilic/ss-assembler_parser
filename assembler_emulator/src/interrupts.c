#include "interrupts.h"
#include <stdio.h>
#include <unistd.h>

void* input()
{
	io[KEY_STATUS] = 0;
	while (1) {
		/* Bit16u value; */
		/* scanf("%d",&value) */
		/* io[KEY_CHAR] = value; */
		char c = getchar();
		io[KEY_CHAR] = c;
		io[KEY_STATUS] |= KEY_PRESSED;
		hardware_int |= KEY_INT_MASK;
	}
}

void* output()
{
	io[KEY_OUT] = 0;
	while (1) {
		while (!io[KEY_OUT]);
		Bit16u c = io[KEY_OUT];
		// putchar(c); platfrom independant
		printf("%d", c);
		io[KEY_OUT] = 0;
	}
}

void* psw_timer()
{
	while (1) {
		while (!(cpu_regs[PSW].dword & PSW_TIMER_FLAG));
		hardware_int |= PSW_INT_MASK;
		sleep(1);
	}
}

void* timer()
{
	while (1) {
		hardware_int |= TIMER_INT_MASK;
		sleep(1);
	}
}

int nextInterrupt()
{
	//priority -> und instr interrupt
	if (hardware_int & INSTR_INT_MASK) {
		hardware_int &= ~INSTR_INT_MASK;
		return INSTR_INTERRUPT;
	} else if (hardware_int & KEY_INT_MASK) {
		hardware_int &= ~KEY_INT_MASK;
		return KEY_INTERRUPT;
	} else if (hardware_int & PSW_INT_MASK) {
		hardware_int &= ~PSW_INT_MASK;
		return PSW_INTERRUPT;
	} else if (hardware_int & TIMER_INT_MASK) {
		hardware_int &= ~TIMER_INT_MASK;
		return TIMER_INTERRUPT;
	}
}
