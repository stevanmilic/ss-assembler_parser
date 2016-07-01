#ifndef _INSTRUCTIONASMTOKEN_H
#define _INSTRUCTIONASMTOKEN_H

#include "asmtoken.h"

#define INSTRUCTION_SIZE_BYTES 4
#define INSTRUCTION_SIZE_BITS 32

class InstructionAsmToken: public AsmToken
{
public:
	InstructionAsmToken(int line_number, const boost::match_results<std::string::const_iterator>& content);
	static InstructionAsmToken* getInstruction(int line_number, const boost::match_results<std::string::const_iterator>& content);
	const static boost::regex pattern;
	virtual int getInstrSize() const;
	std::string getName() const;
	std::string getCondition() const;
	std::string getFlag() const;
	virtual std::string getOC() const = 0;
protected:
	std::string name, flag;
};

class InterruptInstructionAsmToken: public InstructionAsmToken
{
public:
	InterruptInstructionAsmToken(int line_number, const boost::match_results<std::string::const_iterator>& content);
	token_type getType() const
	{
		return interrupt_instruction;
	}
	std::string getOC() const;
	char getSrc() const
	{
		return src;
	}
protected:
	char src;
};

class TermInstructionAsmToken: public InstructionAsmToken
{
public:
	TermInstructionAsmToken(int line_number, const boost::match_results<std::string::const_iterator>& content);
	token_type getType() const
	{
		return term_instruction;
	}
	std::string getOC() const;
	std::string getDst() const
	{
		return dst;
	}
	std::string getSrcImm() const
	{
		return src_imm;
	}
protected:
	std::string dst, src_imm;
};

class LogicalInstructionAsmToken: public InstructionAsmToken
{
public:
	LogicalInstructionAsmToken(int line_number, const boost::match_results<std::string::const_iterator>& content);
	token_type getType() const
	{
		return logical_instruction;
	}
	std::string getOC() const;
	std::string getDst() const
	{
		return dst;
	}
	std::string getSrc() const
	{
		return src;
	}
protected:
	std::string dst, src;
};

class StackInstructionAsmToken: public InstructionAsmToken
{
public:
	StackInstructionAsmToken(int line_number, const boost::match_results<std::string::const_iterator>& content);
	token_type getType() const
	{
		return stack_instruction;
	}
	std::string getOC() const;
	std::string getA() const
	{
		return a;
	}
	std::string getR() const
	{
		return r;
	}
	std::string getF() const
	{
		return f;
	}
	std::string getLS() const
	{
		return ls;
	}
	std::string getImm() const
	{
		return imm;
	}
protected:
	std::string a, r, f, ls, imm;
};

class CallInstructionAsmToken: public InstructionAsmToken
{
public:
	CallInstructionAsmToken(int line_number, const boost::match_results<std::string::const_iterator>& content);
	token_type getType() const
	{
		return call_instruction;
	}
	std::string getOC() const;
	std::string getDst() const
	{
		return dst;
	}
	std::string getImm() const
	{
		return imm;
	}
protected:
	std::string imm, dst;
};

class IOInstructionAsmToken: public InstructionAsmToken
{
public:
	IOInstructionAsmToken(int line_number, const boost::match_results<std::string::const_iterator>& content);
	token_type getType() const
	{
		return io_instruction;
	}
	std::string getOC() const;
	std::string getDst() const
	{
		return dst;
	}
	std::string getSrc() const
	{
		return src;
	}
	std::string getIO() const
	{
		return io;
	}
protected:
	std::string dst, src, io;
};

class MoveInstructionAsmToken: public InstructionAsmToken
{
public:
	MoveInstructionAsmToken(int line_number, const boost::match_results<std::string::const_iterator>& content);
	token_type getType() const
	{
		return move_instruction;
	}
	std::string getOC() const;
	std::string getDst() const
	{
		return dst;
	}
	std::string getSrc() const
	{
		return src;
	}
	std::string getImm() const
	{
		return imm;
	}
	std::string getLR() const
	{
		return lr;
	}
protected:
	std::string dst, src, imm, lr;
};

class LoadInstructionAsmToken: public InstructionAsmToken
{
public:
	LoadInstructionAsmToken(int line_number, const boost::match_results<std::string::const_iterator>& content);
	token_type getType() const
	{
		return load_instruction;
	}
	std::string getOC() const;
	std::string getDst() const
	{
		return dst;
	}
	std::string getHL() const
	{
		return hl;
	}
	std::string getC() const
	{
		return c;
	}
	int getInstrSize() const;
protected:
	std::string dst, hl, c;
};
#endif //_INSTRUCTIONASMTOKEN_H
