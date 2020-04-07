
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "Assembler.h"

using namespace std;

Assembler::Assembler()
{
	syntax_map["load"] = &Assembler::load;
	syntax_map["loadi"] = &Assembler::loadi;
	syntax_map["store"] = &Assembler::store;
	syntax_map["add"] = &Assembler::add;
	syntax_map["addi"] = &Assembler::addi;
	syntax_map["addc"] = &Assembler::addc;
	syntax_map["addci"] = &Assembler::addci;
	syntax_map["sub"] = &Assembler::sub;
	syntax_map["subc"] = &Assembler::subc;
	syntax_map["subi"] = &Assembler::subi;
	syntax_map["subci"] = &Assembler::subci;
	syntax_map["_and"] = &Assembler::_and;
	syntax_map["andi"] = &Assembler::andi;
	syntax_map["xor"] = &Assembler::_xor;
	syntax_map["xori"] = &Assembler::xori;
	syntax_map["compl"] = &Assembler::_compl;
	syntax_map["shl"] = &Assembler::shl;
	syntax_map["shla"] = &Assembler::shla;
	syntax_map["shr"] = &Assembler::shr;
	syntax_map["shra"] = &Assembler::shra;
	syntax_map["compr"] = &Assembler::compr;
	syntax_map["compri"] = &Assembler::compri;
	syntax_map["getstat"] = &Assembler::getstat;
	syntax_map["putstat"] = &Assembler::putstat;
	syntax_map["jump"] = &Assembler::jump;
	syntax_map["jumpl"] = &Assembler::jumpl;
	syntax_map["jumpe"] = &Assembler::jumpe;
	syntax_map["jumpg"] = &Assembler::jumpg;
	syntax_map["call"] = &Assembler::call;
	syntax_map["return"] = &Assembler::_return;
	syntax_map["read"] = &Assembler::read;
	syntax_map["write"] = &Assembler::write;
	syntax_map["halt"] = &Assembler::halt;
	syntax_map["noop"] = &Assembler::noop;
}

/* instruction helper reduces redundant code for functions
 * that repeat similar operations. a constant vs addr is
 * determined by a boolean flag passed to it from the
 * instruction function */
int Assembler::construct_inst(const int op, const int rd, 
							const int i, const int addr, 
							const int format,
							bool is_const)
{
	// check for valid register, const and addr values
	if (rd < 0 || rd > 3)
		return -1;
	if (is_const) {
		if (addr < -128 || addr > 127)
			return -1;
	}
	if (!is_const) {
		if (addr < 0 || addr > 255)
			return -1;
	}
	
	int inst = op; // copy op code
	
	if (format == 1) { // format 1
		
		inst = inst<<11 | rd << 9 | i<<8 | addr<<6;
	}
	else if (format == 2) { // format 2
		if (is_const) { // constant value
			inst = inst<<11 | rd << 9 | i<<8 | (0x000000ff & addr);
		}
		else if (not is_const) { // address value
			inst = inst<<11 | rd << 9 | i<<8 | addr;
		}
	}
	
	return inst;
}

/* function parses assembly code file */
int Assembler::assemble(fstream& in, fstream& out)
{
		string line;
		string opcode;
		const int success = false;
		const int error = true;
		const int debug = false;
		int instruction = 0;
		
		// pull single line of code
		getline(in, line);
		
		while (!in.eof()) {
			istringstream str(line.c_str());
			str >> opcode;
			if (opcode[0] == '!') {
				// skip the comment and get a new line
				getline(in, line);
				continue;
			}	
		
		try {
			if (not syntax_map[opcode]) { //FIND WORD IN SYNTAX MAP
				//cout << "opcode: " << opcode << endl;
				throw NullPointerException();
			}
			else instruction = (this->*syntax_map[opcode])(str);
			cout << " instruction: " << instruction << endl;
		} catch (NullPointerException e) {
			cerr << e.what() << endl;
			return error;
		}
		
		if (instruction == -1)
			return error;
		out << instruction << endl; //PUT RETURNED INSTR INTO OUTPUT FILE
		getline(in, line);
	}
	return success;
}

