#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <stdexcept>

using namespace std;

class NullPointerException : public runtime_error {
public:
	NullPointerException() : runtime_error("Null Function Pointer!") { }
};

class Assembler {
	typedef int (Assembler::*FP)(istringstream &);
	map<string, FP> jumpTable;
public:
	Assembler();
	int assemble(fstream&, fstream&);
	int load(istringstream &);
	int loadi(istringstream &);
	int store(istringstream &);
	int add(istringstream &);
	int addi(istringstream &);
	int addc(istringstream &);
	int addci(istringstream &);
	int sub(istringstream &);
	int subi(istringstream &);
	int subc(istringstream &);
	int subci(istringstream &);
	int and_instr(istringstream &);
	int andi(istringstream &);
	int xor_instr(istringstream &);
	int xori(istringstream &);
	int compl_instr(istringstream &);
	int shl(istringstream &);
	int shla(istringstream &);
	int shr(istringstream &);
	int shra(istringstream &);
	int compr(istringstream &);
	int compri(istringstream &);
	int getstat(istringstream &);
	int putstat(istringstream &);
	int jump(istringstream &);
	int jumpl(istringstream &);
	int jumpe(istringstream &);
	int jumpg(istringstream &);
	int call(istringstream &);
	int return_instr(istringstream &);
	int read_instr(istringstream &);
	int write_instr(istringstream &);
	int halt(istringstream &);
	int noop(istringstream &);
}; // Assembler

#endif
