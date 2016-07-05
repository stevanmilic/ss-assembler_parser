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
		isr_reg |= KEY_INT_MASK;
	}
}

void* output()
{
	io[KEY_OUT] = 0;
	while (1) {
		while (!io[KEY_OUT]);
		Bit16u c = io[KEY_OUT];
		// putchar(c); platfrom independant
		printf("%hu", c);
		fflush(stdout);
		io[KEY_OUT] = 0;
	}
}

void* psw_timer()
{
	while (1) {
		while (!(cpu_regs[PSW].dword & PSW_TIMER_FLAG));
		sleep(1);
		isr_reg |= PSW_INT_MASK;
	}
}

void* timer()
{
	while (1) {
		sleep(1);
		isr_reg |= TIMER_INT_MASK;
	}
}

int nextInterrupt()
{
	//priority -> und instr interrupt
	if (isr_reg & INSTR_INT_MASK) {
		isr_reg &= ~INSTR_INT_MASK;
		return INSTR_INTERRUPT;
	} else if (isr_reg & KEY_INT_MASK) {
		isr_reg &= ~KEY_INT_MASK;
		return KEY_INTERRUPT;
	} else if (isr_reg & PSW_INT_MASK) {
		isr_reg &= ~PSW_INT_MASK;
		return PSW_INTERRUPT;
	} else if (isr_reg & TIMER_INT_MASK) {
		isr_reg &= ~TIMER_INT_MASK;
		return TIMER_INTERRUPT;
	}
}
