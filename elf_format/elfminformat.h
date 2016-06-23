#ifndef _elfminformat_h_
#define _elfminformat_h_

typedef struct {
	int e_shoff;//section header table offset
	int e_shnum;//number of sections
	int e_shstrndx;//index of string table in section header table
} Elf32_Ehdr;

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
	int sh_link;//relocation table -> the section header index of the associated symbol table
	int sh_info;//relocation table -> the section header index to which the relocation applies
} Elf32_Shdr;

typedef struct {
	int name;//index of symbol name in string table
	int value;//offset of the beginning of the section
	int size;//size of symbol in bytes
	int type;//type of entry
	int binding;//global or local?
	int index;//index of section in the section table header
} Elf32_Sym;

typedef struct {
	int r_offset;//offset of location to apply relocation
} Elf32_Rel;

#endif //_elfminformat_h_
