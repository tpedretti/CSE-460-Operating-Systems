#ifndef PCB_H
#define PCB_H

#include <vector>
#include <fstream>
#include <bitset>
#include <string>
using namespace std;

// Program Control Block
class PCB {

	//Virtual Machine Data
	int pc;
	vector <int> r;
	int ir;
	bitset<16> sr;
	int sp;
	int base;
	int limit;

	//Program Name
	string prog;
	
	//Files
	fstream s_file;
	fstream o_file;
	fstream in_file; // goes into .st file
	fstream out_file;
	fstream st_file;

	//--
	int io_completion;
	int turn_around_time;
	int cpu_time;
	int wait_time; int wait_time_begin;
	int io_time; int io_time_begin;

public:
	// parameterized constructor
	PCB(const string &p, const int &b, const int &l) :
		prog(p), base(b), limit(l),
		pc(0), r(4), sp(256),
		cpu_time(0), wait_time(0), io_time(0), 
		wait_time_begin(0) { };
	friend class OS;
	friend class VirtualMachine;
};

#endif