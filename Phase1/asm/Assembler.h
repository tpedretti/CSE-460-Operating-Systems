
#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <stdexcept>

using namespace std;

class NullPointerException : public runtime_error {
public:
	NullPointerException() : runtime_error("Null function pointer!") { }
}; 

class Assembler {
	typedef int (Assembler::*FP)(istringstream &);
	map<string, FP> syntax_map;
public:
	Assembler();
	int assemble(fstream& in, fstream& out);
	int construct_inst(const int op, const int rd, 
							const int i, const int addr, 
							const int format,
							bool is_const);
	int load (istringstream& istr);
	int loadi (istringstream& istr);
	int store (istringstream& istr);
	int add (istringstream& istr);
	int addi (istringstream& istr);
	int addc (istringstream& istr);
	int addci (istringstream& istr);
	int sub (istringstream& istr);
	int subi (istringstream& istr);
	int subc (istringstream& istr);
	int subci (istringstream& istr);
	int _and (istringstream& istr);
	int andi (istringstream& istr);
	int _xor (istringstream& istr);
	int xori (istringstream& istr);
	int _compl (istringstream& istr);
	int shl (istringstream& istr);
	int shla (istringstream& istr);
	int shr (istringstream& istr);
	int shra (istringstream& istr);
	int compr (istringstream& istr);
	int compri (istringstream& istr);
	int getstat (istringstream& istr);
	int putstat (istringstream& istr);
	int jump (istringstream& istr);
	int jumpl (istringstream& istr);
	int jumpe (istringstream& istr);
	int jumpg (istringstream& istr);
	int call (istringstream& istr);
	int _return (istringstream& istr);
	int read (istringstream& istr);
	int write (istringstream& istr);
	int halt (istringstream& istr);
	int noop (istringstream& istr);
};

#endif
