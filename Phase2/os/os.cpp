
/* Authors: Taylor Pedretti, Jeremy Noble
 * Program Name:
 * Date:
 * Description: os.cpp opens up all .s files into one single program and
 loaded into mem. PCB's are created for each program and their respective info
 is loaded into member variables. Upon time slicing the PCB info is loaded into
 vm and the instructions for that program are executed. 
 */

#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib>

#include "../asm/Assembler.h"
#include "../vm/VirtualMachine.h"
#include "../pcb/PCB.h"
#include "os.h"

using namespace std;

/* Constructor for OS upon being called consolidates
 * all .s files into one file then opens each file
 * as an fstream for passing into the assembler. 
 * Once all .o files have been created*/
OS::OS() 
{	
	// consolidate all .s files into prog
	system("ls *.s > prog");
	
	/* open progs, assembler each file, load object code into
	 * mem then store a pointer to it's PCB in jobs */
	 
	 // open "prog"
	 fstream program; // prog file
	 program.open("prog");
	 if (!program.is_open()) {
		cout << "could not open file\n";
	 }
	
	string inputFile;
	string outputFile;
	string in_fileName;
	string out_fileName;
	string st_fileName;
	string name;
    int pos;
    int base = 0;
    int limit = 0; // start base and limit at zero for first program
    fstream assembly, objectCode, in_File, out_File, st_file;
	int counter = 0;
    
	/* while loop opens up prog file and then each file listed
	 * in prog and assembles each of those files. It then
	 * creates a new PCB object for each program and sets all
	 * the info for each PCB */
	do {
		// take name of each .s name in prog and open the corresponding file
		program >> inputFile;

		cout << "input file: " << inputFile << endl;
			
		pos = inputFile.find(".");
		
		// create .o file name
		name = inputFile.substr(0, pos);
		outputFile = name + ".o";

		
		// open input file and output file for assembler
		assembly.open(inputFile.c_str(), ios::in);
		objectCode.open(outputFile.c_str(), ios::out);
		
		// FIXME need to deal with errors in a different way
		// assemble each program then load it into mem
		if (counter == 0)
		{
			base = 0;
			limit = as.assemble(assembly, objectCode);
		}
		else {
			base += limit;			
			limit = as.assemble(assembly, objectCode);
		}
		objectCode.close();
		
		// load all programs into mem
		/* note: the file must be reopened as an input file to be read */
		objectCode.open(outputFile.c_str(), ios::in);
		vm.loadIntoMem(objectCode);
		
		// close fstream objects
		objectCode.close();
		assembly.close();
		
		// create a new PCB object, name it and set base and limit
		PCB * pcb = new PCB(name, base, limit);
		// add pcb into jobs list
		this->jobs.push_back(pcb);		
		// add pcb into readyQ for start of os
		this->readyQ.push(pcb);

		counter++;
	} while (program.good());
}

void OS::run()
{
	string in_fileName;
	string out_fileName;
	string st_fileName;
	int elapsed_time = 0;
	static const int STANDARD_CYCLE = 10;
	//int clock_dif = 0;
	//int test = 0;
	auto itr = jobs.begin();

	//while loop to open all .in and .out files
	while (itr != jobs.end()) {
		// get file names from each PCB
		in_fileName = (*itr)->prog + ".in";
		out_fileName = (*itr)->prog + ".out";
		st_fileName = (*itr)->prog + ".st";

		// open each file
		(*itr)->in_file.open(in_fileName.c_str(), ios::in);
		(*itr)->out_file.open(out_fileName.c_str(), ios::out);
		(*itr)->st_file.open(st_fileName.c_str(),  ios::in | ios::out | ios::trunc);

		itr++;
	}

	//Start the first process
	getNextProcess();

	//while (!readyQ.empty() || !waitQ.empty())
	while (running)
	{
		elapsed_time = vm.runProcess(running, STANDARD_CYCLE);		
		contextSwitch();
	}
}

/* */
void OS::loadState()
{
	// restore program counter
	this->vm.pc = this->running->pc;
	//cout << "pc: " << vm.pc << endl;
	
	// restore instruction register
	this->vm.ir = this->running->ir;
	//cout << "ir: " << vm.ir << endl;
	
	// restore stack pointer
	this->vm.sp = this->running->sp;
	//cout << "vm sp: " << vm.sp << endl;
	
	// restore status register
	this->vm.sr = this->running->sr; //FIXME sr needs to be updated
	
	// restore each of the general purpose registers
	this->vm.r[0] = this->running->r[0];
	this->vm.r[1] = this->running->r[1];
	this->vm.r[2] = this->running->r[2];
	this->vm.r[3] = this->running->r[3];
	
	// all seems to work correcty...
	/*for (int i = 0; i < this->running->r.size(); i++) {
		cout << "r" << i << ": " << this->running->r[i] << endl;
	}*/
	
	// load stack file into mem. check to see if even necessary first
}

/* */ 
void OS::saveState()
{
	// restore program counter
	this->running->pc = this->vm.pc;
	//cout << "pc: " << vm.pc << endl;
	
	// restore instruction register
	this->running->ir = this->vm.ir;
	//cout << "ir: " << vm.ir << endl;
	
	// restore stack pointer
	this->running->sp = this->vm.sp;
	//cout << "running sp: " << running->sp << endl;
	
	// restore status register
	this->running->sr = this->vm.sr; //FIXME sr needs to be updated
	
	// restore each of the general purpose registers
	this->running->r[0] = this->vm.r[0];
	this->running->r[1] = this->vm.r[1];
	this->running->r[2] = this->vm.r[2];
	this->running->r[3] = this->vm.r[3];
}

/* */
void OS::contextSwitch()
{
	int temp = running->sr.to_ulong() >> 5;
	temp &= 0x07;

	switch(temp)
	{
		case 0: // time-slice
			readyQ.push(running);
			break;

		case 1: // halt
			cout << "entered case 1 halt\n";
			running = nullptr;
			break;
		case 2: // out-of-bound reference FIXME
			cout << "Out-Of-Bound reference!" << endl;
			running = nullptr;
			break;

		case 3: // stack overflow FIXME
			cout << "Stack Overflow!" << endl;
			running = nullptr;
			break;

		case 4: // stack underflow FIXME
			cout << "Stack Underflow" << endl;
			running = nullptr;
			break;

		case 5: // invalid operation
			cout << "Invalid Operation" << endl;
			running = nullptr;
			break;

		case 6: // read
			running->in_file >> running->r[getIOReg(running)];
			waitQ.push(running);
			break;

		case 7: // write
			running->out_file << running->r[getIOReg(running)] << endl;
			waitQ.push(running);
			break;
		default:
			break;
	}

	getNextProcess();
}

void OS::getNextProcess()
{
	PCB* temp;
	if (!readyQ.empty())
	{
		running = readyQ.front();
		readyQ.pop();
	}
	else if (!waitQ.empty())
	{
		temp = waitQ.front();
		waitQ.pop();
		readyQ.push(temp);
		temp = nullptr;

		getNextProcess();
	}
}

int OS::getIOReg(PCB * pcb)
{
	int temp = (pcb->sr.to_ulong() >> 8);
	temp &= 0x01;
	return temp;
}
