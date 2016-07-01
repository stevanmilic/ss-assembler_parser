#ifndef _token_h_
#define _token_h_

enum token_type {label, directive, instruction, section_directive, io_directive, type_directive, align_directive, skip_directive, end_directive, interrupt_instruction, term_instruction, io_instruction, logical_instruction, stack_instruction, call_instruction, move_instruction, load_instruction, elf_section, elf_symbol, elf_reltab, elf_rel, elf_section_data, linker_elf_section, linker_elf_symbol, linker_elf_rel};

class Token
{
public:
	virtual token_type getType() const = 0;
	virtual ~Token() {}
};

#endif //_token_h_
