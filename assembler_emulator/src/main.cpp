#include <iostream>
#include "linkerparser.h"

extern "C" {
	//get declartion for emulator(unsigned char*, unsigned int)
#include "emulator.h"
}

int main(int argc, char* argv[])
{
	if (argc < 3) {
		std::cout << "Not enough arguments!" << std::endl;
		return -2;
	}
	std::vector<char> segments;
	std::vector<Elf32_Phdr> phdr;
	/* -------- start linking -------- */
	Parser* parser = new LinkerParser(++argv, --argc);
	try {
		parser->parse();
		LinkerParser* linkerparser = static_cast<LinkerParser*>(parser);
		linkerparser->writeTables();
		segments = linkerparser->getSegments();
		phdr = linkerparser->getProgramHeader();
	} catch (std::exception& e) {
		std::cout << e.what() << std::endl;
		std::cout << "Linking terminated..." << std::endl;
		exit(-1);
	}
	delete parser;
	/* -------- start emulating -------- */
	Elf32_Emu elf_input;
	elf_input.segments = (unsigned char*)&segments[0];
	elf_input.phdr = &phdr[0];
	elf_input.e_phnum = phdr.size();
	elf_input.segments_size = segments.size();
	emulator(elf_input);
	return 0;
}
