/*
Written by: Taylor Pedretti
Reviewed by: Jeremy Noble

Takes in info from PCB and breaks it down to different sets of data mostly bitsets.
Bitsets are data types that are a bool arry but only take 1 bit each.
Then runs the OP codes to what needs to be ran.
*/
#ifndef _VIRTUALMACHINE_H_
#define _VIRTUALMACHINE_H_
#include <vector>
#include <fstream>
#include <bitset>
#include <iostream>

#include "../pcb/PCB.h"

class OS;

using namespace std;

class VirtualMachine {
private:
	static const int REG_FILE_SIZE = 4;
    static const int MEM_SIZE = 256;

    vector <int> r;
    vector <int> mem;
	int sr; //location: 0:Carry, 1:Greater, 2:Equal, 3:Less, 4:Overflow, 5-15:Don't-Care
    int pc, ir, sp, _clock, limit, base;
	bool halt;

	//PHASE 2
	void executeOp(fstream& out);

	void setCarry(bitset<2>&);
	void setOverflow(int, int, bitset<2>&);
	void setCarryR(bitset<2>&);
	void signExtend(bitset<2>&);
	void setStatusReg(int status);	
	void setSR(bitset<2> rd);

	//Load a file into memory.
	void loadFile(fstream& objFile);

	void writeStack(fstream& stack_File);
	void readStack(fstream& stack_File);

public:
	VirtualMachine();
	int getClock();

	//Run the Process and current PCB
	int runProcess(PCB * pcb, int time_slice);

	//Old Run...
	void run(fstream& in_file, fstream& out);		

	bool loadIntoMem(fstream& obj);
	void loadIntoMem(PCB* pcb);

	void loadPCB(PCB* pcb);
	void unloadPCB(PCB* pcb);

friend class OS;
};
#endif
