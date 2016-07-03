#include "instructions.h"

int checkCondition(Cond cond)
{
	switch (cond) {
	case cond_eq:
		if (get_ZF())
			return 0;
		break;
	case cond_ne:
		if (!get_ZF())
			return 0;
		break;
	case cond_gt:
		if (get_NF() == get_OF() && !get_ZF())
			return 0;
		break;
	case cond_ge:
		if (get_NF() == get_OF())
			return 0;
		break;
	case cond_lt:
		if (get_NF() != get_OF())
			return 0;
		break;
	case cond_le:
		if (get_NF() != get_OF() || get_ZF())
			return 0;
		break;
	case reserved:
		//no cond specified
		return 0;
		break;
	case cond_al:
		//no condition
		return 0;
		break;
	}
	return 1;//condinition is not satisfied
}

void intInstruction(Bit8u src)
{
	for (int i = 0; i < 4; i++) {
		cpu_regs[PC].byte[i] = ivt[src].byte[3 - i];
	}
}

void add_srcInstruction(Bit8u src, Bit8u dst)
{
	if (change_flags) {
		lf.type = t_add;
	}
	lf.var1 = cpu_regs[dst];
	lf.var1 = cpu_regs[src];
	cpu_regs[dst].dword += cpu_regs[src].dword;
	lf.res = cpu_regs[dst];
}

void sub_srcInstruction(Bit8u src, Bit8u dst)
{
	if (change_flags) {
		lf.type = t_sub;
	}
	lf.var1 = cpu_regs[dst];
	lf.var1 = cpu_regs[src];
	cpu_regs[dst].dword -= cpu_regs[src].dword;
	lf.res = cpu_regs[dst];
}

void mul_srcInstruction(Bit8u src, Bit8u dst)
{
	if (change_flags) {
		lf.type = t_mul;
	}
	cpu_regs[dst].dword *= cpu_regs[src].dword;
	lf.res = cpu_regs[dst];
}

void div_srcInstruction(Bit8u src, Bit8u dst)
{
	if (change_flags) {
		lf.type = t_div;
	}
	cpu_regs[dst].dword /= cpu_regs[src].dword;
	lf.res = cpu_regs[dst];
}

void cmp_srcInstruction(Bit8u src, Bit8u dst)
{
	lf.type = t_cmp;
	lf.var1 = cpu_regs[dst];
	lf.var1 = cpu_regs[src];
	Bit32 cmp_value = cpu_regs[dst].dword - cpu_regs[src].dword;
	lf.res = cpu_regs[dst];
}

void add_immInstruction(Bit8u dst, Bit32 imm)
{
	if (change_flags) {
		lf.type = t_add;
	}
	lf.var1 = cpu_regs[dst];
	lf.var1.dword = imm;
	cpu_regs[dst].dword += imm;
	lf.res = cpu_regs[dst];
}

void sub_immInstruction(Bit8u dst, Bit32 imm)
{
	if (change_flags) {
		lf.type = t_sub;
	}
	lf.var1 = cpu_regs[dst];
	lf.var1.dword = imm;
	cpu_regs[dst].dword -= imm;
	lf.res = cpu_regs[dst];
}

void mul_immInstruction(Bit8u dst, Bit32 imm)
{
	if (change_flags) {
		lf.type = t_mul;
	}
	cpu_regs[dst].dword *= imm;
	lf.res = cpu_regs[dst];
}

void div_immInstruction(Bit8u dst, Bit32 imm)
{
	if (change_flags) {
		lf.type = t_div;
	}
	cpu_regs[dst].dword /= imm;
	lf.res = cpu_regs[dst];
}

void cmp_immInstruction(Bit8u dst, Bit32 imm)
{
	lf.type = t_cmp;
	lf.var1 = cpu_regs[dst];
	lf.var1.dword = imm;
	Bit32 cmp_value = cpu_regs[dst].dword - imm;
	lf.res = cpu_regs[dst];
}

void andInstruction(Bit8u dst, Bit8u src)
{
	if (change_flags) {
		lf.type = t_and;
	}
	cpu_regs[dst].dword &= cpu_regs[src].dword;
	lf.res = cpu_regs[dst];
}

void orInstruction(Bit8u dst, Bit8u src)
{
	if (change_flags) {
		lf.type = t_or;
	}
	cpu_regs[dst].dword |= cpu_regs[src].dword;
	lf.res = cpu_regs[dst];
}

