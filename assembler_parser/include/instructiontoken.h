#ifndef _INSTRUCTIONTOKEN_H
#define _INSTRUCTIONTOKEN_H

#include "token.h"

#define INSTRUCTION_SIZE_BYTES 4
#define INSTRUCTION_SIZE_BITS 32

class InstructionToken: public Token
{
public:
	InstructionToken(int line_number, const boost::match_results<std::string::const_iterator>& content);
	static InstructionToken* getInstruction(int line_number, const boost::match_results<std::string::const_iterator>& content);
	const static boost::regex pattern;
	int getInstrSize() const;
	std::string getName() const;
	std::string getCondition() const;
	virtual std::string getFlag() const = 0;
	virtual std::string getOC() const = 0;
protected:
	std::string name;
};

class InterruptInstructionToken: public InstructionToken
{
public:
	InterruptInstructionToken(int line_number, const boost::match_results<std::string::const_iterator>& content);
	token_type getType() const
	{
		return interrupt_instruction;
	}
	std::string getFlag() const;
	std::string getOC() const;
	char getSrc() const
	{
		return src;
	}
protected:
	char src;
};

class TermInstructionToken: public InstructionToken
{
public:
	TermInstructionToken(int line_number, const boost::match_results<std::string::const_iterator>& content);
	token_type getType() const
	{
		return term_instruction;
	}
	std::string getFlag() const;
	std::string getOC() const;
	std::string getDst() const
	{
		return dst;
	}
	std::string getBit() const
	{
		return bit;
	}
	std::string getSrc() const
	{
		return src;
	}
	std::string getImm() const
	{
		return imm;
	}
protected:
	std::string dst, bit, src, imm;
};

class LogicalInstructionToken: public InstructionToken
{
public:
	LogicalInstructionToken(int line_number, const boost::match_results<std::string::const_iterator>& content);
	token_type getType() const
	{
		return logical_instruction;
	}
	std::string getFlag() const;
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

class StackInstructionToken: public InstructionToken
{
public:
	StackInstructionToken(int line_number, const boost::match_results<std::string::const_iterator>& content);
	token_type getType() const
	{
		return stack_instruction;
	}
	std::string getFlag() const;
	std::string getOC() const;
	std::string getA() const
	{
		return a;
	}
	std::string getR() const
	{
		return r;
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

class CallInstructionToken: public InstructionToken
{
public:
	CallInstructionToken(int line_number, const boost::match_results<std::string::const_iterator>& content);
	token_type getType() const
	{
		return call_instruction;
	}
	std::string getFlag() const;
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

class IOInstructionToken: public InstructionToken
{
public:
	IOInstructionToken(int line_number, const boost::match_results<std::string::const_iterator>& content);
	token_type getType() const
	{
		return io_instruction;
	}
	std::string getFlag() const;
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

class MoveInstructionToken: public InstructionToken
{
public:
	MoveInstructionToken(int line_number, const boost::match_results<std::string::const_iterator>& content);
	token_type getType() const
	{
		return move_instruction;
	}
	std::string getFlag() const;
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

class LoadInstructionToken: public InstructionToken
{
public:
	LoadInstructionToken(int line_number, const boost::match_results<std::string::const_iterator>& content);
	token_type getType() const
	{
		return load_instruction;
	}
	std::string getFlag() const;
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
protected:
	std::string dst, hl, c;
};
#endif //_INSTRUCTIONTOKEN_H
