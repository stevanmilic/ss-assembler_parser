#ifndef _elftoken_h_
#define _elftoken_h_

#include "token.h"

class ElfToken : public Token
{
public:
	virtual ~ElfToken() {}
	virtual token_type getType() const = 0;
};

class SectionElfToken: public ElfToken
{
public:
	SectionElfToken(std::string name, int size = 0)
	{
		this->name = name;
		this->size = size;
	}
	token_type getType() const
	{
		return elf_section;
	}
	std::string getName() const
	{
		return name;
	}
	int getSize() const
	{
		return size;
	}
protected:
	std::string name;
	int size;
};

class RelSectionElfToken : public SectionElfToken
{
public:
	RelSectionElfToken(std::string name, int size, std::string link, std::string info) : SectionElfToken(name, size)
	{
		this->link = link;
		this->info = info;
	}
	token_type getType() const
	{
		return elf_reltab;
	}
	std::string getLink() const
	{
		return link;
	}
	std::string getInfo() const
	{
		return info;
	}
protected:
	std::string link, info;
};

class SymbolElfToken : public ElfToken
{
public:
	SymbolElfToken(std::string name, int value, std::string type, std::string section)
	{
		this->name = name;
		this->value = value;
		this->type = type;
		this->section = section;
	}
	token_type getType() const
	{
		return elf_symbol;
	}
	std::string getName() const
	{
		return name;
	}
	int getValue() const
	{
		return value;
	}
	std::string getSectionType() const
	{
		return type;
	}
	std::string getSection() const
	{
		return section;
	}
protected:
	std::string name;
	int value;
	std::string type;// string type
	std::string section;
};

class LinkerSymbolElfToken : public SymbolElfToken
{
public:
	LinkerSymbolElfToken(std::string name, int value, std::string type, std::string section) : SymbolElfToken(name, value, type, section)
	{
	}
	token_type getType() const
	{
		return linker_elf_symbol;
	}
};

class LinkerSectionElfToken: public SectionElfToken
{
public:
	LinkerSectionElfToken(std::string name, int size, std::vector<char> data) : SectionElfToken(name, size)
	{
		this->data = data;
	}
	token_type getType() const
	{
		return linker_elf_section;
	}
	std::vector<char>& getData()
	{
		return data;
	}
protected:
	std::vector<char> data;
};

class RelElfToken : public ElfToken
{
public:
	RelElfToken(int offset, int symbol_index)
	{
		this->offset = offset;
		this->symbol_index = symbol_index;
	}
	token_type getType() const
	{
		return elf_rel;
	}
	int getOffset() const
	{
		return offset;
	}
	int getSymbolIndex() const
	{
		return symbol_index;
	}
protected:
	static int pos_id;
	int id;
	int offset;
	int symbol_index;
};

class LinkerRelElfToken : public ElfToken
{
public:
	LinkerRelElfToken(int offset, std::string symbol_name)
	{
		this->offset = offset;
		this->symbol_name = symbol_name;
	}
	token_type getType() const
	{
		return linker_elf_rel;
	}
	int getOffset() const
	{
		return offset;
	}
	std::string& getSymbolName()
	{
		return symbol_name;
	}
protected:
	int offset;
	std::string symbol_name;
};

class SectionDataElfToken: public ElfToken
{
public:
	SectionDataElfToken(std::string data)
	{
		this->data = data;
	}
	token_type getType() const
	{
		return elf_section_data;
	}
	std::string getData() const
	{
		return data;
	}
protected:
	std::string data;
};

class LinkerAddressToken : public ElfToken
{
public:
	LinkerAddressToken(const boost::match_results<std::string>&);
	virtual ~LinkerAddressToken() {}
	virtual token_type getType() = const = 0;
	unsigned int getAddress();
protected:
	unsigned int address;
	int line_number;
};

class LinkerAddAddressToken : public ElfToken
{
public:
	LinkerAddAddressToken(const boost::match_results<std::string>&);
	const static boost::regex pattern;
};

class LinkerSubAddresToken : public ElfToken
{
public:
	LinkerSubAddressToken(const boost::match_results<std::string>&);
	const static boost::regex pattern;
};

class LinkerAlignAddressToken : public ElfToken
{
public:
	LinkerAlignAddressToken(const boost::match_results<std::string>&);
	const static boost::regex pattern;
};

#endif //_elftoken_h_
