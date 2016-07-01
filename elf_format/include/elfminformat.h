#ifndef _elfminformat_h_
#define _elfminformat_h_

typedef struct {
	// int e_machine;//type of machine using 
	int e_shoff;//section header table offset
	int e_phoff;//program header table offset
	int e_shnum;//number of sections in section header
	int e_phnum;//number of segments in program header
	int e_shstrndx;//index of string table in section header table
} Elf32_Ehdr;

typedef enum {
	PT_NULL,
	PT_LOAD,
	PT_INTERP,
	PT_PHDR
} segment_types;

typedef struct {
	int p_type;
	int p_offset;
	// int p_vaddr;
	// int p_addr;
	int p_filesz;
	// int p_memsz;
	int p_flags;
	int p_align;
} Elf32_Phdr;

typedef enum {
	SHT_NULL,//empty section
	SHT_PROGBITS,//the section holds info defined by the program
	SHT_SYMTAB,//the section holds a symbol table
	SHT_STRTAB,//the section holds a string tabble
	SHT_NOBITS,//a section of this type occupies no space in the file
	SHT_REL //the section holds relocation entries
} section_type;

typedef struct {
	int sh_name;//index of section name in string table
	int sh_type;//type of section
	int sh_offset;//offset of first byte from the start of the file
	int sh_size;//size of section in bytes
	int sh_link;//relocation table -> the section header index of the associated section table, in this case it will always be symbol table
	int sh_info;//relocation table -> the section header index to which the relocation applies
} Elf32_Shdr;

typedef enum {
	STB_LOCAL,//local symbols are not visible outside the object file contaning their defition
	STB_GLOBAL//global symbols are visible to all object files being combined
} symbol_type;

typedef enum {
	STT_NOTYPE,//the symbol's type is not specified
	STT_SECTION//the symbol is associated with a section
} symbol_binding;

typedef enum {
	SHN_UNDEF,//undefined
	SHN_ABS = 0xfff1//symbols definied relaive to section number
} section_special_index;

typedef struct {
	int st_name;//index of symbol name in string table
	int st_value;//offset of the beginning of the section
	char st_type;//type of entry
	char st_binding;//global or local?
	int st_shndx;//index of section in the section table header
} Elf32_Sym;

typedef struct {
	int r_offset;//offset of location to apply relocation
	int r_info;//symbol index in symtab
} Elf32_Rel;


#endif //_elfminformat_h_
