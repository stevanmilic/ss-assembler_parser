#include "emulator.h"

int emulator(Bit8u* segments, unsigned int size)
{
	if (size > MAX_MEM_SIZE) {
		return -1;
	}
	mem = segments;
	ivt = (Gen32u*) segments;
	mem_size = size;
	intInstruction(0);
	lf.type = t_unknown;
	while (1) {
		Gen32u instr = fetchDWORD();
		Instr_OC instr_oc = decodeInstruction(instr.byte[0]);
		if (instr_oc == -1) {
			continue;
		}
		executeInstruction(instr_oc, instr);
		/* if (change_flags) { */
		/*  flags = isolate_bits(cpu_regs[PSW].byte[0], 4, 0); */
		/*  fillFlags(); */
		/*  cpu_regs[PSW].byte[0] |= flags; */
		/* } */
	}
}

Gen32u fetchDWORD()
{
	Gen32u instruction;
	for (int i = 3; i >= 0; i--) {
		instruction.byte[i] = mem[cpu_regs[PC].dword++];
	}
	return instruction;
}

int decodeInstruction(Bit8 first_byte)
{
	if (checkCondition(isolate_bits(first_byte, 3, 0))) {
		return -1;
	}
	change_flags = first_byte & INSTR_FLAG;
	return bits_to_value(first_byte, 4, 4);
}

int executeInstruction(Instr_OC oc, Gen32u instr)
{
	Bit32 imm;
	Bit32 test;
	Bit32 imm_t;
	switch (oc) {
	case t_int:
		intInstruction(isolate_bits(instr.dword, 4, 8));
		break;
	case t_add:
		if (isolate_bits(instr.dword, 1, 13)) {
			add_immInstruction(bits_to_value(instr.dword, 5, 8), sign_ext(instr.dword, 14));
		} else {
			add_srcInstruction(bits_to_value(instr.dword, 5, 8), bits_to_value(instr.dword, 5, 14));
		}
		break;
	case t_sub:
		if (isolate_bits(instr.dword, 1, 13)) {
			sub_immInstruction(bits_to_value(instr.dword, 5, 8), sign_ext(instr.dword, 14));
		} else {
			sub_srcInstruction(bits_to_value(instr.dword, 5, 8), bits_to_value(instr.dword, 5, 14));
		}
		break;
		break;
	case t_mul:
		if (isolate_bits(instr.dword, 1, 13)) {
			mul_immInstruction(bits_to_value(instr.dword, 5, 8), sign_ext(instr.dword, 14));
		} else {
			mul_srcInstruction(bits_to_value(instr.dword, 5, 8), bits_to_value(instr.dword, 5, 14));
		}
		break;
		break;
	case t_div:
		if (isolate_bits(instr.dword, 1, 13)) {
			div_immInstruction(bits_to_value(instr.dword, 5, 8), sign_ext(instr.dword, 14));
		} else {
			div_srcInstruction(bits_to_value(instr.dword, 5, 8), bits_to_value(instr.dword, 5, 14));
		}
		break;
	case t_cmp:
		if (isolate_bits(instr.dword, 1, 13)) {
			cmp_immInstruction(bits_to_value(instr.dword, 5, 8), sign_ext(instr.dword, 14));
		} else {
			cmp_srcInstruction(bits_to_value(instr.dword, 5, 8), bits_to_value(instr.dword, 5, 14));
		}
		break;
	case t_and:
		andInstruction(bits_to_value(instr.dword, 5, 8), bits_to_value(instr.dword, 5, 13));
		break;
	case t_or:
		orInstruction(bits_to_value(instr.dword, 5, 8), bits_to_value(instr.dword, 5, 13));
		break;
	case t_not:
		notInstruction(bits_to_value(instr.dword, 5, 8), bits_to_value(instr.dword, 5, 13));
		break;
	case t_test:
		testInstruction(bits_to_value(instr.dword, 5, 8), bits_to_value(instr.dword, 5, 13));
		break;
	case t_ldr_str:
		ldrstrInstruction(bits_to_value(instr.dword, 5, 8), bits_to_value(instr.dword, 5, 13), bits_to_value(instr.dword, 3, 18), bits_to_value(instr.dword, 1, 21), sign_ext(instr.dword, 22));
		break;
	case t_call:
		callInstruction(bits_to_value(instr.dword, 5, 8), sign_ext(instr.dword, 13));
		break;
	case t_in_out:
		ioInstruction(bits_to_value(instr.dword, 4, 8), bits_to_value(instr.dword, 4, 12), bits_to_value(instr.dword, 1, 16));
		break;
	case t_mov_shr_shl:
		moveInstruction(bits_to_value(instr.dword, 5, 8), bits_to_value(instr.dword, 5, 13), bits_to_value(instr.dword, 5, 18), bits_to_value(instr.dword, 1, 23));
		break;
	case t_ldc:
		ldcInstruction(bits_to_value(instr.dword, 4, 8), bits_to_value(instr.dword, 1, 12), bits_to_value(instr.dword, 16, 20));
		break;
	default:
		break;
	}
}
