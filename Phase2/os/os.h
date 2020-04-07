
/* Authors: Taylor Pedretti, Jeremy Noble
 * Program Name:
 * Date:
 * Description:
 */
 
#ifndef OS_H
#define OS_H

#include <iostream>
#include <string>
#include <list>
#include <queue>
#include <bitset>

#include "../asm/Assembler.h"
#include "../vm/VirtualMachine.h"

class OS;

// Operating System Class
class OS 
{
private:
	VirtualMachine vm;
	Assembler as;
	
	list<PCB *> jobs;
	queue<PCB *> readyQ;
	queue<PCB *> waitQ;
	PCB * running;
	const static int TIME_SLICE = 15;
	const static int CONTEXT_SWITCH_TIME = 5;
	
	fstream progs;
	int idle_time;
	int sys_time;
public:
	OS();
	~OS();
	void run();
	void run2();
	void loadState();
	void saveState();

	void contextSwitch();
	void getNextProcess();
	int getIOReg(PCB* pcb);
};

#endif
