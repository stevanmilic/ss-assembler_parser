#include "emulator.h"

int emulator(Elf32_Emu input)
{
	if (input.segments_size > MAX_MEM_SIZE) {
		return -1;
	}
	mem = input.segments;
	ivt = (Gen32u*) input.segments;
	mem_size = input.segments_size;
	intInstruction(0);
	lf.type = t_unknown;
	while (1) {
		for(int i = 0; i < input.e_phnum; i++){
			if(cpu_regs[PC].dword >= input.phdr[i].p_offset && cpu_regs[PC].dword <= (input.phdr[i].p_offset + input.phdr[i].p_memsz)){
				curr_sh_off = input.phdr[i].p_offset;
				break;
			}
		}
		Gen32u instr = fetchDWORD();
		Instr_OC instr_oc = decodeInstruction(instr.byte[0]);
		if (instr_oc == -1) {
			continue;
		}
		instr.byte[0] = instr.byte[1];
		instr.byte[1] = instr.byte[2];
		instr.byte[2] = instr.byte[3];
		instr.byte[3] = 0;
		executeInstruction(instr_oc, instr);
		/* if (change_flags) { */
		/* 	flags = isolate_bits(cpu_regs[PSW].byte[0], 4, 0); */
		/* 	fillFlags(); */
		/* 	cpu_regs[PSW].byte[0] |= flags; */
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
	switch (oc) {
	case t_int:
		intInstruction(isolate_bits(instr.dword, 4, 0));
		break;
	case t_add:
		if (isolate_bits(instr.dword, 1, 5)) {
			add_immInstruction(isolate_bits(instr.dword, 5, 0), bits_to_value(instr.dword, 18, 6));
		} else {
			add_srcInstruction(isolate_bits(instr.dword, 5, 0), bits_to_value(instr.dword, 5, 6));
		}
		break;
	case t_sub:
		if (isolate_bits(instr.dword, 1, 5)) {
			sub_immInstruction(isolate_bits(instr.dword, 5, 0), bits_to_value(instr.dword, 18, 6));
		} else {
			sub_srcInstruction(isolate_bits(instr.dword, 5, 0), bits_to_value(instr.dword, 5, 6));
		}
		break;
	case t_mul:
		if (isolate_bits(instr.dword, 1, 5)) {
			mul_immInstruction(isolate_bits(instr.dword, 5, 0), bits_to_value(instr.dword, 18, 6));
		} else {
			mul_srcInstruction(isolate_bits(instr.dword, 5, 0), bits_to_value(instr.dword, 5, 6));
		}
		break;
	case t_div:
		if (isolate_bits(instr.dword, 1, 5)) {
			div_immInstruction(isolate_bits(instr.dword, 5, 0), bits_to_value(instr.dword, 18, 6));
		} else {
			div_srcInstruction(isolate_bits(instr.dword, 5, 0), bits_to_value(instr.dword, 5, 6));
		}
		break;
	case t_cmp:
		if (isolate_bits(instr.dword, 1, 5)) {
			cmp_immInstruction(isolate_bits(instr.dword, 5, 0), bits_to_value(instr.dword, 18, 6));
		} else {
			cmp_srcInstruction(isolate_bits(instr.dword, 5, 0), bits_to_value(instr.dword, 5, 6));
		}
		break;
	case t_and:
		andInstruction(isolate_bits(instr.dword, 5, 0), bits_to_value(instr.dword, 5, 5));
		break;
	case t_or:
		orInstruction(isolate_bits(instr.dword, 5, 0), bits_to_value(instr.dword, 5, 5));
		break;
	case t_not:
		notInstruction(isolate_bits(instr.dword, 5, 0), bits_to_value(instr.dword, 5, 5));
		break;
	case t_test:
		testInstruction(isolate_bits(instr.dword, 5, 0), bits_to_value(instr.dword, 5, 5));
		break;
	case t_ldr_str:
		ldrstrInstruction(isolate_bits(instr.dword, 5, 0), bits_to_value(instr.dword, 5, 5), bits_to_value(instr.dword, 3, 10), bits_to_value(instr.dword, 1, 13), bits_to_value(instr.dword, 10, 14));
		break;
	case t_call:
		callInstruction(bits_to_value(instr.dword, 5, 0), bits_to_value(instr.dword, 19, 5));
		break;
	case t_in_out:
		ioInstruction(isolate_bits(instr.dword, 4, 0), bits_to_value(instr.dword, 4, 4), bits_to_value(instr.dword, 1, 8));
		break;
	case t_mov_shr_shl:
		moveInstruction(isolate_bits(instr.dword, 5, 0), bits_to_value(instr.dword, 5, 5), bits_to_value(instr.dword, 5, 10), bits_to_value(instr.dword, 1, 15));
		break;
	case t_ldc:
		ldcInstruction(isolate_bits(instr.dword, 4, 0), bits_to_value(instr.dword, 1, 4), bits_to_value(instr.dword, 16, 8));
		break;
	default:
		break;
	}
}