// load
int Assembler::load(istringstream& istr)
{
	
	int rd, addr; 
	istr >> rd >> addr;
	int inst= 0;
	int i = 0;
	
	return construct_inst(inst, rd, i, addr, 2, false);
}

 // load immediate
int Assembler::loadi(istringstream& istr)
{
	int rd, addr; 
	istr >> rd >> addr;
	int inst= 0;
	int i = 1;
	
	return construct_inst(inst, rd, i, addr, 2, true);
}

// store
int Assembler::store(istringstream& istr)
{
	int rd, addr; 
	istr >> rd >> addr;
	int inst= 1;
	int i = 1;
	
	return construct_inst(inst, rd, i, addr, 2, false);
}

// add
int Assembler::add(istringstream& istr)
{
	int rd, addr; 
	istr >> rd >> addr;
	int inst= 2;
	int i = 0;
	
	return construct_inst(inst, rd, i, addr, 2, true);
}

// add immediate
int Assembler::addi(istringstream& istr)
{
	int rd, addr; 
	istr >> rd >> addr;
	int inst= 2;
	int i = 1;
	
	return construct_inst(inst, rd, i, addr, 2, true);
}

// addc
int Assembler::addc(istringstream& istr)
{
	int rd, addr; 
	istr >> rd >> addr;
	int inst= 3;
	int i = 0;
	
	return construct_inst(inst, rd, i, addr, 1, false);
}

// addci
int Assembler::addci(istringstream& istr)
{
	int rd, addr; 
	istr >> rd >> addr;
	int inst= 3;
	int i = 1;
	
	return construct_inst(inst, rd, i, addr, 2, true);
}

// sub
int Assembler::sub(istringstream& istr)
{
	int rd, addr; 
	istr >> rd >> addr;
	int inst= 4;
	int i = 0;
	
	return construct_inst(inst, rd, i, addr, 1, false);
}

// subi
int Assembler::subi(istringstream& istr)
{
	int rd, addr; 
	istr >> rd >> addr;
	int inst= 4;
	int i = 1;
	
	return construct_inst(inst, rd, i, addr, 2, true);
}

// subc
int Assembler::subc(istringstream& istr)
{
	int rd, addr; 
	istr >> rd >> addr;
	int inst= 5;
	int i = 0;
	
	return construct_inst(inst, rd, i, addr, 1, false);
}

// subci
int Assembler::subci(istringstream& istr)
{
	int rd, addr; 
	istr >> rd >> addr;
	int inst= 5;
	int i = 1;
	
	return construct_inst(inst, rd, i, addr, 2, true);
}

// and
int Assembler::_and(istringstream& istr)
{
	int rd, addr; 
	istr >> rd >> addr;
	int inst= 6;
	int i = 0;
	
	return construct_inst(inst, rd, i, addr, 1, false);
}

// andi
int Assembler::andi(istringstream& istr)
{
	int rd, addr; 
	istr >> rd >> addr;
	int inst= 6;
	int i = 1;
	
	return construct_inst(inst, rd, i, addr, 2, true);
}

// xor
int Assembler::_xor(istringstream& istr)
{
	int rd, addr; 
	istr >> rd >> addr;
	int inst= 7;
	int i = 0;
	
	return construct_inst(inst, rd, i, addr, 1, false);
}

// xori
int Assembler::xori(istringstream& istr)
{
	int rd, addr; 
	istr >> rd >> addr;
	int inst= 7;
	int i = 1;
	
	return construct_inst(inst, rd, i, addr, 2, true);
}

// compl
int Assembler::_compl(istringstream& istr)
{
	int rd; 
	const int addr = 0;
	istr >> rd;
	int inst = 8;
	int i = 0;
	
	return construct_inst(inst, rd, i, addr, 1, false);
}

// shl
int Assembler::shl(istringstream& istr)
{
	int rd; 
	const int addr = 0;
	istr >> rd;
	int inst = 9;
	int i = 0;
	
	return construct_inst(inst, rd, i, addr, 2, false);
}

