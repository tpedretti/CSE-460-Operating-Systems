#ifndef _VIRTUALMACHINE_H_
#define _VIRTUALMACHINE_H_
#include <vector>
#include <fstream>
#include <bitset>
#include <iostream>

using namespace std;

class VirtualMachine {
private:
	static const int REG_FILE_SIZE = 4;
    static const int MEM_SIZE = 256;
    vector <int> r;
    vector <int> mem;
	int ir;
	bitset<16> sr; //location: 0:Carry, 1:Greater, 2:Equal, 3:Less, 4:Overflow, 5-15:Don't-Care
    int pc, sp, _clock, limit;
	bool halt;
	void executeOp(fstream& inFile, fstream& outFile);
	void setCarry(bitset<2>&);
	void setCarryR(bitset<2>&);
	void signExtend(bitset<2>&);

	void tempCarry(bool test);
	int counter = 1;

public:
	VirtualMachine();
	void run(fstream& obj, fstream& inFile, fstream& outFile);
	bool loadIntoMem(fstream& obj);
	int getClock();	
};
#endif