void notInstruction(Bit8u dst, Bit8u src)
{
	if (change_flags) {
		lf.type = t_not;
	}
	cpu_regs[dst].dword = ~cpu_regs[src].dword;
	lf.res = cpu_regs[dst];
}

void testInstruction(Bit8u dst, Bit8u src)
{
	if (change_flags) {
		lf.type = t_test;
	}
	Bit32 test_value = cpu_regs[dst].dword & cpu_regs[src].dword;
	lf.res = cpu_regs[dst];
}

void ldrstrInstruction(Bit8u a, Bit8u r, Bit8u f, Bit8u ls, Bit32 imm)
{
	if (f == 4) {
		cpu_regs[a].dword += 4;
	} else if (f == 5) {
		cpu_regs[a].dword -= 4;
	}
	if (ls) {
		for (int i = 3; i >= 0; i--) {
			cpu_regs[r].byte[i] = mem[curr_sh_off + cpu_regs[a].dword + imm++];
		}
	} else {
		for (int i = 3; i >= 0; i--) {
			mem[cpu_regs[a].dword + imm++] = cpu_regs[r].byte[i];
		}
	}
	if (f == 2) {
		cpu_regs[a].dword += 4;
	} else if (f == 3) {
		cpu_regs[a].dword -= 4;
	}
}

void callInstruction(Bit8u dst, Bit32 imm)
{
	cpu_regs[LR] = cpu_regs[PC];
	cpu_regs[PC].dword = curr_sh_off + cpu_regs[dst].dword + imm;
}

void ioInstruction(Bit8u dst, Bit8u src, Bit8u fio)
{
	if (fio) {
		cpu_regs[dst].dword = io[cpu_regs[src].dword];
		//in
	} else {
		io[cpu_regs[src].dword] = cpu_regs[dst].dword;
		//out
	}
}

void moveInstruction(Bit8u dst, Bit8u src, Bit8u imm, Bit8u lr)
{
	if (!lr) {
		if (change_flags) {
			lf.type = t_shr;
		}
		cpu_regs[dst].dword = imm >> cpu_regs[src].dword;
	} else {
		if (change_flags) {
			lf.type = t_shl;
		}
		cpu_regs[dst].dword = cpu_regs[src].dword << imm;
	}
	lf.res = cpu_regs[dst];
}

void ldcInstruction(Bit8u dst, Bit8u hl, Bit16 c)
{
	if (hl) {
		cpu_regs[dst].word[1] = c;
	} else {
		cpu_regs[dst].word[0] = c;
	}
}

int get_ZF()
{
	if (lf.res.dword == 0) {
		return 1;
	}
	return 0;
}

int get_NF()
{
	if (lf.res.dword & 0x80000000) {
		return 1;
	}
	return 0;
}

int get_CF()
{
	switch (lf.type) {
	case t_add:
		if (lf.res.dword < lf.var1.dword) {
			return 1;
		}
		break;
	case t_sub:
	case t_cmp:
		if (lf.res.dword > lf.var1.dword) {
			return 1;
		}
		break;
	//TO DO: ....
	case t_shr:
		break;
	case t_shl:
		break;
	default:
		return 0;
		break;
	}
	return 0;
}

int get_OF()
{
	switch (lf.type) {
	case t_add:
		if ((lf.var1.dword ^ lf.var2.dword ^ 0x80000000)) {
			return 1;
		}
		break;
	case t_sub:
	case t_cmp:
		if ((lf.var1.dword ^ lf.var2.dword ^ 0x80000000)) {
			return 1;
		}
		break;
defualt:
		return 0;
		break;
	}
	return 0;
}

void fillFlags()
{
	switch (lf.type) {
	case t_add:
		if (lf.res.dword < lf.var1.dword) flags |= FLAG_CF;
		else flags &= ~FLAG_CF;
		if (lf.res.dword == 0) flags |= FLAG_ZF;
		else flags &= ~FLAG_ZF;
		if (((lf.var1.dword ^ lf.var2.dword ^ 0x80000000)))
			flags |= FLAG_OF;
		else flags &= ~FLAG_OF;
		if (lf.res.dword & 0x80000000) flags |= FLAG_NF;
		else flags &= ~FLAG_NF;
		break;
	case t_sub:
	case t_cmp:
	default:
		break;
	}
	lf.type = t_unknown;
}