// shla
int Assembler::shla(istringstream& istr)
{
	int rd; 
	const int addr = 0;
	istr >> rd;
	int inst = 9;
	int i = 0;
	
	return construct_inst(inst, rd, i, addr, 2, false);
}

// shr
int Assembler::shr(istringstream& istr)
{
	int rd; 
	const int addr = 0;
	istr >> rd;
	int inst = 11;
	int i = 0;
	
	return construct_inst(inst, rd, i, addr, 2, false);
}

// shra
int Assembler::shra(istringstream& istr)
{
	int rd; 
	const int addr = 0;
	istr >> rd;
	int inst = 12;
	int i = 0;
	
	return construct_inst(inst, rd, i, addr, 2, false);
}

// compare
int Assembler::compr(istringstream& istr)
{
	int rd, addr; 
	istr >> rd >> addr;
	int inst= 13;
	int i = 0;
	
	return construct_inst(inst, rd, i, addr, 1, false);
}

// compare immediate
int Assembler::compri(istringstream& istr)
{
	int rd, addr; 
	istr >> rd >> addr;
	int inst= 13;
	int i = 1;
	
	return construct_inst(inst, rd, i, addr, 2, true);
}

// get status
int Assembler::getstat(istringstream& istr)
{
	int rd; 
	const int addr = 0;
	istr >> rd;
	int inst = 14;
	int i = 0;
	
	return construct_inst(inst, rd, i, addr, 2, false);
}

// put status
int Assembler::putstat(istringstream& istr)
{
	int rd; 
	const int addr = 0;
	istr >> rd;
	int inst = 15;
	int i = 0;
	
	return construct_inst(inst, rd, i, addr, 2, false);
}

// jump
int Assembler::jump(istringstream& istr)
{
	int addr; 
	const int rd = 0;
	istr >> addr;
	int inst= 16;
	int i = 1;
	
	return construct_inst(inst, rd, i, addr, 2, false);
}

// jumpl
int Assembler::jumpl(istringstream& istr)
{
	int addr; 
	const int rd = 0;
	istr >> addr;
	int inst= 17;
	int i = 1;
	
	return construct_inst(inst, rd, i, addr, 2, false);
}

// jumpe
int Assembler::jumpe(istringstream& istr)
{
	int addr; 
	const int rd = 0;
	istr >> addr;
	int inst= 18;
	int i = 1;
	
	return construct_inst(inst, rd, i, addr, 2, false);
}

// jumpg
int Assembler::jumpg(istringstream& istr)
{
	int addr; 
	const int rd = 0;
	istr >> addr;
	int inst= 19;
	int i = 1;
	
	return construct_inst(inst, rd, i, addr, 2, false);
}

// call
int Assembler::call(istringstream& istr)
{
	int addr; 
	const int rd = 0;
	istr >> addr;
	int inst= 20;
	int i = 1;
	
	return construct_inst(inst, rd, i, addr, 2, true);
}

// return
int Assembler::_return(istringstream& istr)
{
	const int rd = 0;
	const int addr = 0;
	int inst= 21;
	int i = 0;
	
	return construct_inst(inst, rd, i, addr, 1, false);
}

// read
int Assembler::read(istringstream& istr)
{
	int rd;
	const int addr = 0; 
	istr >> rd;
	int inst= 22;
	int i = 0;
	
	return construct_inst(inst, rd, i, addr, 2, true);
}

// write
int Assembler::write(istringstream& istr)
{
	const int rd = 0;
	const int addr = 0;
	int inst= 23;
	int i = 0;
	
	return construct_inst(inst, rd, i, addr, 2, false);
}

// halt
int Assembler::halt(istringstream& istr)
{
	const int rd = 0;
	const int addr = 0;
	int inst= 24;
	int i = 0;
	
	return construct_inst(inst, rd, i, addr, 2, false);
}

// noop
int Assembler::noop(istringstream& istr)
{
	const int rd = 0;
	const int addr = 0;
	int inst= 25;
	int i = 0;
	
	return construct_inst(inst, rd, i, addr, 2, false);
}
